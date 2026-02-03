#include "cstr/cstr.h"

#include "../../inc/fsi/FSIpredefines.h"
#include "../../inc/fsi/core/FSI_dirUtils.h"

#ifdef __FSI_OS_WIN32
# 	include "../../inc/fsi/core/interfaces/FSI_dirUtils_windows.h"
#else
# 	include "../../inc/fsi/core/interfaces/FSI_dirUtils_posix.h"
#endif // __FSI_OS_WIN32

void fsi_walk(CVEC *vec, const char *path)
{
    FSI_DirData dir;
    FSI_EntryData entry;

    if (!fsi_openDir(&dir, path))
        return;

    CSTR tmpPath = cstr_init();

    while (fsi_readDir(&dir, &entry))
    {
        cstr_set(&tmpPath, path);
        cstr_add(&tmpPath, "/");
        cstr_add(&tmpPath, entry.name);

        cvec_push(vec, const char*, CVEC_sys_strdup(tmpPath.data));

        if (entry.type == FSI_DIR)
            fsi_walk(vec, tmpPath.data);
    }

    cstr_destroy(&tmpPath);

    fsi_closeDir(&dir);
}

