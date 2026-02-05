#ifndef INCLUDE_LIB_FSI_INDEXER_HPP_
#define INCLUDE_LIB_FSI_INDEXER_HPP_

#include <string>
#include <vector>

#include "../core/FSI_timeUtils.hpp"

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

	typedef struct IndexerFoundInfo
	{
		std::string path; // /home/user/projects, ...
		std::string name; // main, ...
		std::string extension; // .txt, .png, .cpp, ...
		std::string extType; // Audio, Image, Text, Code, Other, ...

		utils::TimeUtils_DateTime lastModDT; // Last modified path
	} IndexerFoundInfo;

	typedef struct IndexerInfo
	{
		std::string id; // Optional
		std::string path; // Dir to add; (Indexer::addInfo())
						  // OR dir to start adding; (Indexer::addExtendedInfo())

		IndexerPathType pathType; // Path type (Dir, File, SymLink, Others)
	} IndexerInfo;

	typedef struct IndexerError
	{
		std::string message; // Message of error

		int code = __FSI_SUCCESS; // Return code (Default is `__FSI_SUCCESS` or `0`)

		bool fatal = false; // Should it count as an important error? Controlled by the developer
		bool raise = true; // Should raise an exception? Controlled by the developer
	} IndexerError;

	class Indexer
	{
	private:
		std::string id;
		std::vector<IndexerInfo> indexerInfo;

	protected:
		/**
		 * @brief Get if path from function param is a:
		 * 	* File
		 * 	* Dir
		 * 	* SymLink
		 * 	* Others
		 * @param path Path to get the type from
		 * @return IndexerPathType The type of the function param
		 */
		virtual IndexerPathType __getPathType(const std::string &path) const;

		/**
		 * @brief Iterate all the files and directories (symlinks too) from `path`
		 * @param path Path to iterate
		 * @return A tree of full path in a vector of strings
		 */
		virtual std::vector<std::string> __iteratePath(const std::string &path);

	public:
		Indexer(const std::string &id);
		~Indexer();

		/**
		 * @brief Add the exact path to have the info from
		 * @param info Get the information from the exact path
		 * @return IndexerError Return error or success from `IndexerError` type
		 */
		IndexerError addInfo(const IndexerInfo &info);

		/**
		 * @brief Add the exact path and the sub-paths to have the info from
		 * @param info Get the information from the exact path and deeper
		 * @return IndexerError Return error or success from `IndexerError` type
		 */
		IndexerError addExtendedInfo(const IndexerInfo &info);

		/**
		 * @brief Remove information from its ID or full path
		 * @param searcher Search for the path or ID and remove them from the indexer
		 * @return IndexerError Return error or success from `IndexerError` type
		 */
		IndexerError removeInfo(const std::string &searcher);

		/**
		 * @brief Find the index from the vector database from the indexer
		 * @param toFind Search for the path or ID and remove them from the indexer
		 * @return IndexerInfo Find the indexed info from the indexer "data base"
		 */
		IndexerInfo findIndex(const std::string &toFind);

		/**
		 * @brief Get when the file, dir or symlink was last modified
		 * @param path Path to get date time information
		 * @return Date time info
		 */
		utils::TimeUtils_DateTime getFileDTInfo(const std::string &path);

		/**
		 * @brief Search for an exact matching path or ID;
		 * @param find Get the ID or path to find
		 * @return The first match found
		 */
		std::string searchExactMatching(const std::string &find);

		/**
		 * @brief Search all matching path or ID that contains `path`
		 * @param path Path or ID to search
		 * @return A vector of all the found `path`s
		 */
		std::vector<std::string> searchMatching(const std::string &path);

		/**
		 * @brief Return indexer information from this object
		 * @return Get all the indexed information that was added
		 */
		std::vector<IndexerInfo> getIndexerInfo() const;

		/**
		 * @brief Get all the indexed paths from the vector indexes
		 * @return All the indexed paths in a vector
		 */
		std::vector<std::string> getIndexPaths() const;

		/**
		 * @brief Get this object's ID
		 * @return Object ID
		 */
		std::string getID() const;
	};
}

#endif  // INCLUDE_LIB_FSI_INDEXER_HPP_
