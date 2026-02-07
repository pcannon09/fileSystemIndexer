#ifndef INCLUDE_CORE_FSI_DIRUTILS_H_
#define INCLUDE_CORE_FSI_DIRUTILS_H_

#ifdef __cplusplus
# 	define __FSI_DIRUTILS_CPP_OPEN 		extern "C" {
# 	define __FSI_DIRUTILS_CPP_CLOSE 	}
#else
# 	define __FSI_DIRUTILS_CPP_OPEN
# 	define __FSI_DIRUTILS_CPP_CLOSE
#endif // __cplusplus

__FSI_DIRUTILS_CPP_OPEN

#include "../FSIpredefines.h"

#include "cvec/cvec.h"

typedef enum FSI_EntryType
{
	FSI_FILE,
	FSI_DIR,
	FSI_SYMLINK,
	FSI_OTHER
} FSI_EntryType;

typedef struct FSI_DirData
{
	void *impl;
} FSI_DirData;

typedef struct FSI_EntryData
{
	const char *name; // Valid until next read

	FSI_EntryType type;
} FSI_EntryData;

/**
 * @brief Walk to every directory and file and set `vec` to a CVEC value
 * @param vec The vector to set data to
 * @param path Path to walk to all its sub-dirs
 */
void fsi_walk(CVEC *vec, const char *path);

__FSI_DIRUTILS_CPP_CLOSE

#endif  // INCLUDE_CORE_FSI_DIRUTILS_H_
