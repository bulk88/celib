// ceirda.c
//
// Time-stamp: <03/01/02 22:16:02 keuchel@netwave.de>

#if UNDER_CE > 201

#include <af_irda.h>
#include "ceirda.h"

int
xceirda_getaddr(const char *hostname, const char *servicename,
		SOCKADDR_IRDA *addr)
{
  int len;
  int retry = 0;
  SOCKET s;
  int i;
  DEVICELIST *pirda_devlist = NULL;
  int status;

  len = 1024;
  pirda_devlist = malloc(len);
  pirda_devlist->numDevice = 0;

  if((s = socket(AF_IRDA, SOCK_STREAM, 0)) == SOCKET_ERROR)
    return -1;

  while(retry++ < 5)
    {
      len = 1024;

      if(getsockopt(s, SOL_IRLMP, IRLMP_ENUMDEVICES,
		    (char *) pirda_devlist, &len) == SOCKET_ERROR)
	{
	  goto bad;
	}

      if(pirda_devlist->numDevice != 0)
	{
	  break;
	}

      Sleep(200);
    }

  if(pirda_devlist->numDevice == 0)
    {
      goto bad;
    }

  for(i = 0; i < pirda_devlist->numDevice; i++)
    {
      if(xcestricmp(pirda_devlist->Device[i].irdaDeviceName, hostname) == 0)
	{
	  break;
	}
    }

  if(i == pirda_devlist->numDevice)
    {
      goto bad;
    }

  addr->irdaAddressFamily = AF_IRDA;
  memcpy(addr->irdaDeviceID, 
	 pirda_devlist->Device[i].irdaDeviceID, 4);
  strcpy(addr->irdaServiceName, servicename);

  status = 0;
  goto done;

 bad:
  status = -1;

 done:
  free(pirda_devlist);
  closesocket(s);

  return status;
}

#endif
