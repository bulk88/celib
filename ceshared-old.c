// ceshared.c - obsolete, shared dll sections dont work in wince211
//
// Time-stamp: <19/12/01 16:42:21 keuchel@netwave.de>

#include <windows.h>
#include "celib.h"
#include "ceshared.h"

/* write back & discard all cached data */
#define CACHE_SYNC_DISCARD      0x001 
/* discard all cached instructions */
#define CACHE_SYNC_INSTRUCTIONS 0x002   
/* write back but don't discard data cache*/
#define CACHE_SYNC_WRITEBACK	0x004

DWORD CacheSync(DWORD);

#ifndef _WIN32_WCE_EMULATION
#define SYNC CacheSync(CACHE_SYNC_DISCARD|CACHE_SYNC_INSTRUCTIONS)
#else
#define SYNC
#endif

// Synchronizing with events sometimes fails!
// I get an event, but avail count is zero!
// Seems that the compiler does not store it
// or there are problems with cache...
//
// CacheSync() seems to fix the problem...

wchar_t *mutex_name = L"celib_shared_mutex";
HANDLE hmutex;

struct procstart_entry xcelocal_procstart;

// The mips compilers dont generate a bss section, only data... You can
// ignore the linker warning
#pragma bss_seg("shared_bss")
#pragma data_seg("shared_data")

int xceshared_initialized;

struct procstart_entry xceshared_procstart;

struct pipe_entry xceshared_pipetab[MAXPIPES];

void
xceshared_init()
{
  if(xceshared_initialized == 0)
    {
      xceshared_procstart.showwindow = TRUE;
      xceshared_initialized = TRUE;
    }
}

void
xceshared_protect()
{
  MEMORY_BASIC_INFORMATION mi;
  DWORD dwOldProt;

  VirtualQuery(&xceshared_procstart, &mi, sizeof(mi));

  XCETrace("Changing region at %x, size %x prot %x",
	   mi.BaseAddress, mi.RegionSize, mi.Protect);

  // PAGE_READWRITE = 4, size 0x2000
  // Does not work and no error!
  if(VirtualProtect(mi.BaseAddress, mi.RegionSize, PAGE_READWRITE|PAGE_NOCACHE,
		    &dwOldProt) == FALSE)
    {
      XCETrace("VirtualProtect: %d", GetLastError());
    }

  XCETrace("Changed region at %x, size %x now prot %x",
	   mi.BaseAddress, mi.RegionSize, mi.Protect);
}

void
xceshared_reset()
{
  //xceshared_protect();

  memset(&xceshared_procstart, 0, sizeof(struct procstart_entry));
  xceshared_procstart.showwindow = TRUE;
}

void
xceshared_setshowwindow(BOOL bShow)
{
  XCETrace("setshowwindow %d", bShow);
  xceshared_procstart.showwindow = bShow;
}

void
xceshared_setcwd(char *cwd)
{
  strcpy(xceshared_procstart.cwd, cwd);
}

void
xceshared_setenvironblock(char *env)
{
  char *s;
  char *d;
  int len;
  char *endp;

  endp = xceshared_procstart.environ + MAX_ENVIRONBLOCKSIZE;

  for(s = env, d = xceshared_procstart.environ; *s;)
    {
      len = strlen(s);
      if(d + len >= endp)
	{
	  XCEShowMessageA("Out of space in environment (max %d)",
			  MAX_ENVIRONBLOCKSIZE);
	  exit(1);
	}
      memcpy(d, s, len + 1);
      s += len + 1;
      d += len + 1;
    }
  *d = 0;
}

BOOL
xceshared_setstdhandle(DWORD dwId, HANDLE hFile)
{
  XCETrace("xceshared_setstdhandle(%d, %x)", dwId, hFile);

  if(hFile == INVALID_HANDLE_VALUE || hFile == NULL)
    {
    }
  else
    {
      if(!ISPIPEHANDLE(hFile))
	{
	  XCETrace("Handle is not a pipe");
	  return FALSE;
	}

      if(!ISINHERIT(hFile))
	{
	  XCETrace("Handle is not inheritable");
	  return TRUE;
	}
    }

  if(dwId == STD_INPUT_HANDLE)
    {
      xceshared_procstart.hstdin = hFile;
    }
  else if(dwId == STD_OUTPUT_HANDLE)
    {
      xceshared_procstart.hstdout = hFile;
    }
  if(dwId == STD_ERROR_HANDLE)
    {
      xceshared_procstart.hstderr = hFile;
    }

  return TRUE;
}

//////////////////////////////////////////////////////////////////////

BOOL
xceshared_getshowwindow()
{
  return xceshared_procstart.showwindow;
}

void
xceshared_getcwd(char *cwd)
{
  strcpy(cwd, xceshared_procstart.cwd);
}

void
xceshared_getenvironblock(char *buf)
{
  char *s;
  char *d;
  int len;

  for(s = xceshared_procstart.environ, d = buf; *s;)
    {
      len = strlen(s);
      memcpy(d, s, len + 1);
      s += len + 1;
      d += len + 1;
    }
  *d = 0;
}

HANDLE
xceshared_getstdhandle(DWORD dwId)
{
  if(dwId == STD_INPUT_HANDLE)
    {
      return xceshared_procstart.hstdin;
    }
  else if(dwId == STD_OUTPUT_HANDLE)
    {
      return xceshared_procstart.hstdout;
    }
  if(dwId == STD_ERROR_HANDLE)
    {
      return xceshared_procstart.hstderr;
    }

  return INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////

BOOL
xceshared_lock()
{
  if((hmutex = CreateMutexW(NULL, FALSE, mutex_name)) == NULL)
    {
      return FALSE;
    }
  WaitForSingleObject(hmutex, INFINITE);
  return TRUE;
}

BOOL
xceshared_release()
{
  return ReleaseMutex(hmutex);
}

//////////////////////////////////////////////////////////////////////
// Needed because of cache problems? Did not fix it...

void
pipe_setavail(struct pipe_entry *pp, int avail)
{
  pp->avail = avail;
}

int
pipe_getavail(struct pipe_entry *pp)
{
  return pp->avail;
}

// we assume that less that bufsize bytes are written...

int
pipe_write(HANDLE hPipe, unsigned char *buf, int size)
{
  HANDLE hevent_canread, hevent_canwrite;
  struct pipe_entry *pp;
  int written = 0;
  int pipeindex;
  int len = 0;

  XCETrace("pipe_write(%x, %x, %d)", hPipe, buf, size);

  pipeindex = DECODEPIPEHANDLE(hPipe);
  pp = &xceshared_pipetab[pipeindex];

  hevent_canread = 
    XCECreateEventA(NULL, FALSE, FALSE, pp->event_name_canread);
  hevent_canwrite = 
    XCECreateEventA(NULL, FALSE, FALSE, pp->event_name_canwrite);

  if(pp->used == 0 || pp->opencntwrite == 0)
    {
      XCETrace("pipe_write on closed write handle");
      errno = EIO;
      return -1;
    }

  if(pp->opencntread == 0)
    {
      XCETrace("pipe_write: no reader handle open");
      errno = EPIPE;
      return -1;
    }

  // Wait until reader consumed data...
  
  // Seems that there can be deadlocks where 2 processes
  // wait for the reader of the other... Ange-ftp writes
  // to ftp without reading output, and ftp writes before
  // reading input...
  
  // So its a bad idea to wait for the reader as long as the buffer
  // is not full... This has to be fixed...

  SYNC;
  
  if(pipe_getavail(pp) > 0)
    {
      XCETrace("pipe_write: not empty, waiting for pipe reader (%d/%d)",
	       pp->opencntread, pp->opencntwrite);

      while(1)
	{
	  DWORD dwWait;
	  
	  dwWait = WaitForSingleObject(hevent_canwrite, 100);

	  SYNC;

	  if(dwWait == WAIT_OBJECT_0 && pipe_getavail(pp) == 0)
	    break;

	  if(dwWait != WAIT_TIMEOUT)
	    {
	      XCETrace("error: pipe_write: dwWait = %d, avail = %d", 
		       dwWait, pipe_getavail(pp));
	      errno = EIO;
	      return -1;
	    }

	  if(pp->opencntread == 0)
	    {
	      XCETrace("pipe_write: reader has closed pipe");
	      errno = EPIPE;
	      return -1;
	    }
	}
    }

  len = min(size, pp->size);

  memcpy(pp->buf, buf, len);
  pipe_setavail(pp, len);

  XCETrace("%d bytes written to pipe, signalling reader", 
	   pipe_getavail(pp));

  SYNC;

  SetEvent(hevent_canread);

  size -= len;
  written += len;

  CloseHandle(hevent_canread);
  CloseHandle(hevent_canwrite);

  return written;
}

int
pipe_read(HANDLE hPipe, unsigned char *buf, int size)
{
  HANDLE hevent_canread, hevent_canwrite;
  struct pipe_entry *pp;
  int read = 0;
  int len;
  int pipeindex;

  XCETrace("pipe_read(%x, %x, %d)", hPipe, buf, size);

  pipeindex = DECODEPIPEHANDLE(hPipe);
  pp = &xceshared_pipetab[pipeindex];

  hevent_canread = 
    XCECreateEventA(NULL, FALSE, FALSE, pp->event_name_canread);
  hevent_canwrite = 
    XCECreateEventA(NULL, FALSE, FALSE, pp->event_name_canwrite);

  if(pp->used == 0 || pp->opencntread == 0)
    {
      XCETrace("pipe_read on closed read handle");
      return -1;
    }

  XCETrace("pipe_read: waiting for pipe writer (%d/%d)",
	   pp->opencntread, pp->opencntwrite);

  while(1)
    {
      DWORD dwWait;

      dwWait = WaitForSingleObject(hevent_canread, 100);

      SYNC;

      if(dwWait == WAIT_OBJECT_0 && pipe_getavail(pp) != 0)
	break;

      if(dwWait == WAIT_OBJECT_0 && pipe_getavail(pp) == 0)
	{
	  XCETrace("got event but no data ready");
	  errno = EIO;
	  return -1;
	}
      else if(dwWait != WAIT_TIMEOUT)
	{
	  XCETrace("pipe_read: wait failed: dwWait = %d avail = %d err = %d", 
		   dwWait, pipe_getavail(pp), GetLastError());
	  //DebugBreak();
	  errno = EIO;
	  return -1;
	}

      if(pp->opencntwrite == 0 && pipe_getavail(pp) == 0)
	{
	  XCETrace("writer has closed pipe");
	  //errno = EPIPE;
	  return 0;
	}
    }

  len = min(pipe_getavail(pp), size);
  memcpy(buf, pp->readp, len);
  pipe_setavail(pp, pipe_getavail(pp) - len);
  pp->readp += len;
  read += len;

  // signal writer to write more...
  if(pipe_getavail(pp) == 0)
    {
      pp->readp = pp->buf;

      SetEvent(hevent_canwrite);
    }
  else
    {
      // signal ourselves that we can read...
      SetEvent(hevent_canread);
    }

  SYNC;

  XCETrace("read_pipe returns %d", read);

  CloseHandle(hevent_canread);
  CloseHandle(hevent_canwrite);

  return read;
}

//////////////////////////////////////////////////////////////////////

BOOL
XCECreatePipe(PHANDLE hRead, PHANDLE hWrite, LPSECURITY_ATTRIBUTES lpsa,
	     DWORD size)
{
  int i;
  int pipeindex = -1;
  HANDLE hEvent;

  XCETrace("CreatePipe()");

  xceshared_lock();

  for(i = 0; i < MAXPIPES; i++)
    {
      if(xceshared_pipetab[i].used == 0)
	{
	  pipeindex = i;
	  break;
	}
    }

  if(pipeindex == -1)
    {
      xceshared_release();
      errno = ENOMEM;
      return FALSE;
    }

  memset(&xceshared_pipetab[pipeindex], 0, sizeof(struct pipe_entry));
  xceshared_pipetab[pipeindex].used = 1;
  xceshared_pipetab[pipeindex].hread = MAKEPIPEREADHANDLE(pipeindex);
  xceshared_pipetab[pipeindex].hwrite = MAKEPIPEWRITEHANDLE(pipeindex);
  xceshared_pipetab[pipeindex].size = PIPEBUFSIZE;
  xceshared_pipetab[pipeindex].readp = xceshared_pipetab[pipeindex].buf;
  //xceshared_pipetab[pipeindex].writep = xceshared_pipetab[pipeindex].buf;
  xceshared_pipetab[pipeindex].avail = 0;
  xceshared_pipetab[pipeindex].opencntread = 1;
  xceshared_pipetab[pipeindex].opencntwrite = 1;

  sprintf(xceshared_pipetab[pipeindex].event_name_canread, 
	  "PIPE_READ_EVENT%d", pipeindex);
  sprintf(xceshared_pipetab[pipeindex].event_name_canwrite, 
	  "PIPE_WRITE_EVENT%d", pipeindex);

  // TODO: These handles are never closed. They are needed
  // because read/write currently open/close handles. When
  // no handle is open, the event can get lost...

  hEvent = 
    XCECreateEventA(NULL, FALSE, FALSE, 
		    xceshared_pipetab[pipeindex].event_name_canread);

  // There is currently a leak due to incorrect reference counting,
  // so we must clear all events on these handles... Else the next
  // write gets an EIO...
  WaitForSingleObject(hEvent, 0);

  hEvent = 
    XCECreateEventA(NULL, FALSE, FALSE, 
		    xceshared_pipetab[pipeindex].event_name_canwrite);
  WaitForSingleObject(hEvent, 0);

  *hRead = xceshared_pipetab[pipeindex].hread;
  *hWrite = xceshared_pipetab[pipeindex].hwrite;

  XCETrace("CreatePipe(): hRead = %x, hWrite = %x",
	   *hRead, *hWrite);

  xceshared_release();

  return TRUE;
}

BOOL
XCEReadFile(HANDLE hFile, LPVOID buf, DWORD dwSize, LPDWORD lpdwRead,
	    LPOVERLAPPED lpOverlapped)
{
  int res = TRUE;

  if(ISCONSREADHANDLE(hFile) && console_read_func)
    {
      int fd = 0;

      if((*lpdwRead = (*console_read_func)(fd, buf, dwSize)) < 0)
	{
	  SetLastError(ERROR_BROKEN_PIPE);

	  *lpdwRead = 0;

	  return FALSE;
	}
      return TRUE;
    }
  else if(ISPIPEHANDLE(hFile))
    {
      if((*lpdwRead = pipe_read(hFile, buf, dwSize)) < 0)
	{
	  SetLastError(ERROR_BROKEN_PIPE);

	  *lpdwRead = 0;

	  return FALSE;
	}
      return TRUE;
    }

  res = ReadFile(hFile, buf, dwSize, lpdwRead, lpOverlapped);
  
  return res;
}

BOOL
XCEWriteFile(HANDLE hFile, LPVOID buf, DWORD dwSize, LPDWORD lpdwWritten,
	    LPOVERLAPPED lpOverlapped)
{
  int res = TRUE;

  if(ISCONSWRITEHANDLE(hFile) && console_write_func)
    {
      int fd = DECODECONSHANDLE(hFile);

      if((*lpdwWritten = (*console_write_func)(fd, buf, dwSize)) < 0)
	{
	  *lpdwWritten = 0;
	  return FALSE;
	}
      return TRUE;
    }
  else if(ISPIPEHANDLE(hFile))
    {
      if((*lpdwWritten = pipe_write(hFile, buf, dwSize)) < 0)
	{
	  *lpdwWritten = 0;
	  return FALSE;
	}
      return TRUE;
    }

  res = WriteFile(hFile, buf, dwSize, lpdwWritten, lpOverlapped);
  
  return res;
}

BOOL
close_pipe(HANDLE handle)
{
  int pipeindex;

  pipeindex = DECODEPIPEHANDLE(handle);

  XCETrace("close_pipe: %d/%d",
	   xceshared_pipetab[pipeindex].opencntread,
	   xceshared_pipetab[pipeindex].opencntwrite);

  if(xceshared_pipetab[pipeindex].used == 0)
    {
      xceshared_release();
      XCETrace("WARNING: Closing pipe handle that is not in use");
      return FALSE;
    }

  if(ISPIPEREADHANDLE(handle))
    {
      if(xceshared_pipetab[pipeindex].opencntread > 0)
	{
	  xceshared_pipetab[pipeindex].opencntread--;
	}
      else
	{
	  xceshared_release();
	  XCETrace("WARNING: Closing zero client pipe read handle");
	  return FALSE;
	}
    }
  else
    {
      if(xceshared_pipetab[pipeindex].opencntwrite > 0)
	{
	  xceshared_pipetab[pipeindex].opencntwrite--;
	}
      else
	{
	  xceshared_release();
	  XCETrace("WARNING: Closing zero client pipe write handle");
	  return FALSE;
	}
    }

  if(xceshared_pipetab[pipeindex].opencntread == 0 &&
     xceshared_pipetab[pipeindex].opencntwrite == 0)
    {
      xceshared_pipetab[pipeindex].used = 0;
    }

  xceshared_release();
  return TRUE;
}

int
pipe_increment_opencount(HANDLE hFile)
{
  int pipeindex;

  if(!ISPIPEHANDLE(hFile))
    return -1;

  pipeindex = DECODEPIPEHANDLE(hFile);

  XCETrace("inc_open_count: before %d/%d",
	   xceshared_pipetab[pipeindex].opencntread,
	   xceshared_pipetab[pipeindex].opencntwrite);

  if(ISPIPEHANDLE(hFile) && ISINHERIT(hFile))
    {
      // must add one for the inherited program, because
      // the caller normally closed duplicated handles...
      if(ISPIPEREADHANDLE(hFile))
	{
	  xceshared_pipetab[pipeindex].opencntread++;
	}
      else
	{
	  xceshared_pipetab[pipeindex].opencntwrite++;
	}
    }

  XCETrace("inc_open_count: after %d/%d",
	   xceshared_pipetab[pipeindex].opencntread,
	   xceshared_pipetab[pipeindex].opencntwrite);

  return 0;
}

BOOL
XCECloseHandle(HANDLE handle)
{
  int res = TRUE;

  if(ISPIPEHANDLE(handle))
    res = close_pipe(handle);
  else
    res = CloseHandle(handle);

  return res;
}

BOOL
XCEDuplicateHandle(HANDLE hProc1, HANDLE hFile1, 
		  HANDLE hProc2, HANDLE *phFile2,
		  DWORD dwAccess, BOOL bInherit, 
		  DWORD dwOpts
		  )
{
  XCETrace("DuplicateHandle(%x)", hFile1);

  if(hProc1 != hProc2)
    {
      XCETrace("DuplicateHandle on different processes");
      return FALSE;
    }

  if(hProc1 != GetCurrentProcess())
    {
      XCETrace("DuplicateHandle on different process");
      return FALSE;
    }

  if(!ISPIPEHANDLE(hFile1))
    {
      XCETrace("DuplicateHandle on non-pipe");
      //return FALSE;
    }

  *phFile2 = MAKEINHERIT(hFile1);

  if(ISPIPEHANDLE(hFile1))
    pipe_increment_opencount(*phFile2);

  return TRUE;
}

BOOL
XCESetStdHandle(DWORD dwId, HANDLE hFile)
{
  XCETrace("SetStdHandle(%d, %x)", dwId, hFile);

  if(hFile == NULL || hFile == INVALID_HANDLE_VALUE)
    {
      // Do nothing...
    }
  else if(!ISPIPEHANDLE(hFile))
    {
      // We just set it to invalid...
      XCETrace("Warning: Handle is not a pipe");
      hFile = INVALID_HANDLE_VALUE;
      //return FALSE;
    }

  if(dwId == STD_INPUT_HANDLE)
    {
      xcelocal_procstart.hstdin = hFile;
    }
  else if(dwId == STD_OUTPUT_HANDLE)
    {
      xcelocal_procstart.hstdout = hFile;
    }
  if(dwId == STD_ERROR_HANDLE)
    {
      xcelocal_procstart.hstderr = hFile;
    }

  return TRUE;
}

HANDLE
XCEGetStdHandle(DWORD dwId)
{
  HANDLE hFile = INVALID_HANDLE_VALUE;

  XCETrace("GetStdHandle(%d)", dwId);

  if(dwId == STD_INPUT_HANDLE)
    {
      hFile = xcelocal_procstart.hstdin;
    }
  else if(dwId == STD_OUTPUT_HANDLE)
    {
      hFile = xcelocal_procstart.hstdout;
    }
  if(dwId == STD_ERROR_HANDLE)
    {
      hFile = xcelocal_procstart.hstderr;
    }

  if(!ISPIPEHANDLE(hFile))
    {
      XCETrace("Warning: Handle is not a pipe");

      if(dwId == STD_INPUT_HANDLE)
	{
	  hFile = MAKECONSREADHANDLE(0);
	}
      else if(dwId == STD_OUTPUT_HANDLE)
	{
	  hFile = MAKECONSWRITEHANDLE(1);
	}
      if(dwId == STD_ERROR_HANDLE)
	{
	  hFile = MAKECONSWRITEHANDLE(2);
	}
    }

  return hFile;
}

DWORD
XCEGetFileType(HANDLE hFile)
{
  XCETrace("GetFileType(%x)", hFile);

  if(hFile == INVALID_HANDLE_VALUE)
    return FILE_TYPE_UNKNOWN;

  if(ISPIPEHANDLE(hFile))
    {
      return FILE_TYPE_PIPE;
    }

  return FILE_TYPE_DISK;
}

BOOL 
XCEPeekNamedPipe(HANDLE   hPipe, 
		 LPVOID   lpvBuffer,	
		 DWORD    cbBuffer,
		 LPDWORD  lpcbRead,
		 LPDWORD  lpcbAvail,
		 LPDWORD  lpcbMessage)
{
  struct pipe_entry *pp;
  int pipeindex;

  XCETrace("PeekNamedPipe(%x)", hPipe);

  if(!ISPIPEHANDLE(hPipe))
    {
      return FALSE;
    }

  SYNC;

  pipeindex = DECODEPIPEHANDLE(hPipe);
  pp = &xceshared_pipetab[pipeindex];

  // does not copy data yet...
  *lpcbAvail = pp->avail;

  return TRUE;
}

int
xceshared_attach()
{
  return 0;
}

int
xceshared_detach()
{
  // TODO: We should also close the event handles we have
  // allocated in CreatePipe()
  //
  // We should also have a table of other pipes,
  // not only standard handles...

  if(ISPIPEHANDLE(XCEGetStdHandle(STD_INPUT_HANDLE)))
     XCECloseHandle(XCEGetStdHandle(STD_INPUT_HANDLE));
  if(ISPIPEHANDLE(XCEGetStdHandle(STD_OUTPUT_HANDLE)))
     XCECloseHandle(XCEGetStdHandle(STD_OUTPUT_HANDLE));
  if(ISPIPEHANDLE(XCEGetStdHandle(STD_ERROR_HANDLE)))
     XCECloseHandle(XCEGetStdHandle(STD_ERROR_HANDLE));

  return 0;
}
