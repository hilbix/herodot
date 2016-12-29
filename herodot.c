/* This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 *
 * Comments of self-explained code are skipped.
 */

#include "tino/getopt.h"

#include "herodot_version.h"

#include "herodot.h"

struct me
  {
    int		verbose;
  };

int
main(int argc, char **argv)
{
  HERODOT	H;
  struct me	me;
  int		argn;
  int		flag_quiet, flag_readonly;
  int		level_debug;

#if 0
  int		argn, flag, i, v;
  long		t;
  const char	*str;
  char		auxbuf[TINO_GETOPT_AUXBUF_SIZE];
  union tino_getopt_types	c;
  struct tino_getopt_impl	aux;

  str	= "(this value was not inititialized)";
#endif
  argn	= tino_getopt(argc, argv, 1, 0,
		      TINO_GETOPT_VERSION(HERODOT_VERSION)
		      " database [command args..]",

		      TINO_GETOPT_USAGE
		      "h	this help"
		      ,
#if 0
		      TINO_GETOPT_CHAR
		      "c char	a char"
		      , &c.c,

		      TINO_GETOPT_STRING
		      TINO_GETOPT_NODEFAULT
		      "s str	fetch a string"
		      , &str,

		      TINO_GETOPT_FLAG
		      TINO_GETOPT_DEFAULT
		      TINO_GETOPT_MIN
		      TINO_GETOPT_MAX
		      "f	set a flag"
		      , &flag,
		      10,
		      20,
		      30,

		      TINO_GETOPT_INT
		      TINO_GETOPT_DEFAULT
		      TINO_GETOPT_MIN
		      TINO_GETOPT_MAX
		      TINO_GETOPT_SUFFIX
		      "n nr	number (kmgt-suffix)"
		      , &i,
		      50,
		      0,
		      10000,

		      TINO_GETOPT_LONGINT
		      TINO_GETOPT_TIMESPEC
		      "t time	number (mhdw-suffix)"
		      , &t,

		      TINO_GETOPT_FLAG
		      TINO_GETOPT_MIN
		      TINO_GETOPT_MAX
		      "q	be more quiet"
		      , &v,
		      10,
		      -10,
#endif

		      TINO_GETOPT_INT
		      "d	debug level"
		      , &level_debug,

		      TINO_GETOPT_FLAG
		      "q	suppress warnings"
		      , &flag_quiet,

		      TINO_GETOPT_FLAG
		      "r	readonly mode (database is opened readonly)"
		      , &flag_readonly,

		      TINO_GETOPT_FLAG
		      "v	verbose output"
		      , &me.verbose,

		      NULL
		      );
  if (argn<=0)
    return 1;

  H	= herodot(NULL);			/* initialize structure	*/
  H->ptr(H, HERODOT_USER, &me);
  H->nr(H, HERODOT_DEBUG, level_debug);		/* default debug handler is STDERR	*/
  H->str(H, HERODOT_ERR, HERODOT_FATAL);	/* allow us to skip error checks	*/

  if (!flag_quiet)
    H->str(H, HERODOT_WARN, HERODOT_STDERR);

  H->bool(H, HERODOT_READONLY, flag_readonly);
  H->open(H, argv[argn]);

  return H->close(H)->Exit(H);			/* functions can be chained.  Always.	*/
}

