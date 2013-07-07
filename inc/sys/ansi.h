#ifndef SYS_ANSI_H
#define SYS_ANSI_H 1

// bsd ansi.h

typedef union {
  char __mbstate8[128];
  __int64 __mbstateL;	/* for alignment */
} __mbstate_t;
  
#define	_BSD_MBSTATE_T_	__mbstate_t /* mbstate_t */
  
#endif
