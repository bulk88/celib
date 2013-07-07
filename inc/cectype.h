/*	$NetBSD: ctype.h,v 1.19 2001/01/21 10:44:15 itojun Exp $	*/

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)ctype.h	5.3 (Berkeley) 4/3/91
 */

#ifndef _CECTYPE_H_
#define _CECTYPE_H_

#include "celib_defs.h"

#define	_U	0x01
#define	_L	0x02
#define	_N	0x04
#define	_S	0x08
#define	_P	0x10
#define	_C	0x20
#define	_X	0x40
#define	_B	0x80

XCE_BEGIN_DECLS

extern XCE_EXPORT const unsigned char *_xce_ctype_;
extern XCE_EXPORT const short *_xce_tolower_tab_;
extern XCE_EXPORT const short *_xce_toupper_tab_;

XCE_EXPORT int	xceisalnum __XCEP ((int));
XCE_EXPORT int	xceisalpha __XCEP ((int));
XCE_EXPORT int	xceiscntrl __XCEP ((int));
XCE_EXPORT int	xceisdigit __XCEP ((int));
XCE_EXPORT int	xceisgraph __XCEP ((int));
XCE_EXPORT int	xceislower __XCEP ((int));
XCE_EXPORT int	xceisprint __XCEP ((int));
XCE_EXPORT int	xceispunct __XCEP ((int));
XCE_EXPORT int	xceisspace __XCEP ((int));
XCE_EXPORT int	xceisupper __XCEP ((int));
XCE_EXPORT int	xceisxdigit __XCEP ((int));
XCE_EXPORT int	xcetolower __XCEP ((int));
XCE_EXPORT int	xcetoupper __XCEP ((int));
XCE_EXPORT int	xceisascii __XCEP ((int));
XCE_EXPORT int	xcetoascii __XCEP ((int));
XCE_EXPORT int	xceisblank __XCEP ((int));

XCE_END_DECLS

// WINCE300 uses a function call (stdlib.h)
#if UNDER_CE >= 300
#undef isalpha
#undef isupper
#undef islower
#undef isdigit
#undef isxdigit
#undef isspace
#undef ispunct
#undef isalnum
#undef isprint
#undef isgraph
#undef iscntrl
#undef isascii
#endif

#define	isdigit(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _N))
#define	islower(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _L))
#define	isspace(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _S))
#define	ispunct(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _P))
#define	isupper(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _U))
#define	isalpha(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_U|_L)))
#define	isxdigit(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_N|_X)))
#define	isalnum(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_U|_L|_N)))
#define	isprint(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_P|_U|_L|_N|_B)))
#define	isgraph(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_P|_U|_L|_N)))
#define	iscntrl(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _C))
#define	tolower(c)	((int)((_xce_tolower_tab_ + 1)[(int)(c)]))
#define	toupper(c)	((int)((_xce_toupper_tab_ + 1)[(int)(c)]))
#define	isascii(c)	((unsigned)(c) <= 0177)
#define	toascii(c)	((c) & 0177)

#define	xceisdigit(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _N))
#define	xceislower(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _L))
#define	xceisspace(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _S))
#define	xceispunct(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _P))
#define	xceisupper(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _U))
#define	xceisalpha(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_U|_L)))
#define	xceisxdigit(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_N|_X)))
#define	xceisalnum(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_U|_L|_N)))
#define	xceisprint(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_P|_U|_L|_N|_B)))
#define	xceisgraph(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & (_P|_U|_L|_N)))
#define	xceiscntrl(c)	((int)((_xce_ctype_ + 1)[(int)(c)] & _C))
#define	xcetolower(c)	((int)((_xce_tolower_tab_ + 1)[(int)(c)]))
#define	xcetoupper(c)	((int)((_xce_toupper_tab_ + 1)[(int)(c)]))
#define	xceisascii(c)	((unsigned)(c) <= 0177)
#define	xcetoascii(c)	((c) & 0177)

#define _CTYPE_NUM_CHARS	(1<<(sizeof(char)<<3))

#define _CTYPE_ID	 	"XCECTYPE"
#define _CTYPE_REV		2

extern const unsigned char _XCE_ctype_[];
extern const short *_XCE_tolower_tab_;
extern const short *_XCE_toupper_tab_;

#endif
