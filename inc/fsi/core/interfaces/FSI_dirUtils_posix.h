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
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct FSI_DirData_POSIX
{
	DIR *dir;
	CSTR path;

	int dirfd;
} FSI_DirData_POSIX;

typedef struct FSI_Visited_POSIX
{
	dev_t dev;
	ino_t ino;
} FSI_Visited_POSIX;

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

/**
 * @brief Get the entry type of the dirfd from the FSI_DirData_POSIX
 * * Get if: Directory, File, Symlink or other
 * @param dirfd Entry type to get from the syscall
 * @param e Get dirent type quickly from the `e` ptr with its corresponding `dirent` data
 * @return Return the FSI_EntryType
 */
static inline FSI_EntryType fsi_getEntryType(int dirfd, struct dirent *e)
{
	switch (e->d_type)
	{
		case DT_REG: return FSI_FILE;
		case DT_DIR: return FSI_DIR;
		case DT_LNK: return FSI_SYMLINK;
		case DT_UNKNOWN:
		default:
		{
			struct stat st;

			if (fstatat(dirfd, e->d_name, &st, AT_SYMLINK_NOFOLLOW) == 0)
			{
				if (S_ISREG(st.st_mode)) return FSI_FILE;
				if (S_ISDIR(st.st_mode)) return FSI_DIR;
				if (S_ISDIR(st.st_mode)) return FSI_SYMLINK;
			}

			return FSI_OTHER;
		}
	}
}

/**
 * @brief Get the entry type of the dirfd from the FSI_DirData_POSIX
 * * Get if: Directory, File, Symlink or other
 * * Always using the dirent method; will go faster
 * @param dirfd The FD directory from the opened database
 * @param e Dirent entry to get information from the dirent
 * @param stOut Output Status where the status will be set
 * @return Return the FSI_EntryType
 */
static inline FSI_EntryType fsi_getEntryTypeFromDirent(int dirfd, struct dirent *e, struct stat *stOut)
{
	switch (e->d_type)
	{
		case DT_REG:
			return FSI_FILE;

		case DT_DIR:
			if (fstatat(dirfd, e->d_name, stOut, AT_SYMLINK_NOFOLLOW) == 0)
				return FSI_DIR;

			return FSI_OTHER;

		case DT_LNK: return FSI_SYMLINK;

		default:
			if (fstatat(dirfd, e->d_name, stOut, AT_SYMLINK_NOFOLLOW) == 0)
			{
				if (S_ISDIR(stOut->st_mode)) return FSI_DIR;
				if (S_ISREG(stOut->st_mode)) return FSI_FILE;
			}

			return FSI_OTHER;
	}
}

/**
 * @brief Check if `dev` and `ino` are tracked, return the corresponding number of tracked (1) or not (0)
 * @param dev Device number; in this case for visited directory (or file) to get if tracked
 * @param ino Serial number to get if tracked
 * @return Tracked result; 1 if tracked, else, 0
 */
static inline int fsi_visitedHas(CVEC *v, dev_t dev, ino_t ino)
{
	for (size_t i = 0 ; i < v->size ; i++)
	{
		FSI_Visited_POSIX e = *(FSI_Visited_POSIX*)cvec_get(v, i);
		
		if (e.dev == dev && e.ino == ino)
			return 1;
	}

	return 0;
}

/**
 * @brief Add visited for the dev and ino files; This can be used to check correctly for no recursions
 * @param v Dynamic vector to allocate the visited data from `FSI_Visited_POSIX` type
 * @param dev Device number; in this case for visited directory (or file)
 * @param ino Serial number to track
 */
static inline void fsi_visitedAdd(CVEC *v, dev_t dev, ino_t ino)
{
	FSI_Visited_POSIX e = {0};

	e.dev = dev;
	e.ino = ino;

	cvec_push(v, FSI_Visited_POSIX, e);
}

/**
 * @brief Internal method helper to walk a directory fast
 * @param out Output where all the directories (and files) will be added
 * @param visited Vector track if visited or not
 * @param path Path to start walking from
 */
static inline void __fsi_walk(
    CVEC *out,
    CVEC *visited,

    const char *path
)
{
    int fd = open(path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd < 0) return;

    DIR *dir = fdopendir(fd);

    if (!dir)
    {
    	close(fd);
    	return;
    }

    struct stat dirSt;

    if (fstat(fd, &dirSt) != 0)
    {
    	closedir(dir);
    	return;
    }

    if (fsi_visitedHas(visited, dirSt.st_dev, dirSt.st_ino))
    {
        closedir(dir);

        return;
    }

    fsi_visitedAdd(visited, dirSt.st_dev, dirSt.st_ino);

    struct dirent *e;
    CSTR tmp = cstr_init();

    while ((e = readdir(dir)))
    {
        if (e->d_name[0] == '.' &&
            (e->d_name[1] == '\0' || (e->d_name[1] == '.' && e->d_name[2] == '\0')))
            continue;

        struct stat st;

        int type = fsi_getEntryTypeFromDirent(fd, e, &st);

        cstr_set(&tmp, path);
        cstr_add(&tmp, "/");
        cstr_add(&tmp, e->d_name);

        cvec_push(out, const char*, CVEC_sys_strdup(tmp.data));

        if (type == FSI_DIR)
            __fsi_walk(out, visited, tmp.data);
    }

    cstr_destroy(&tmp);

    closedir(dir);
}

#endif // FSI_OS_WIN32

__FSI_DIRUTILS_POSIX_CPP_CLOSE

#undef __FSI_DIRUTILS_POSIX_CPP_OPEN
#undef __FSI_DIRUTILS_POSIX_CPP_CLOSE

#endif  // INCLUDE_CORE_FSI_DIRUTILS_H_
