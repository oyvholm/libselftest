<!-- NEWS.md -->
<!-- File ID: c3300bcc-c43b-11f1-b6f9-9bfc79a8fe4f -->

Summary of user-visible changes in libselftest
==============================================

For a complete log of changes, refer to the Git commit log in the 
repositories mentioned in `README.md`.

v0.1.0 - 2026-xx-xx
-------------------

First release.

- Embedded TAP testing framework for C programs. Copy `selftest.c` and 
  `selftest.h` into your project with `make export`, which inserts the 
  library version and date. It is not installed as a shared or static 
  library.
- Test macros for integers, booleans, pointers, strings and memory 
  (`OK_EQUAL`, `OK_STRCMP`, `OK_MEMCMP` and friends), with `OK_*_L` 
  variants that take the caller's file name and line number.
- `sc()`, `tc()` and `rc()` run a command and check stdout, stderr and 
  the exit status using substring, exact or POSIX extended regular 
  expression matching. `EXECSTR` stands for the name of the executable 
  in the expected output.
- Function tests and executable tests, a temporary directory 
  (`LST_TMPDIR`), and optional Valgrind checking with 
  `lst_use_valgrind`.
- Build without the test suite by defining `NO_SELFTEST`.
- The `libselftest`(3) man page describes all public names, and `make 
  docsync` checks that it stays complete. `lst-demo` is a complete 
  example program.
- Licensed under the GNU Lesser General Public License, version 2.1 or 
  later.

v0.0.0 - 2026-04-28
-------------------

Birth of the universe. Initial empty commit.

<!--
vim: set ts=2 sw=2 sts=2 tw=72 et fo=tcqw fenc=utf8 :
vim: set com=b\:#,fb\:-,fb\:*,n\:> ft=markdown :
-->
