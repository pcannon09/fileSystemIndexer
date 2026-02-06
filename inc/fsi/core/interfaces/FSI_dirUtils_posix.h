#ifndef INCLUDE_INTERFACES_FSI_DIRUTILS_POSIX_H_
#define INCLUDE_INTERFACES_FSI_DIRUTILS_POSIX_H_

#ifdef __cplusplus
# 	define __FSI_DIRUTILS_POSIX_CPP_OPEN 		extern "C" {
# 	define __FSI_DIRUTILS_POSIX_CPP_CLOSE 		}
#else
# 	define __FSI_DIRUTILS_POSIX_CPP_OPEN
# 	define __FSI_DIRUTILS_POSIX_CPP_CLOSE
#endif // __cplusplus

__FSI_DIRUTILS_POSIX_CPP_OPEN

#include "../../FSIpredefines.h"

#ifndef FSI_OS_WIN32

#include "../FSI_dirUtils.h"

#include "cstr/cstr.h"

#include <dirent.h>

typedef struct FSI_DirData_POSIX
{
	DIR *dir;
	CSTR path;
} FSI_DirData_POSIX;

/**
 * @brief Open directory as read only
 * @param d Directory data
 * @param path Path to open dir
 * @return Status (1 if worked, 0 if failed)
 */
int fsi_openDir(FSI_DirData *d, const char *path);

/**
 * @brief Read directory from reading the `FSI_DirData` information
 * @param d Directory data
 * @param out Entry data to set the information
 * @return Status (1 if worked, 0 if failed)
 */
int fsi_readDir(FSI_DirData *d, FSI_EntryData *out);

/**
 * @brief Get the directory data to deallocate memory
 * @param d Object to deallocate
 */
void fsi_closeDir(FSI_DirData *d);

#endif // FSI_OS_WIN32

__FSI_DIRUTILS_POSIX_CPP_CLOSE

#undef __FSI_DIRUTILS_POSIX_CPP_OPEN
#undef __FSI_DIRUTILS_POSIX_CPP_CLOSE

#endif  // INCLUDE_CORE_FSI_DIRUTILS_H_
