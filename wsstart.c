/* Name: wsstart.c --- startup winsock
 *
 * Time-stamp: <19/02/01 21:52:32 keuchel@w2k>
 */

#include <windows.h>
#include <winsock.h>
#include "celib.h"
#include "cesocket.h"

static WORD wVersionRequested;
static WSADATA wsaData;

int
xcewsstart()
{
#ifndef WIN311	
  wVersionRequested = MAKEWORD(2, 0);
#else
  wVersionRequested = 0x0101;
#endif  
  // returns zero on success
  return(WSAStartup(wVersionRequested, &wsaData));
}

void
xcewsstop()
{
  WSACleanup();
}
