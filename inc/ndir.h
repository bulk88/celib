#ifndef NDIR_H
#define NDIR_H 1

#include "celib_defs.h"

#define DIRBLKSIZ	512		/* size of directory block */
#define MAXNAMLEN	(512-1)

/* NOTE:  MAXNAMLEN must be one less than a multiple of 4 */

struct direct		    /* data from readdir() */
{
  long	d_ino;		    /* inode number of entry */
  unsigned short d_reclen;  /* length of this record */
  unsigned short d_namlen;  /* length of string in d_name */
  char d_name[MAXNAMLEN+1]; /* name of file */
};

typedef struct
{
  int	dd_fd;			/* file descriptor */
  int	dd_loc;			/* offset in block */
  int	dd_size;		/* amount of valid data */
  char	dd_buf[DIRBLKSIZ];	/* directory block */
  void  *dd_handle;		// Find handle
  int   dd_isfat;		// Is it FAT?
  char  dd_path[256];		// dir path
} DIR;			        /* stream data from opendir() */

#ifdef __cplusplus
extern "C" {
#endif

XCE_EXPORT DIR *xceopendir(char *fname);
XCE_EXPORT void xceclosedir(DIR *dirp);
XCE_EXPORT struct direct *xcereaddir (DIR *dirp);

#ifdef __cplusplus
};
#endif

#endif
