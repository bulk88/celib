// cecrt_defs.h - redefine funcs
//
// Time-stamp: <23/09/01 10:43:23 keuchel@netwave.de>

#ifndef CECRT_DEFS_H
#define CECRT_DEFS_H 1

#define strrchr xcestrrchr
#define _strrchr xcestrrchr
#define strdup xcestrdup
#define strndup xcestrndup
#define _strdup xcestrdup
#define strnicmp xcestrnicmp
#define _strnicmp xcestrnicmp
#define stricmp xcestricmp
#define _stricmp xcestricmp
#define strcmpi xcestrcmpi
#define _strcmpi xcestrcmpi
#define strlwr xcestrlwr
#define _strlwr xcestrlwr
#define strupr xcestrupr
#define _strupr xcestrupr
#define strcasecmp xcestrcasecmp
#define strncasecmp xcestrncasecmp
#define strtod xcestrtod
#define _strtod xcestrtod
#define strtol xcestrtol
#define _strtol xcestrtol
#define strtoul xcestrtoul
#define _strtoul xcestrtoul
#define strpbrk xcestrpbrk
#define strspn xcestrspn
#define strsep xcestrsep

#ifndef chdir
#define chdir xcechdir
#endif
#define _chdir xcechdir
#ifndef open
#define open xceopen
#endif
#define _open xceopen
#ifndef read
#define read xceread
#endif
#define _read xceread
#ifndef write
#define write xcewrite
#endif
#define _write xcewrite
#ifndef close
#define close xceclose
#endif
#define _close xceclose

#define stat xcestat
#define _stat xcestat
#define fstat xcefstat
#define _fstat xcefstat
#define lstat xcelstat

#ifndef lseek
#define lseek xcelseek
#endif
#define _lseek xcelseek
#ifndef creat
#define creat xcecreat
#endif
#define _creat xcecreat
#ifndef popen
#define popen xcepopen
#endif
#define _popen xcepopen
#ifndef pclose
#define pclose xcepclose
#endif
#define _pclose xcepclose
#define perror xceperror
#ifndef strerror
#define strerror xcestrerror
#define _strerror xcestrerror
#endif
#ifndef abort
#define abort xceabort
#define _abort xceabort
#endif
#define getcwd xcegetcwd
#define getenv xcegetenv
#define putenv xceputenv
#define _getenv xcegetenv
#define _putenv xceputenv
#define system xcesystem
#ifndef emacs
#ifndef isatty
#define isatty xceisatty
#endif
#endif
#define _isatty xceisatty
#define _locking xcelocking
#define locking xcelocking
#ifndef access
#define access xceaccess
#endif
#define _access xceaccess
#ifndef unlink
#define unlink xceunlink
#endif
#define _unlink xceunlink
#define _signal signal
#define alloca _alloca
#ifndef umask
#define umask xceumask
#endif
#define _umask xceumask
#ifndef mkdir
#define mkdir xcemkdir
#endif
#define _mkdir xcemkdir
#ifndef rmdir
#define rmdir xcermdir
#endif
#define _rmdir xcermdir
#ifndef rename
#define rename xcerename
#endif
#define _rename xcerename
#ifndef chmod
#define chmod xcechmod
#endif
#define _chmod xcechmod
#ifndef getpid
#define getpid xcegetpid
#endif
#define _getpid xcegetpid
#define tzset _tzset
#define tzname _tzname
#define snprintf xcesnprintf
#define ftruncate xceftruncate
#define get_osfhandle xceget_osfhandle
#define _get_osfhandle xceget_osfhandle

#ifndef PALM_SIZE
#define fopen xcefopen
#define freopen xcefreopen
#define fclose xcefclose
#define fdopen xcefdopen
#define fileno xcefileno
#define rewind xcerewind
#endif

#define setmode xcesetmode
#define _setmode xcesetmode

#define _ftime ftime
#define chroot xcechroot
#define mktemp xcemktemp

#ifndef perl
#define getservbyname xcegetservbyname
#define getservbyport xcegetservbyport
#define getprotobyname xcegetprotobyname
#define getprotobynumber xcegetprotobynumber
#endif

#define wsstart xcewsstart
#define wsstop xcewsstop
#define wserror xcewserror
#define wsstrerror xcewsstrerror
#define winstrerror xcewinstrerror
#define rcmd xcercmd
#define rexec xcerexec
#define exit xceexit

#define gmtime xcegmtime
#define localtime xcelocaltime
#define asctime xceasctime
#define utime xceutime
#define futime xcefutime
#define ftime xceftime
#define ctime xcectime
#define gettimeofday xcegettimeofday
#define time xcetime
#define mktime xcemktime

#define calloc xcecalloc
#define alarm xcealarm
#define ascii2addr xceascii2addr
#undef bcopy
#define bcopy xcebcopy
#ifndef perl
#undef bcmp
#define bcmp xcebcmp
#endif
#undef bzero
#define bzero xcebzero
#define index xceindex
#define rindex xcerindex

#define opendir xceopendir
#define closedir xceclosedir
#define readdir xcereaddir

#define chown xcechown

#define setpwent xcesetpwent
#define getpwent xcegetpwent
#define endpwent xceendpwent
#define getpwnam xcegetpwnam
#define getpwuid xcegetpwuid

#define getusershell xcegetusershell
#define endusershell xceendusershell
#define putpwent xceputpwent
#define getpass xcegetpass
#define getlogin xcegetlogin
#define sleep xcesleep

#define connectlog xceconnectlog
#define openlog xceopenlog
#define closelog xcecloselog
#define syslog xcesyslog
#define vsyslog xcesyslog

#define setuid xcesetuid
#define getuid xcegetuid
#define geteuid xcegeteuid
#define seteuid xceseteuid

#define setgid xcesetgid
#define getgid xcegetgid
#define getegid xcegetegid
#define setegid xcesetegid

#define _tzset _xcetzset

#define _beginthread _xcebeginthread
#define _beginthreadex _xcebeginthreadex
#define _endthread _xceendthread
#define _endthreadex _xceendthreadex

#define getopt xcegetopt

#define glob xceglob
#define globfree xceglobfree
#define signal xcesignal

#define localhost xcelocalhost

#define remove xceremove

#define setlocale xcesetlocale

#define wctomb xcewctomb
#define mbtowc xcembtowc
#define mblen xcemblen

#define atof xceatof

#define strcoll xcestrcoll
#define execvp xceexecvp
#define bsearch xcebsearch
#define tmpnam xcetmpnam

#define strftime xcestrftime
#define dup xcedup
#define dup2 xcedup2
#define pipe xcepipe
#define _pipe xcepipe

#define execl xceexecl
#define execlp xceexeclp
#define execv xceexecv
#define execvp xceexecvp

#define setenv xcesetenv

#define environ __xceenviron

#define clock xceclock
#define times xcetimes

#define fcntl xcefcntl
#define ioctl xceioctl

#define kill xcekill

#endif
