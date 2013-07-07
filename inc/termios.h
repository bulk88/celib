#ifndef TERMIOS_H
#define TERMIOS_H 1

#include "celib_defs.h"
#include "termbits.h"

// ioctls
#define TCGETS		0x5401
#define TCSETS		0x5402
#define TCSETSW		0x5403
#define TCSETSF		0x5404
#define TCGETA		0x5405
#define TCSETA		0x5406
#define TCSETAW		0x5407
#define TCSETAF		0x5408
#define TCSBRK		0x5409
#define TCXONC		0x540A
#define TCFLSH		0x540B

#define TIOCGWINSZ	0x5413
#define TIOCSWINSZ	0x5414

XCE_BEGIN_DECLS

typedef int speed_t;

struct winsize {
  unsigned short ws_row;
  unsigned short ws_col;
  unsigned short ws_xpixel;
  unsigned short ws_ypixel;
};

#define NCC 8

struct termios {
  unsigned short c_iflag; /* input mode flags */
  unsigned short c_oflag; /* output mode flags */
  unsigned short c_cflag; /* control mode flags */
  unsigned short c_lflag; /* local mode flags */
  unsigned char c_line;   /* line discipline */
  unsigned char c_cc[NCC]; /* control characters */
};

XCE_EXPORT int termios_init();
XCE_EXPORT int tcsetattr(int fd, int flags, struct termios *t);
XCE_EXPORT int tcgetattr(int fd, struct termios *t);
XCE_EXPORT int cfgetispeed(struct termios *t);
XCE_EXPORT int cfgetospeed(struct termios *t);
XCE_EXPORT int cfsetispeed(struct termios *t, int speed);
XCE_EXPORT int cfsetospeed(struct termios *t, int speed);

XCE_END_DECLS

#endif
