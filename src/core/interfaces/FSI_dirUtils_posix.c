#define _DEFAULT_SOURCE

#include "../../../inc/fsi/FSIpredefines.h"

#include "../../../inc/fsi/core/interfaces/FSI_dirUtils_posix.h"

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int fsi_openDir(FSI_DirData *d, const char *path)
{
    FSI_DirData_POSIX *p = malloc(sizeof(*p));

    if (!p) return 0;

    p->path = cstr_init();

    {
    	const int dirFD = open(path, O_RDONLY);

    	p->dir = fdopendir(dirFD);
    }

    if (!p->dir)
    {
        cstr_destroy(&p->path);

        FSI_FREE(p);

        return 0;
    }

    cstr_set(&p->path, path);

    d->impl = p;

    return 1;
}

int fsi_readDir(FSI_DirData *d, FSI_EntryData *out)
{
    FSI_DirData_POSIX *p = d->impl;
    struct dirent *e;

    while ((e = readdir(p->dir)))
    {
        if (e->d_name[0] == '.' &&
           (!e->d_name[1] ||
           (e->d_name[1] == '.' && !e->d_name[2])))
            continue;

        if (e->d_type == DT_DIR)
            out->type = FSI_DIR;

        else if (e->d_type == DT_REG)
            out->type = FSI_FILE;

        else if (e->d_type == DT_LNK)
            out->type = FSI_SYMLINK;

        else out->type = FSI_OTHER;

        out->name = e->d_name;

        return 1;
    }

    return 0;
}

void fsi_closeDir(FSI_DirData *d)
{
	FSI_DirData_POSIX *p = d->impl;

	cstr_destroy(&p->path);

	closedir(p->dir);

	FSI_FREE(p);
}

