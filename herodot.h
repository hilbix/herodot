/** @file herodot.h
 *
 * Embedded shell-parsable pure-text append-only database with checksums and automatic indexes.
 *
 * Not yet optimized for speed.
 *
 * For CRC computation see:
 * http://www.drdobbs.com/parallel/fast-parallelized-crc-computation-using/229401411
 * http://www.intel.ua/content/dam/www/public/us/en/documents/white-papers/crc-iscsi-polynomial-crc32-instruction-paper.pdf
 * http://www.ross.net/crc/download/crc_v3.txt
 */

#ifndef _INC_HERODOT_H_
#ifdef HERODOT

/* Structure definition	*/

#define H0(A,B)		HI(A,B,(H))
#define H1(A,B,C)	HI(A,B,(H,C))
#define H2(A,B,C,D)	HI(A,B,(H,C,D))
#define H3(A,B,C,D,E)	HI(A,B,(H,C,D,E))

H2(H		, ptr	, S, P)
H2(H		, nr	, S, N)
H2(H		, str	, S, S)
H2(H		, bool	, S, B)
H2(H		, cb	, S, F)		/** < first arg of callback F always is H, must return 0	*/

H1(H		, open	, S)
H0(H		, close	)

H0(H		, err	)
H0(int		, Err	)
H0(H		, exit	)
H0(int		, Exit	)
H3(H		, bug	, int, const char *, ...)

H2(H		, alloc	, void **	, size_t)
H2(void *	, Alloc	, void *	, size_t)
H1(H		, free	, const void *	)

#undef	H3
#undef	H2
#undef	H1
#undef	H0
#undef	HI

#else

#include "tino/debug.h"

#warning "inner"

#include <errno.h>

#define	HERODOT	struct herodot *

#define	H HERODOT
#define	S const char *
#define	N long long
#define	B int
#define	P const void *
#define	F int (*)()

#define	HERODOT_USER	"user"
#define	HERODOT_DEBUG	"debug"
#define	HERODOT_ERR	"err"
#define	HERODOT_FATAL	"fatal"
#define	HERODOT_WARN	"warn"
#define	HERODOT_READONLY	"ro"
#define	HERODOT_STDERR	"stderr"

#define	HI(X,Y,Z)	static X herodot_##Y Z;
#include "herodot.h"
#warning "one"

struct herodot
  {
    void	*user;		/* user pointer	*/

#define	HI(X,Y,Z)	X (*Y) Z;
#include "herodot.h"
#warning "two"

    unsigned	allocated:1;
    int		errnr;
  };

/** Inttialize structure
 *
 * Param: init (optional)
 */
H
herodot(H init)
{
  DP(("(%p)", init));

  H	h = init;

  if (!h)
    h	= herodot_Alloc(NULL, NULL, sizeof *h);
  if (!h)
    return h;
  memset(h, 0, sizeof *h);
  if (!init)
    h->allocated	= 1;

#define	HI(X,Y,Z)	h->Y = herodot_##Y;
#include "herodot.h"
#warning "three"

  DP((" %p", h));
  return h;
}

#define _INC_HERODOT_H_


/** create a herodot structure
 *
 * Argument is pre-allocated structure (which will be NULled).
 */

static H herodot_err(H h) { errno = h->Err(h); return h; }
static int
herodot_Err(H h)
{
  int	e;

  e	= h->errnr;
  h->errnr = 0;
  return e;
}

static H
herodot_exit(H h)
{
  int	a;

  a = h->allocated;
  errno = h->Exit(h);
  return a ? NULL : h;
}

static int
herodot_Exit(H h)
{
  int	e;
  H	(*f)(H, const void *);

  e = herodot_Err(h);

  000;	/* free structures	*/

  f = h->allocated ? h->free : 0;
  memset(h, 0, sizeof *h);
  if (f)
    f(NULL, h);

  return e;
}

static H
herodot_alloc(H h, void **ptr, size_t len)
{
  DP(("(%p, %p, %llu)", h, ptr, (unsigned long long)len));
  *ptr = h->Alloc(h, *ptr, len);
  return h;
}

static void *
herodot_Alloc(H h, void *ptr, size_t len)
{
  DP(("(%p, %p, %llu)", h, ptr, (unsigned long long)len));
  ptr = ptr ? realloc(ptr, len) : malloc(len);
  if (!ptr)
    h->bug(h, ENOMEM, "out of memory allocating %llu bytes", (unsigned long long)len);
  DP((" %p", ptr));
  return ptr;
}

static H
herodot_free(H h, P ptr)
{
  DP(("(%p, %p)", h, ptr));
  if (ptr)
    free((void *)ptr);
  return h;
}

static H
herodot_ptr(H h, S ent, P ptr)
{
  DP(("(%p, %s, %p)", h, ent, ptr));

  000;
  return h;
}

static H herodot_nr(H h, S ent, N nr) { 000; return h; }
static H herodot_str(H h, S ent, S s) { 000; return h; }
static H herodot_bool(H h, S ent, B on) { 000; return h; }
static H herodot_cb(H h, S ent, int (*fn)()) { 000; return h; }
static H herodot_open(H h, S name) { 000; return h; }
static H herodot_close(H h) { 000; return h; }
static H herodot_bug(H h, int errnr, S s, ...) { 000; return h; }

#undef F
#undef P
#undef B
#undef N
#undef S
#undef H

#endif
#endif

