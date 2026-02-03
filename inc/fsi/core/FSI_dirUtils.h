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

#include <dirent.h>
#include <string.h>

#include "cvec/cvec.h"

typedef enum FSI_Type
{
	FSI_FILE,
	FSI_DIR,
	FSI_SYMLINK,
	FSI_OTHER
} FSI_Type;

typedef struct FSI_DirData
{
	void *impl;
} FSI_DirData;

typedef struct FSI_EntryData
{
	const char *name;

	FSI_Type type;
} FSI_EntryData;

#define FSI_FREE(x) free(x); x = NULL;
#define FSI_DEL(x) delete x; x = nullptr;

void fsi_walk(CVEC *vec, const char *path);

__FSI_DIRUTILS_CPP_CLOSE

#endif  // INCLUDE_CORE_FSI_DIRUTILS_H_
