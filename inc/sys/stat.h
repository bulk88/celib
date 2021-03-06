#ifndef SYS_STAT_H
#define SYS_STAT_H 1

#include <celib.h>
#include <sys/types.h>

#define _S_IFMT         0170000         /* file type mask */
#define _S_IFDIR        0040000         /* directory */
#define _S_IFCHR        0020000         /* character special */
#define _S_IFIFO        0010000         /* pipe */
#define _S_IFREG        0100000         /* regular */
#define _S_IREAD        0000400         /* read permission, owner */
#define _S_IWRITE       0000200         /* write permission, owner */
#define _S_IEXEC        0000100         /* execute/search permission, owner */

#define S_IFMT   _S_IFMT
#define S_IFREG  _S_IFREG
#define S_IFCHR  _S_IFCHR
#define S_IFDIR  _S_IFDIR
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

#ifndef S_ISDIR
#define S_ISDIR(X) (((X) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(X) (((X) & S_IFMT) == S_IFREG)
#endif

XCE_BEGIN_DECLS

struct xcestat 
{
  dev_t st_dev;
  ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  dev_t st_rdev;
  off_t st_size;
  time_t st_atime;
  time_t st_mtime;
  time_t st_ctime;
};

XCE_EXPORT int xcestat(const char *f, struct xcestat *st);
XCE_EXPORT int xcelstat(const char *fname, struct xcestat *st);

XCE_END_DECLS

#endif
