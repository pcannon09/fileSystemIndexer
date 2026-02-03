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

#include "../FSI_dirUtils.h"

#include "cstr/cstr.h"

typedef struct FSI_DirData_POSIX
{
	DIR *dir;
	CSTR path;
} FSI_DirData_POSIX;

int fsi_openDir(FSI_DirData *d, const char *path);
int fsi_readDir(FSI_DirData *d, FSI_EntryData *out);

void fsi_closeDir(FSI_DirData *d);

__FSI_DIRUTILS_POSIX_CPP_CLOSE

#undef __FSI_DIRUTILS_POSIX_CPP_OPEN
#undef __FSI_DIRUTILS_POSIX_CPP_CLOSE

#endif  // INCLUDE_CORE_FSI_DIRUTILS_H_
