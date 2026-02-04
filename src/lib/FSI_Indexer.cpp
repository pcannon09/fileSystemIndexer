#include <filesystem>
#include <algorithm>

#include "../../inc/fsi/lib/FSI_Indexer.hpp"
#include "../../inc/fsi/core/FSI_timeUtils.hpp"

#include "../../inc/fsi/core/FSI_dirUtils.h"

#include "ciof/ciof.hpp"

namespace fs = std::filesystem;

namespace fsi
{
	// PROTECTED //
	IndexerError Indexer::__fileAdd(const std::string &searcher, IndexerInfo info)
	{
		IndexerError error = this->removeInfo(searcher);

		if (__FSI_INDEXERERR_CHECK(error))
			return error;

		if (info.pathType == IndexerPathType::SymLink)
			info.path = fs::read_symlink(info.path);

		return this->addInfo(info);
	}

	IndexerPathType Indexer::__getPathType(const std::string &path) const
	{
		if (fs::is_directory(path)) return IndexerPathType::Directory;
		else if (fs::is_symlink(path)) return IndexerPathType::SymLink;
		else return IndexerPathType::File;
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
        		paths.emplace_back(p);

        		FSI_FREE(p);
        	}
    	}

    	cvec_destroy(&tmpPathsVec);

    	return paths;
	}


	// PUBLIC //
	Indexer::Indexer(const std::string &id)
		: id(id)
	{ }

	IndexerError Indexer::addExtendedInfo(const IndexerInfo &info)
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

	std::string Indexer::searchExactMatching(const std::string &find)
	{
		if (find.empty())
			return "";

		return this->findIndex(find).path;
	}

	std::vector<std::string> Indexer::searchMatching(const std::string &find)
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

