// inet_aton.c - from BSD
//
// Time-stamp: <13/05/01 11:01:55 keuchel@w2k>

#include <windows.h>
#include <winsock.h>
#include "celib.h"

int
xceinet_aton(const char *cp, struct in_addr *addr)
{
  register u_long val;
  register int base, n;
  register char c;
  u_int parts[4];
  register u_int *pp = parts;

  c = *cp;
  for (;;) {
    /*
     * Collect number up to ``.''.
     * Values are specified as for C:
     * 0x=hex, 0=octal, isdigit=decimal.
     */
    if (!isdigit(c))
      return (0);
    val = 0; base = 10;
    if (c == '0') {
      c = *++cp;
      if (c == 'x' || c == 'X')
        base = 16, c = *++cp;
      else
        base = 8;
    }
    for (;;) {
      if (isdigit(c)) {
        val = (val * base) + (c - '0');
        c = *++cp;
      } else
        break;
    }
    if (c == '.') {
      /*
       * Internet format:
       *	a.b.c.d
       *	a.b.c	(with c treated as 16 bits)
       *	a.b	(with b treated as 24 bits)
       */
      if (pp >= parts + 3)
        return (0);
      *pp++ = val;
      c = *++cp;
    } else
      break;
  }
  /*
   * Check for trailing characters.
   */
  if (c != '\0' && (!isspace(c)))
    return (0);
  /*
   * Concoct the address according to
   * the number of parts specified.
   */
  n = pp - parts + 1;
  switch (n) {

  case 0:
    return (0);		/* initial nondigit */

  case 1:				/* a -- 32 bits */
    break;

  case 2:				/* a.b -- 8.24 bits */
    if (val > 0xffffff)
      return (0);
    val |= parts[0] << 24;
    break;

  case 3:				/* a.b.c -- 8.8.16 bits */
    if (val > 0xffff)
      return (0);
    val |= (parts[0] << 24) | (parts[1] << 16);
    break;

  case 4:				/* a.b.c.d -- 8.8.8.8 bits */
    if (val > 0xff)
      return (0);
    val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
    break;
  }
  if (addr)
    addr->s_addr = htonl(val);
  return (1);
}
