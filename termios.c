// termios.c
//
// Time-stamp: <23/09/01 09:05:13 keuchel@netwave.de>

#include "celib.h"
#include "termios.h"
#include "ioctl.h"

struct termios terminal_termios;

int
termios_init()
{
  terminal_termios.c_oflag = OPOST;
  terminal_termios.c_iflag = IGNPAR;
  terminal_termios.c_lflag = ECHO | ICANON;
  return xceioctl(0, TCSETA, &terminal_termios);
}

int
tcsetattr(int fd, int flags, struct termios *tio)
{
  memcpy(&terminal_termios, tio, sizeof(struct termios));
  return xceioctl(fd, TCSETA, tio);
}

int
tcgetattr(int fd, struct termios *tio)
{
  memcpy(tio, &terminal_termios, sizeof(struct termios));
  return xceioctl(fd, TCGETA, tio);
}

int
cfgetispeed(struct termios *t)
{
  // these are normally in c_cflag
  return B38400;
}

int
cfgetospeed(struct termios *t)
{
  return B38400;
}

int
cfsetispeed(struct termios *t, int speed)
{
  return 0;
}

int
cfsetospeed(struct termios *t, int speed)
{
  return 0;
}
