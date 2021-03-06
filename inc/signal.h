#ifndef SIGNAL_H
#define SIGNAL_H 1

#include "celib_defs.h"
#include "sys/types.h"

#define SIGHUP		1
#define SIGINT          2
#define SIGQUIT		3
#define SIGILL          4
#define SIGPIPE         5
#define SIGFPE          8
#define SIGUSR1		10
#define SIGSEGV         11
#define SIGUSR2		12
#define SIGTERM         15
#define SIGCHLD		17
#define SIGTSTP		20
#define SIGBREAK        21
#define SIGABRT         22
#define NSIG            22

#define SA_NOCLDSTOP	1
#define SA_SHIRQ	0x04000000
#define SA_STACK	0x08000000
#define SA_RESTART	0x10000000
#define SA_INTERRUPT	0x20000000
#define SA_NOMASK	0x40000000
#define SA_ONESHOT	0x80000000

/* signal action codes */

#define SIG_DFL (void (*)(int))0           /* default signal action */
#define SIG_IGN (void (*)(int))1           /* ignore signal */
#define SIG_SGE (void (*)(int))3           /* signal gets error */
#define SIG_ACK (void (*)(int))4           /* acknowledge */
#define SIG_ERR (void (*)(int))-1          /* signal error value */

#define SIG_BLOCK          0	/* for blocking signals */
#define SIG_UNBLOCK        1	/* for unblocking signals */
#define SIG_SETMASK        2	/* for setting the signal mask */

XCE_BEGIN_DECLS

typedef void (* SIGHANDLER)(int);
typedef void (* sighandler_t)(int);

typedef int sig_atomic_t;
typedef unsigned int sigset_t;

struct sigaction {
  sighandler_t sa_handler;
  sigset_t sa_mask;
  unsigned long sa_flags;
  void (*sa_restorer)(void);
};

XCE_EXPORT void (*xcesignal(int, void (*)(int)))(int);
XCE_EXPORT int xceraise(int);
XCE_EXPORT int xcekill(pid_t pid, int sig);

XCE_EXPORT int sigaddset(sigset_t *mask, int sig);
XCE_EXPORT int sigemptyset(sigset_t *mask);
XCE_EXPORT int sigdelset(sigset_t *mask, int sig);
XCE_EXPORT int sigfillset(sigset_t *mask);
XCE_EXPORT int sigismember(const sigset_t *mask, int sig);
XCE_EXPORT int sigpending(sigset_t *set);

XCE_EXPORT int sigblock(int mask);
XCE_EXPORT int sigpause(int mask);
XCE_EXPORT int sigsetmask(int mask);
XCE_EXPORT int siggetmask(void);

XCE_EXPORT int sigaction(int sig, struct sigaction *act,
			 struct sigaction *oldact);

XCE_EXPORT int sigprocmask(int how, const sigset_t *set,
			   sigset_t *oldset);

XCE_EXPORT int sigsuspend(const sigset_t *sigmask);

XCE_END_DECLS

#endif
