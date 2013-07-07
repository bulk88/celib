// ascii2addr.c
//
// Time-stamp: <12/02/01 16:56:51 keuchel@keuchelnt>

#include <windows.h>
#include <winsock.h>
#include "celib.h"

int
xceascii2addr(int af, const char *ascii, void *result)
{
  struct in_addr *ina;
  char strbuf[4*sizeof("123")]; /* long enough for V4 only */

  switch(af) 
    {
    case AF_INET:
      ina = (struct in_addr *) result;
      strbuf[0] = '\0';
      strncat(strbuf, ascii, (sizeof strbuf)-1);
      if (xceinet_aton(strbuf, ina))
        return sizeof(struct in_addr);
      WSASetLastError(WSAEINVAL);
      break;
    default:
      WSASetLastError(WSAEPROTONOSUPPORT);
      break;
    }

  return -1;
}
