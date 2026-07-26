/*
 * lst-demo.h
 * File ID: 9c1f2a7a-4a58-11f1-b37b-83850402c3ce
 *
 * (C)opyleft 2026- Øyvind A. Holm <sunny@sunbase.org>
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _LST_DEMO_H
#define _LST_DEMO_H

#include "version.h"

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NO_SELFTEST
#include "selftest.h"
#endif

#if 1
#  define DEBL  msg(2, "DEBL: %s, line %u in %s()", \
                       __FILE__, __LINE__, __func__)
#else
#  define DEBL  ;
#endif

#ifdef CHECK_ERRNO
#define check_errno  do { \
	if (errno) { \
		myerror("%s():%s:%d: errno = %d", \
		        __func__, __FILE__, __LINE__, errno); \
	} \
} while (0)
#else
#define check_errno  do { } while (0)
#endif

#define failed(a)  myerror("%s():%d: %s failed", __func__, __LINE__, (a))
#define no_null(a)  ((a) ? (a) : "(null)")

struct Options {
	/* sort -d -k2 */
	bool help;
	bool license;
	bool selftest;
	bool testexec;
	bool testfunc;
	bool valgrind;
	int verbose;
	bool version;
};

/*
 * Public function prototypes
 */

/* lst-demo.c */
extern char *progname;
struct Options opt_struct(void);
int msg(const int verbose, const char *format, ...);
const char *std_strerror(const int errnum);
int myerror(const char *format, ...);
void init_opt(struct Options *dest);

/* tests.c */
int opt_selftest(char *execname, const struct Options *o);

#endif /* ifndef _LST_DEMO_H */

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
