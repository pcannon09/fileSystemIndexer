#include <filesystem>
#include <algorithm>
#include <future>
#include <execution>

#include "../../inc/fsi/lib/FSI_Indexer.hpp"
#include "../../inc/fsi/core/FSI_timeUtils.hpp"

#include "../../inc/fsi/core/FSI_dirUtils.h"

#include "../../inc/fsi/FSIpredefines.h"

#include "ciof/ciof.hpp"

namespace fs = std::filesystem;

namespace fsi
{
	// PROTECTED //
	IndexerPathType Indexer::__getPathType(const std::string &path) const
	{
		if (fs::is_directory(path)) return IndexerPathType::Directory;
		else if (fs::is_symlink(path)) return IndexerPathType::SymLink;

		return IndexerPathType::File;
	}

	std::vector<std::string> Indexer::__searchMatchingStandard(const std::string &find)
	{
		if (find.empty())
			return {};

		std::vector<std::string> vec;

		for (const auto &x : this->indexerInfo)
		{
			if (x.path.find(find) != std::string::npos)
				vec.emplace_back(x.path);
		}

		return vec;
	}

	std::vector<std::string> Indexer::__searchMatchingThreaded(const std::string& find)
	{
    	if (find.empty())
        	return {};

    	std::vector<std::string> result;
    	std::mutex mutex;

    	std::for_each(
        		std::execution::par,
        		indexerInfo.begin(),
        		indexerInfo.end(),
        		[&](const auto& x)
        	{
            	if (x.path.find(find) != std::string::npos)
            	{
                	std::lock_guard<std::mutex> lock(mutex);
                	result.emplace_back(x.path);
            	}
        	}
    	);

    	return result;
	}

	IndexerError Indexer::__addExtendedInfoStandard(const IndexerInfo &info)
	{
		IndexerError error = this->addInfo(info);
		IndexerInfo totalInfo = info;

		if (__FSI_INDEXERERR_CHECK(error))
			return error;

		const std::vector<std::string> iteratedPaths = this->__iteratePath(info.path);

		for (const auto &x : iteratedPaths)
		{
			totalInfo.pathType = this->__getPathType(totalInfo.path);
			totalInfo.path = x;

			this->indexerInfo.emplace_back(totalInfo);
		}

		return error;
	}

	IndexerError Indexer::__addExtendedInfoThreaded(const IndexerInfo &info)
	{
		IndexerError error = this->addInfo(info);

		if (__FSI_INDEXERERR_CHECK(error))
			return error;

		const auto paths = this->__iteratePath(info.path);
		const auto &chunks = this->__splitPathByCores(paths);

		std::vector<std::future<std::vector<IndexerInfo>>> tasks;

		for (auto &chunk : chunks)
		{
			if (chunk.empty())
				continue;

			tasks.emplace_back(std::async(std::launch::async,
						[&, chunk]() -> std::vector<IndexerInfo>
			{
				std::vector<IndexerInfo> local;

				local.reserve(chunk.size());

				for (const auto &p : chunk)
				{
					IndexerInfo tmp = info;
					tmp.path = p;
					tmp.pathType = this->__getPathType(p);
					local.emplace_back(std::move(tmp));
				}

				return local;
			}));
		}

		// Merging phase
		for (auto &t : tasks)
		{
			auto local = t.get();

			this->indexerInfo.insert(
					this->indexerInfo.end(),
					std::make_move_iterator(local.begin()),
					std::make_move_iterator(local.end()));
		}

		return error;
	}

	std::vector<std::string> Indexer::__iteratePath(const std::string &path)
	{
		std::vector<std::string> paths;

		CVEC tmpPathsVec = cvec_init(-1, sizeof(char*));

		fsi_walk(&tmpPathsVec, path.c_str());

		for (size_t i = 0; i < tmpPathsVec.size; ++i)
		{
			char* p = *(char**)cvec_get(&tmpPathsVec, i);

			if (p)
			{
				paths.emplace_back(p, strlen(p));

				FSI_FREE(p);
			}
		}

		cvec_destroy(&tmpPathsVec);

		return paths;
	}

	// PUBLIC //
	// CONSTRUCTOR
	Indexer::Indexer(const std::string &id, const bool threadsImpl)
		: id(id), threadsImpl(threadsImpl)
	{ }

	// DECONSTRUCTOR
	Indexer::~Indexer()
	{ }

	IndexerError Indexer::addExtendedInfo(const IndexerInfo &info)
	{
		if (this->threadsImpl)
			return this->__addExtendedInfoThreaded(info);
		
		return this->__addExtendedInfoStandard(info);
	}

	std::string Indexer::searchExactMatching(const std::string &find)
	{
		if (find.empty())
			return "";

		return this->findIndex(find).path;
	}

	std::vector<std::string> Indexer::searchMatching(const std::string &find)
	{
		if (this->threadsImpl) return this->__searchMatchingThreaded(find);

		return this->__searchMatchingStandard(find);
	}

	IndexerError Indexer::addInfo(const IndexerInfo &info)
	{
		IndexerError error;
		IndexerInfo totalInfo = info;

		// Do checks before adding
		if (totalInfo.path.empty())
		{
			error.code = fsi::codes::ERROR_ADDITION_FAIL;
			error.message = "Path is empty";
			error.fatal = false;
			error.raise = false;

			return error;
		}

		if (!fs::exists(totalInfo.path))
		{
			error.code = fsi::codes::ERROR_INVALID_PATH;
			error.message = ciof::format("Unknown path `%1`; No such file or directory exists", totalInfo.path);
			error.fatal = true;

			return error;
		}

		
		// Set path type
		totalInfo.pathType = this->__getPathType(totalInfo.path);

		// Add final information

		this->indexerInfo.emplace_back(totalInfo);

		return error;
	}

	IndexerInfo Indexer::findIndex(const std::string &toFind)
	{
		for (const auto &x : this->indexerInfo)
		{
			if (x.path == toFind || x.id == toFind)
				return x;
		}

		return { };
	}

	IndexerError Indexer::removeInfo(const std::string &searcher)
	{
		IndexerError error;

		// Do checks

		if (searcher.empty())
		{
			error.code = fsi::codes::ERROR_EMPTY_VALUE;
			error.message = "Empty value to remove from the Indexer Information list";
			error.fatal = false;
			error.raise = false;

			return error;
		}

		// Erase the found object (IndexerInfo) value by ID or Path
		this->indexerInfo.erase(
				std::remove_if(
					this->indexerInfo.begin(),
					this->indexerInfo.end(),
					[&](const IndexerInfo &info) {
						return (info.id == searcher || info.path == searcher);
					}),
				this->indexerInfo.end()
		);

		return error;
	}

	utils::TimeUtils_DateTime Indexer::getFileDTInfo(const std::string &path)
	{
		const fs::file_time_type &lastWriteTime = fs::last_write_time(path);

		return utils::fsClockDataToDT(lastWriteTime);
	}

	std::vector<IndexerInfo> Indexer::getIndexerInfo() const
	{ return this->indexerInfo; }

	std::vector<std::string> Indexer::getIndexPaths() const
	{
		std::vector<std::string> additions;

		for (const auto &x : this->indexerInfo)
		{ additions.emplace_back(x.path); }

		return additions;
	}

	std::string Indexer::getID() const
	{ return this->id; }
}

