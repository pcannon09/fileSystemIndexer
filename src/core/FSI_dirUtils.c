#include "cstr/cstr.h"

#include "../../inc/fsi/FSIpredefines.h"
#include "../../inc/fsi/core/FSI_dirUtils.h"

#ifdef FSI_OS_WIN32
# 	include "../../inc/fsi/core/interfaces/FSI_dirUtils_windows.h"
#else
# 	include "../../inc/fsi/core/interfaces/FSI_dirUtils_posix.h"
#endif // __FSI_OS_WIN32

void fsi_walk(CVEC *out, const char *path)
{
    CVEC visited = cvec_init(-1, sizeof(FSI_Visited_POSIX*));

    __fsi_walk(out, &visited, path);

    cvec_destroy(&visited);
}

