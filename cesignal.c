// xcesignal.c
//
// Time-stamp: <23/09/01 10:45:25 keuchel@netwave.de>

#include "celib.h"
#include "signal.h"
#include "xcetrace.h"

SIGHANDLER _sighandlers[NSIG];

SIGHANDLER
xcesignal(int sig, SIGHANDLER new)
{
  SIGHANDLER old;

  XCETRACE2(XCE_TRACE_SIGNALS, ("signal(%d, %x)", sig, new));

  if(sig < 0 || sig >= NSIG)
    return SIG_ERR;

  old = _sighandlers[sig];
  _sighandlers[sig] = new;

  return old;
}

int
xceraise(int sig)
{
  XCETrace("raise(%d)", sig);

  if(_sighandlers[sig] == SIG_IGN)
    {
      XCETrace("signal ignored");
    }
  else if(_sighandlers[sig] == SIG_DFL)
    {
      XCETrace("calling default handler");

      if(sig == SIGINT)
	exit(-1);
    }
  else
    {
      XCETrace("calling signal handler");
      (_sighandlers[sig])(sig);
      XCETrace("calling signal handler ok");
    }

  return 0;
}

int 
sigaction(int sig, struct sigaction *act,
	  struct sigaction *oldact)
{
  return 0;
}

int 
sigprocmask(int how, const sigset_t *set,
	    sigset_t *oldset)
{
  return 0;
}

int 
sigaddset(sigset_t *mask, int sig)
{
  *mask |= (1 << sig);

  return 0;
}

int 
sigemptyset(sigset_t *mask)
{
  *mask = 0;

  return 0;
}

int 
sigsuspend(const sigset_t *sigmask)
{
  return 0;
}

int 
xcekill(pid_t pid, int sig)
{
  if(pid == xcegetpid())
    {
      xceraise(sig);
    }

  errno = EINVAL;
  return -1;
}


