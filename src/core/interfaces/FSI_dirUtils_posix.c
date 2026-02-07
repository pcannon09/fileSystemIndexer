#include "../../../inc/fsi/FSIpredefines.h"

#ifndef FSI_OS_WIN32

#include "../../../inc/fsi/core/interfaces/FSI_dirUtils_posix.h"

#include <stdlib.h>

int fsi_openDir(FSI_DirData *d, const char *path)
{
	const int dirFD = open(path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);

	if (dirFD < 0) return 0;

	DIR *dir = fdopendir(dirFD);

	if (!dir)
	{
		close(dirFD);

		return 0;
	}

	FSI_DirData_POSIX *p = malloc(sizeof(*p));

	if (!p)
	{
		closedir(dir);

		return 0;
	}

	p->dir = dir;
	p->dirfd = dirFD;

	d->impl = p;

	return 1;
}

void fsi_closeDir(FSI_DirData *d)
{
	FSI_DirData_POSIX *p = d->impl;

	cstr_destroy(&p->path);

	// Closes FD variable too
	closedir(p->dir);

	FSI_FREE(p);
}

#endif // FSI_OS_WIN32

