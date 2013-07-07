// cedir.c
//
// Time-stamp: <27/08/01 12:50:30 keuchel@netwave.de>

#include <windows.h>
#include <errno.h>
#include <sys/stat.h>
#include <malloc.h>
#include "ndir.h"

#define MAXPATHLEN 255

static struct direct dir_static;

#define IS_DIRECTORY_SEP(x) (x == '\\' || x == '/')

DIR *
xceopendir (char *fname)
{
  DIR *dirp;
  struct xcestat statbuf;
  char filename[MAXPATHLEN];
  char *p, *d;

  XCETrace("opendir(%s)", fname);

  if(strcmp(fname, ".") == 0)
    XCEGetCurrentDirectoryA(sizeof(filename), filename);
  else
    XCEFixPathA(fname, filename);

  XCETrace("opendir2(%s)", filename);

  if(xcestat(filename, &statbuf) < 0)
    return NULL;

  if(!(statbuf.st_mode & S_IFDIR))
    return NULL;

  if (!(dirp = (DIR *) malloc (sizeof (DIR))))
    {
      errno = ENOMEM;
      return NULL;
    }

  dirp->dd_handle = INVALID_HANDLE_VALUE;
  dirp->dd_isfat = 1;
  dirp->dd_fd = 0;
  dirp->dd_loc = 0;
  dirp->dd_size = 0;

  strcpy(dirp->dd_path, filename);

  return dirp;
}

void
xceclosedir (DIR *dirp)
{
  if (dirp->dd_handle != INVALID_HANDLE_VALUE)
    {
      FindClose ((HANDLE)dirp->dd_handle);
    }
  free ((char *) dirp);
}

struct direct *
xcereaddir (DIR *dirp)
{
  WIN32_FIND_DATA find_data;

#ifdef UNDER_CE
  if(dirp->dd_loc == 0)
    {
      strcpy(find_data.cFileName, ".");
    }
  else if(dirp->dd_loc == 1)
    {
      strcpy(find_data.cFileName, "..");
    }
  else
#endif
  if (dirp->dd_handle == INVALID_HANDLE_VALUE)
    {
      char filename[MAXNAMLEN + 3];
      int ln;

      XCEFixPathA(dirp->dd_path, filename);

      ln = strlen (filename) - 1;
      if (!IS_DIRECTORY_SEP (filename[ln]))
	strcat (filename, "\\");
      strcat (filename, "*");

      // printf("FindFirstFile: %s\n", filename);

      dirp->dd_handle = (HANDLE)XCEFindFirstFileA (filename, &find_data);

      if (dirp->dd_handle == INVALID_HANDLE_VALUE)
	return NULL;
    }
  else
    {
      if (!XCEFindNextFileA ((HANDLE)dirp->dd_handle, &find_data))
	return NULL;
    }
  
  dir_static.d_ino = 1;
#ifdef UNDER_CE
  dirp->dd_loc++;
#endif

  dir_static.d_reclen = sizeof (struct direct) - MAXNAMLEN + 3 +
    dir_static.d_namlen - dir_static.d_namlen % 4;
  
  dir_static.d_namlen = strlen (find_data.cFileName);
  strcpy (dir_static.d_name, find_data.cFileName);

  return &dir_static;
}
