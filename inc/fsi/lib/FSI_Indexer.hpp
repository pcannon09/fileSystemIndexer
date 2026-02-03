#ifndef INCLUDE_LIB_FSI_INDEXER_HPP_
#define INCLUDE_LIB_FSI_INDEXER_HPP_

#include <string>
#include <vector>

#define __FSI_SUCCESS 		0
#define __FSI_INDEXERERR_CHECK(e) \
	(e.raise || e.fatal) && e.code != __FSI_SUCCESS

namespace fsi
{
	namespace codes
	{
		constexpr int ERROR_ADDITION_FAIL 		= 1;
		constexpr int ERROR_ERASE_FAIL 			= 2;
		constexpr int ERROR_INVALID_PATH 		= 3;
		constexpr int ERROR_EMPTY_VALUE 		= 4;
	}

	enum class IndexerPathType
	{
		Directory,
		File,
		SymLink
	};

	typedef struct IndexerDateTime
	{
		unsigned int hour;
		unsigned int min;
		unsigned int sec;
		unsigned int ms;

		unsigned int year;
		unsigned int month;
		unsigned int day;
	} IndexerDateTime;

	typedef struct IndexerFoundInfo
	{
		std::string path; // /home/user/projects, ...
		std::string name; // main, ...
		std::string extension; // .txt, .png, .cpp, ...
		std::string extType; // Audio, Image, Text, Code, Other, ...

		IndexerDateTime createdDT;
		IndexerDateTime lastModDT;
	} IndexerFoundInfo;

	typedef struct IndexerInfo
	{
		std::string id; // Optional
		std::string path; // Dir to add; (Indexer::addInfo())
						  // OR dir to start adding; (Indexer::addExtendedInfo())

		IndexerPathType pathType;
	} IndexerInfo;

	typedef struct IndexerError
	{
		std::string message;

		int code = __FSI_SUCCESS;

		bool fatal = false;
		bool raise = true;
	} IndexerError;

	class Indexer
	{
	private:
		std::string id;
		std::vector<IndexerInfo> indexerInfo;

	protected:
		virtual IndexerError __fileAdd(const std::string &searcher, IndexerInfo info);
		virtual IndexerPathType __getPathType(const std::string &path) const;
		virtual std::vector<std::string> __iteratePath(const std::string &path);

	public:
		Indexer(const std::string &id);

		IndexerError addInfo(const IndexerInfo &info);
		IndexerError addExtendedInfo(const IndexerInfo &info);
		IndexerError removeInfo(const std::string &searcher);

		IndexerInfo findIndex(const std::string &toFind);

		std::string searchExactMatching(const std::string &find);

		std::vector<std::string> searchMatching(const std::string &path);

		std::vector<IndexerInfo> getIndexerInfo() const;
		std::vector<std::string> getIndexPaths() const;
		std::string getID() const;
	};
}

#endif  // INCLUDE_LIB_FSI_INDEXER_HPP_
