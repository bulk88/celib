/*-
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)pwd.h	5.13 (Berkeley) 5/28/91
 */

#ifndef _PWD_H_
#ifdef __cplusplus
extern "C" {
#endif
#define	_PWD_H_

#include <sys/types.h>
#if UNDER_CE > 201
#include <stdio.h>
#endif
#include <unistd.h>

#if UNDER_CE <= 201
typedef void FILE;
#endif

#define	_PATH_PASSWD		"\\Speicherkarte2\\etc\\passwd"
#define	_PASSWORD_LEN		128	/* max length, not counting NULL */

struct passwd {
	char	*pw_name;		/* user name */
	char	*pw_passwd;		/* encrypted password */
	int	pw_uid;			/* user uid */
	int	pw_gid;			/* user gid */
	char	*pw_comment;		/* comment */
	char	*pw_gecos;		/* Honeywell login info */
	char	*pw_dir;		/* home directory */
	char	*pw_shell;		/* default shell */
};

/* now in unixstd.h */

#if 0
typedef unsigned long uid_t;
typedef unsigned long gid_t;
#endif

XCE_EXPORT void xcesetpwent(void);
XCE_EXPORT struct passwd *xcegetpwent(void);
XCE_EXPORT void xceendpwent(void);
XCE_EXPORT struct passwd *xcegetpwnam(const char *name);
XCE_EXPORT struct passwd *xcegetpwuid(uid_t uid);
XCE_EXPORT char *xcegetusershell();
XCE_EXPORT void xceendusershell();
XCE_EXPORT int xceputpwent(struct passwd *pe, FILE *f);
XCE_EXPORT char *xcegetpass(char *prompt);
XCE_EXPORT char *xcegetlogin();

#ifdef __cplusplus
};
#endif
#endif /* _PWD_H_ */
