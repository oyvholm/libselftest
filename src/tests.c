/*
 * tests.c
 * File ID: 54fd13c2-4e41-11f1-8be7-83850402c3ce
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

#include "lst-demo.h"

#ifndef NO_SELFTEST

#define TYPE_HELP_STR  EXECSTR ": Type \"" EXECSTR " --help\" for help" \
                       " screen. Returning with value 1.\n"
#define OPTION_ERROR_STR  EXECSTR ": Option error\n" TYPE_HELP_STR

/******************************************************************************
                             --selftest functions
******************************************************************************/

/*
 * print_version_info() - Displays output from the --version command. Returns 0 
 * if ok, or 1 if lst_streams_exec() failed.
 */

static int print_version_info(void)
{
	struct lst_streams ss;
	int res;

	lst_streams_init(&ss);
	res = lst_streams_exec(&ss, chp{ lst_execname, "--version", NULL });
	if (res) {
		failed_ok("lst_streams_exec()"); /* gncov */
		if (ss.err.buf) /* gncov */
			lst_diag(ss.err.buf); /* gncov */
		return 1; /* gncov */
	}
	lst_diag("========== BEGIN version info ==========\n"
	         "%s"
	         "=========== END version info ===========",
	         no_null(ss.out.buf));
	lst_streams_free(&ss);

	return 0;
}

/******************************************************************************
                     lst-demo-specific selftest functions
******************************************************************************/

/*
 * init_testvars() - Initializes file-static variables and environment 
 * variables to use the temporary directory defined in LST_TMPDIR. Returns 1 if 
 * anything fails, or 0 if OK.
 */

static int init_testvars(void)
{
	lst_diag("Initialize variables and the environment");

	return !!lst_init_home();
}

/*
 * cleanup_tempdir() - Delete all known filesystem entries in the temporary 
 * work directory defined in LST_TMPDIR. Expects `linenum` to be the `__LINE__` 
 * value from a parent function. Returns nothing.
 */

static void cleanup_tempdir(const char *file, const int linenum)
{
	lst_cleanup_tempdir(file, linenum);
}

/******************************************************************************
                 Function tests, no temporary directory needed
******************************************************************************/

                             /*** lst-demo.c ***/

/*
 * test_std_strerror() - Tests the std_strerror() function. Returns nothing.
 */

static void test_std_strerror(void)
{
	lst_diag("Test std_strerror()");
	OK_STRCMP(std_strerror(EACCES), "Permission denied",
	          "std_strerror(EACCES) is as expected");
}

/******************************************************************************
                   Function tests, use a temporary directory
******************************************************************************/

                            /*** lst-demo.c ***/

/*
 * test_myerror() - Tests the myerror() function. Returns nothing.
 */

static void test_myerror(void)
{
	int res, len_stderr;
	const char *desc;
	char *orig_progname = progname, *exp_stderr;

	lst_diag("Test myerror()");

	desc = "myerror() with float";
	if (lst_init_output_files()) {
		lst_restore_output_files(); /* gncov */
		failed_ok("lst_init_output_files()"); /* gncov */
		return; /* gncov */
	}
	errno = EACCES;
	progname = lst_execname;
	res = myerror("Test with float: %.5f", 3.14159);
	progname = orig_progname;
	lst_restore_output_files();
	OK_EQUAL(errno, 0, "%s (errno)", desc);
	print_gotexp_int(errno, 0);
	exp_stderr = lst_allocstr("%s: Test with float: 3.14159: Permission"
	                          " denied\n", lst_execname);
	if (!exp_stderr) {
		failed_ok("lst_allocstr()"); /* gncov */
		return; /* gncov */
	}
	len_stderr = (int)strlen(exp_stderr);
	verify_output_files(desc, "", exp_stderr);
	OK_EQUAL(res, len_stderr, "%s (retval)", desc);
	print_gotexp_int(res, len_stderr);
	cleanup_tempdir(__FILE__, __LINE__);
	free(exp_stderr);
}

/*
 * chk_arg_sum() - Used by test_arg_sum(). Calls arg_sum() with `args` and a 
 * buffer of size `bufsize`, and verifies that the return value is `exp` (NULL 
 * means that NULL is expected) and that stderr is `exp_stderr`. `desc` is a 
 * short test description, and `linenum` is the `__LINE__` value from the 
 * caller. Returns nothing.
 */

static void chk_arg_sum(const int linenum,
                        char *const args[], const size_t bufsize,
                        const char *exp, const char *exp_stderr,
                        const char *desc)
{
	char buf[ARG_SUM_BUFSIZE];
	const char *res;
	char *orig_progname = progname, *s;

	assert(args);
	assert(bufsize <= sizeof(buf));
	assert(exp_stderr);
	assert(desc);

	if (lst_init_output_files()) {
		lst_restore_output_files(); /* gncov */
		failed_ok("lst_init_output_files()"); /* gncov */
		return; /* gncov */
	}
	progname = lst_execname;
	res = arg_sum(buf, bufsize, args);
	progname = orig_progname;
	lst_restore_output_files();
	if (exp) {
		OK_STRCMP(no_null(res), exp, "%s (retval)", desc);
	} else {
		OK_NULL(res, "%s (retval is NULL)", desc);
	}
	s = *exp_stderr ? lst_allocstr("%s: %s\n", lst_execname, exp_stderr)
	                : lst_allocstr("%s", "");
	if (!s) {
		failed_ok("lst_allocstr()"); /* gncov */
		return; /* gncov */
	}
	verify_output_files(desc, "", s);
	free(s);
	cleanup_tempdir(__FILE__, linenum);
}

/*
 * test_arg_sum() - Tests the arg_sum() function. Returns nothing.
 */

static void test_arg_sum(void)
{
	lst_diag("Test arg_sum()");
	chk_arg_sum(__LINE__,
	            chp{ NULL }, ARG_SUM_BUFSIZE, "0", "", "Empty list");
	chk_arg_sum(__LINE__,
	            chp{ "1", "2", "3", NULL }, ARG_SUM_BUFSIZE, "6", "",
	            "1 + 2 + 3");
	chk_arg_sum(__LINE__,
	            chp{ "-3", "5", NULL }, ARG_SUM_BUFSIZE, "2", "",
	            "-3 + 5");
	chk_arg_sum(__LINE__,
	            chp{ "-10", "-20", NULL }, ARG_SUM_BUFSIZE, "-30", "",
	            "Only negative numbers");
	chk_arg_sum(__LINE__,
	            chp{ "9223372036854775807", NULL }, ARG_SUM_BUFSIZE,
	            "9223372036854775807", "", "LLONG_MAX");
	chk_arg_sum(__LINE__,
	            chp{ "-9223372036854775808", NULL }, ARG_SUM_BUFSIZE,
	            "-9223372036854775808", "", "LLONG_MIN fits in buffer");
	chk_arg_sum(__LINE__,
	            chp{ "1", "abc", NULL }, ARG_SUM_BUFSIZE, NULL,
	            "abc: Not an integer", "Not an integer");
	chk_arg_sum(__LINE__,
	            chp{ "12x", NULL }, ARG_SUM_BUFSIZE, NULL,
	            "12x: Not an integer", "Trailing garbage");
	chk_arg_sum(__LINE__,
	            chp{ "", NULL }, ARG_SUM_BUFSIZE, NULL,
	            ": Not an integer", "Empty string");
	chk_arg_sum(__LINE__,
	            chp{ "9223372036854775807", "1", NULL }, ARG_SUM_BUFSIZE,
	            NULL, "1: Sum is out of range", "LLONG_MAX + 1");
	chk_arg_sum(__LINE__,
	            chp{ "9223372036854775808", NULL }, ARG_SUM_BUFSIZE, NULL,
	            "9223372036854775808: Sum is out of range",
	            "Argument is larger than LLONG_MAX");
	chk_arg_sum(__LINE__,
	            chp{ "-9223372036854775808", "-1", NULL },
	            ARG_SUM_BUFSIZE, NULL, "-1: Sum is out of range",
	            "Below LLONG_MIN");
	chk_arg_sum(__LINE__,
	            chp{ "1000", NULL }, 4, NULL,
	            "arg_sum(): Buffer is too small", "Buffer is too small");
	chk_arg_sum(__LINE__,
	            chp{ "1000", NULL }, 5, "1000", "",
	            "Buffer is exactly big enough");
}

/******************************************************************************
            Test the executable file, no temporary directory needed
******************************************************************************/

                             /*** --valgrind ***/

/*
 * test_valgrind_option() - Tests the --valgrind command line option. Returns 
 * nothing.
 */

static void test_valgrind_option(void)
{
	struct Options o = opt_struct();
	struct lst_streams ss;

	lst_diag("Test --valgrind");

	if (o.valgrind) {
		lst_use_valgrind = false; /* gncov */
		lst_streams_init(&ss); /* gncov */
		lst_streams_exec(&ss, chp{ "valgrind", /* gncov */
		                           "--version", NULL });
		if (!strstr(ss.out.buf, "valgrind-")) { /* gncov */
			OK_ERROR("Valgrind is not installed," /* gncov */
			         " disabling Valgrind checks");
			lst_use_valgrind = false; /* gncov */
		} else {
			OK_SUCCESS(0, "Valgrind is installed"); /* gncov */
			lst_use_valgrind = true; /* gncov */
		}
		lst_streams_free(&ss); /* gncov */
	}

	sc((chp{ lst_execname, "--valgrind", "-h", NULL }),
	   "Show this",
	   "",
	   EXIT_SUCCESS,
	   "--valgrind -h");
}

                          /*** Standard options ***/

/*
 * test_standard_options() - Tests the various generic options available in 
 * most programs. Returns nothing.
 */

static void test_standard_options(void)
{
	char *s;

	lst_diag("Test standard options");

	lst_diag("Test -h/--help");
	sc((chp{ lst_execname, "-h", NULL }),
	   "  Show this help",
	   "",
	   EXIT_SUCCESS,
	   "-h");
	sc((chp{ lst_execname, "--help", NULL }),
	   "  Show this help",
	   "",
	   EXIT_SUCCESS,
	   "--help");

	lst_diag("Test -v/--verbose");
	sc((chp{ lst_execname, "-h", "--verbose", NULL }),
	   "  Show this help",
	   "",
	   EXIT_SUCCESS,
	   "-hv: Help text is displayed");
	sc((chp{ lst_execname, "-hv", NULL }),
	   EXEC_VERSION,
	   "",
	   EXIT_SUCCESS,
	   "-hv: Version number is printed along with the help text");
	sc((chp{ lst_execname, "-vvv", "--verbose", "--help", NULL }),
	   "  Show this help",
	   EXECSTR ": main(): Using verbose level 4\n",
	   EXIT_SUCCESS,
	   "-vvv --verbose: Using correct verbose level");
	sc((chp{ lst_execname, "-vvvvq", "-v", "--verbose", "--help", NULL }),
	   "  Show this help",
	   EXECSTR ": main(): Using verbose level 5\n",
	   EXIT_SUCCESS,
	   "--verbose: One -q reduces the verbosity level");

	lst_diag("Test --version");
	s = lst_allocstr("%s %s (%s)\n",
	                 lst_execname, EXEC_VERSION, EXEC_DATE);
	if (s) {
		sc((chp{ lst_execname, "--version", NULL }),
		   s,
		   "",
		   EXIT_SUCCESS,
		   "--version");
		free(s);
	} else {
		failed_ok("lst_allocstr()"); /* gncov */
	}
	tc((chp{ lst_execname, "--version", "-q", NULL }),
	   EXEC_VERSION "\n",
	   "",
	   EXIT_SUCCESS,
	   "--version with -q shows only the version number");

	lst_diag("Test --license");
	rc((chp{ lst_execname, "--license", NULL }),
	   "GNU Lesser General Public License.*either version 2.1 of the"
	   " License",
	   "^$",
	   EXIT_SUCCESS,
	   "--license: It's version 2.1 of the LGPL");

	lst_diag("Unknown option");
	sc((chp{ lst_execname, "--gurgle", NULL }),
	   "",
	   OPTION_ERROR_STR,
	   EXIT_FAILURE,
	   "Unknown option: \"Option error\" message is printed");
}

                          /*** Sum of arguments ***/

/*
 * test_sum_args() - Tests that the program prints the sum of its arguments. 
 * Returns nothing.
 */

static void test_sum_args(void)
{
	lst_diag("Test sum of arguments");
	tc((chp{ lst_execname, NULL }),
	   "0\n",
	   "",
	   EXIT_SUCCESS,
	   "No arguments, sum is 0");
	tc((chp{ lst_execname, "1", "2", "3", NULL }),
	   "6\n",
	   "",
	   EXIT_SUCCESS,
	   "1 2 3");
	tc((chp{ lst_execname, "--", "-3", "5", NULL }),
	   "2\n",
	   "",
	   EXIT_SUCCESS,
	   "-- -3 5");
	tc((chp{ lst_execname, "1", "abc", NULL }),
	   "",
	   EXECSTR ": abc: Not an integer\n",
	   EXIT_FAILURE,
	   "1 abc: Not an integer");
	tc((chp{ lst_execname, "9223372036854775807", "1", NULL }),
	   "",
	   EXECSTR ": 1: Sum is out of range\n",
	   EXIT_FAILURE,
	   "LLONG_MAX + 1 is out of range");
	tc((chp{ lst_execname, "--", "-9223372036854775808", "-1", NULL }),
	   "",
	   EXECSTR ": -1: Sum is out of range\n",
	   EXIT_FAILURE,
	   "Sum below LLONG_MIN is out of range");
}

/******************************************************************************
              Test the executable file with a temporary directory
******************************************************************************/

/*
 * functests_with_tempdir() - Tests functions that need a temporary directory 
 * to store the output from stderr and stdout. Returns nothing.
 */

static void functests_with_tempdir(void)
{
	int result;

	lst_diag("Test functions that need a temporary directory for stdout"
	         " and stderr");
	result = mkdir(LST_TMPDIR, 0755);
	OK_SUCCESS(result, "mkdir " LST_TMPDIR " for function tests");
	if (result) {
		lst_diag("Cannot create directory \"%s\", skipping" /* gncov */
		         " tests: %s",
		         LST_TMPDIR, strerror(errno)); /* gncov */
		errno = 0; /* gncov */
		return; /* gncov */
	}

	/* lst-demo.c */
	test_myerror();
	test_arg_sum();

	result = rmdir(LST_TMPDIR);
	OK_SUCCESS(result, "rmdir " LST_TMPDIR " after function tests");
	if (result) {
		diag_errno(); /* gncov */
		return; /* gncov */
	}
}

/*
 * tests_with_tempdir() - Executes tests on the executable with a temporary 
 * directory. The temporary directory uses a standard name, defined in 
 * LST_TMPDIR. If the directory already exists or it's unable to create it, it 
 * aborts. Returns nothing.
 */

static void tests_with_tempdir(void)
{
	int result;

	result = mkdir(LST_TMPDIR, 0777);
	OK_SUCCESS(result, "%s(): Create temporary directory %s",
	                   __func__, LST_TMPDIR);
	if (result) {
		lst_diag("Cannot create directory \"%s\", skipping" /* gncov */
		         " tests: %s",
		         LST_TMPDIR, strerror(errno)); /* gncov */
		errno = 0; /* gncov */
		return; /* gncov */
	}

	if (OK_SUCCESS(rmdir(LST_TMPDIR),
	               "Delete temporary directory %s", LST_TMPDIR))
		diag_errno(); /* gncov */
}

/******************************************************************************
                        Top-level --selftest functions
******************************************************************************/

/*
 * test_functions() - Tests various functions directly. Returns nothing.
 */

static void test_functions(const struct Options *o)
{
	assert(o);

	if (!o->testfunc)
		return; /* gncov */

	lst_diag("Test various routines");

	/* lst-demo.c */
	test_std_strerror();

	functests_with_tempdir();
}

/*
 * test_executable() - Runs various tests with the executable and verifies that 
 * stdout, stderr and the return value are as expected. Returns nothing.
 */

static void test_executable(const struct Options *o)
{
	assert(o);
	if (!o->testexec)
		return; /* gncov */

	lst_diag("Test the executable");
	test_valgrind_option();
	print_version_info();
	test_standard_options();
	test_sum_args();
	tests_with_tempdir();
	print_version_info();
}

#endif /* ifndef NO_SELFTEST */

/*
 * opt_selftest() - Runs internal testing to check that it works on the current 
 * system. Executed if --selftest is used. Returns `EXIT_FAILURE` if any tests 
 * fail. Otherwise, it returns `EXIT_SUCCESS`.
 */

int opt_selftest(char *main_execname, const struct Options *o)
{
	assert(main_execname);
	assert(*main_execname);
	assert(o);

#ifndef NO_SELFTEST
	lst_execname = get_execname(main_execname);
	lst_diag("Running tests for %s %s (%s)",
	         lst_execname, EXEC_VERSION, EXEC_DATE);

	if (init_testvars()) {
		lst_diag("init_testvars() failed, aborting"); /* gncov */
		free(lst_execname); /* gncov */
		return EXIT_FAILURE; /* gncov */
	}
	test_selftest_funcs();
	test_functions(o);
	test_executable(o);
	print_selftest_footer();
	free(lst_execname);

	return selftest_failcount() ? EXIT_FAILURE : EXIT_SUCCESS;
#else
	fprintf(stdout, "# %s: This version is compiled without the built-in"
	                " test suite\n", main_execname);
	puts("1..0");
	if (o) { } /* Suppress potential unused variable warning */

	return EXIT_SUCCESS;
#endif
}

/* vim: set ts=8 sw=8 sts=8 noet fo+=w tw=79 fenc=UTF-8 : */
