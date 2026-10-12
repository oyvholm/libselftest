<!-- README.md -->
<!-- File ID: 9bcdfb50-4a58-11f1-8ca0-83850402c3ce -->

# libselftest

Embedded TAP testing framework for C programs.

## Overview

**libselftest** is a lightweight C testing framework designed to be 
compiled directly into the program. It produces TAP (Test Anything 
Protocol) output and supports both unit-style function tests and full 
executable tests.

### Main design goals

- Zero external test harness required
- The tests always match the exact binary that was compiled
- Easy to run on any machine where the program is installed
- High portability (Linux, the BSDs, Termux, and older systems)
- Clean compilation under very strict warning flags
- C99/POSIX.1-2001 compatible

### Why write the test suite in C?

- **Speed**\
  An execution rate of roughly 4000 tests per second on a normal 
  consumer laptop is typical. Real-world examples:

  - [geocalc](https://github.com/oyvholm/geocalc) — 1122 tests in 0.287 
    seconds ≈ 3909 tests/s
  - [suuid](https://github.com/oyvholm/suuid) — 979 tests in 0.234 
    seconds ≈ 4183 tests/s

- **Direct access to functions**\
  External test suites (Perl, Python, shell, etc.) can only drive the 
  program from the outside and inspect its output and exit status. When 
  the tests are written in C they can call functions directly, including 
  with values that are difficult or impossible to produce through normal 
  program flow. Because there is almost no per-test overhead, functions 
  can be exercised millions of times to uncover rare edge cases.

- **No language-version friction**\
  External test suites introduce an extra dependency on a particular 
  version of another language and its libraries. Keeping the tests in C 
  removes that class of compatibility problems.

- **Tests match the binary exactly**\
  The test code is compiled together with the program, so there is no 
  risk of the test suite drifting away from the code it is supposed to 
  verify.

- **Same quality bar**\
  The tests are subject to the same strict compiler warnings and coding 
  standards as the rest of the program.

- **No extra runtime dependencies**\
  Users (or CI) only need the binary itself; they do not need a separate 
  test harness or interpreter installed.

- **Natural fit for the "ship the tests" model**\
  Because the tests live inside the program, `./program --selftest` 
  becomes a reliable self-check that works on any machine where the 
  binary has been installed.

### Features

- TAP-compatible output (`ok` / `not ok`, plan, diagnostics)
- Rich set of assertion macros (`OK_EQUAL`, `OK_STRCMP`, `OK_SUCCESS`, 
  etc.)
- Capture of stdout/stderr/status from the program under test
- Built-in Valgrind support (`--valgrind`)
- Temporary directory management
- Support for both function tests and executable tests

For a complete list of test macros, refer to the overview in 
`libselftest`(3) and the descriptions in `selftest.h`.

### Status

Stable and ready for production use. The programming interface may 
change slightly until v1.0.0 is tagged.

It compiles with no warnings, has no known Valgrind errors, and is 
tested on the following platforms:

- Linux
- NetBSD
- FreeBSD
- OpenBSD
- Termux on Android

### Examples

The repository contains `lst-demo`, a small demonstration program that 
shows how to integrate and use the framework.

The test suite can be implemented in a number of ways, and the 
`lst-demo` demonstration program uses this implementation:

```sh
lst-demo --selftest            # Run all tests
lst-demo --selftest func       # Function tests only
lst-demo --selftest exec       # Executable tests only
lst-demo --selftest func,exec  # Both (same as "all")
lst-demo --valgrind            # Same as --selftest, but under Valgrind
```

The demonstration program also shows how to compile without the test 
suite by compiling with `-DNO_SELFTEST`.

### Using libselftest in your own program

1. Execute `make export` in the `src` directory.
2. Copy `export/selftest.c` and `export/selftest.h` to your project.
3. Call the test runner from your option parsing code when `--selftest` 
   or `--valgrind` is given.
4. Write your tests using the `OK_*` macros.

See the `lst-demo` program in this repository for a complete working 
example, and the `libselftest`(3) man page for an overview of all macros 
and functions. After `make`, it can be read with `man ./libselftest.3` 
in the `src` directory.

## Download

The main Git repository is stored at GitHub:

- URL: <https://github.com/oyvholm/libselftest>
- SSH clone: git@github.com:oyvholm/libselftest.git
- https clone: <https://github.com/oyvholm/libselftest.git>

The repository is mirrored to GitLab:

- URL: <https://gitlab.com/oyvholm/libselftest>
- SSH clone: git@gitlab.com:oyvholm/libselftest.git
- https clone: <https://gitlab.com/oyvholm/libselftest.git>

## Development

The `master` branch is considered stable; no unstable development 
happens there. Every new functionality or bug fix is created on topic 
branches which may be rebased now and then. All tests on `master` 
(executed with "make test") MUST succeed. If any test fails, it's 
considered a bug. Please report any failing tests in the issue tracker.

To ensure compatibility between versions, the project follows the 
Semantic Versioning Specification described at <https://semver.org>. 
Using the version number `X.Y.Z` as an example:

- `X` is the *major version*.
  This number is only incremented when backwards-incompatible changes 
  are introduced.
- `Y` is the *minor version*.
  Increased when new backwards-compatible features are added.
- `Z` is the *patch level*.
  Increased when new backwards-compatible bugfixes are added.

### Compiler flags for development

To avoid complications on various systems, the default build only uses 
`-Wall -O2`. Additional warning flags are enabled if any of the 
following conditions are met:

- The file `src/.devel` or `.git/.devel` exists
- The environment variable `DEVEL` is set to any value

These development flags can be explicitly disabled by setting the 
`NODEVEL` environment variable, regardless of the conditions above. The 
current `CFLAGS` can be checked with `make cflags`. For example:

- `make cflags`
- `make cflags DEVEL=1`
- `make cflags NODEVEL=true`

## `make` commands

### make / make all

Generate the `lst-demo` executable and the `lst-demo`(1) and 
`libselftest`(3) man pages.

### make export

Create copies of `selftest.c` and `selftest.h` in an `export` directory 
and insert the current library version and date (including the Git 
commit hash) into the files. This makes it easy to identify which 
version was installed. To specify a custom directory, use the 
`EXPORT_DIR` macro.

For example: `make export EXPORT_DIR=/path/to/project/src`

### make clean

Remove all generated files except `tags`.

### make docsync

Check that all public macros, functions, structs and extern variables in 
`selftest.h` are mentioned in bold in `src/libselftest.3.man`, so the 
man page doesn't fall behind when names are added or renamed. It is also 
executed by `make testsrc`.

### make edit

Open all files in the subtree in your favourite editor defined in 
`EDITOR`.

### make asm

Generate assembly code for all `.c` files. On many Unix-like systems, 
the assembly files are stored with a `.s` extension.

### make gcov

Generate test coverage with `gcov`(1). Should be as close to 100% as 
possible.

### make gcov-cmt / make gcov-cmt-clean

Add or remove `gcov` markers in the source code in lines that are not 
tested. Lines that are hard to test, for example full disk, full memory, 
long paths and so on, can be marked with the string `/* gncov */` to 
avoid marking them. To mark lines even when marked with gncov, set the 
`GNCOV` environment variable to a non-empty value. For example:

    make gcov-cmt GNCOV=1

### make gncov-refresh

Remove `gncov` markers from lines that no longer need to be excluded 
from coverage testing. This might be necessary if code changes or 
compiler optimizations make previously untestable lines testable. For 
example, if a function is inlined or optimized differently, lines that 
were once skipped might now be covered by tests.

These commands need the `gcov-cmt` script, available from 
<https://github.com/oyvholm/utils/raw/master/gcov-cmt>.

### make gdb

Start gdb with main() as the default breakpoint, this is defined in 
`src/gdbrc`. Any additional gdb options can be added in `src/gdbopts`. 
An example would be "-tty /dev/\[...\]" to send the program output to 
another window.

### make install

`make install` installs `lst-demo`, the `lst-demo`(1) man page, and the 
`libselftest`(3) man page (`man3`) to the location defined by `PREFIX` 
in `src/Makefile`. Default location is `/usr/local`, but it can be 
installed somewhere else by specifying `PREFIX`. For example:

    make install PREFIX=~/local

### make tags

Generate `tags` file, used by Vim and other editors.

### make test

Run all tests. This command MUST NOT fail on purpose on `master`.

### make uninstall

Delete the installed version from `PREFIX`.

### make valgrind

Run all tests with Valgrind to find memory leaks and other problems. 
Should also not fail on master.

### Create HTML or PDF

`make html` creates HTML versions of all documentation in the current 
directory tree, and `make pdf` creates PDF versions. When executed in 
the `src/` directory, HTML or PDF versions of the man pages are created, 
stored as `lst-demo.html` or `lst-demo.pdf`, and `libselftest.html` or 
`libselftest.pdf`.

All `*.md` files can be converted to HTML or PDF by replacing the `.md` 
extension with `.html` or `.pdf`. For example, use `make README.html` to 
generate an HTML file from the `.md` file, or `make README.pdf` to 
create a PDF version. If the `.md` file is stored in Git, an extra 
footer with the text "Generated from *filename* revision *git-id* 
(*date*)" is added.

Uses `cmark`, available from <https://commonmark.org/>.

## License

This library is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser General Public License as published by 
the Free Software Foundation; either version 2.1 of the License, or (at 
your option) any later version.

This library is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser 
General Public License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this library; if not, see <https://www.gnu.org/licenses/>.

## Author

Øyvind A. Holm \<<sunny@sunbase.org>\>

## About this document

This file is written in [CommonMark](https://commonmark.org) and all 
`make` commands use `cmark`(1) to generate HTML and reformat text.

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", 
"SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this 
document are to be interpreted as described in [RFC 
2119](https://www.rfc-editor.org/rfc/rfc2119.txt).

<!--
vim: set ts=2 sw=2 sts=2 tw=72 et fo=tcqwn fenc=utf8 :
vim: set com=b\:#,fb\:-,fb\:*,n\:> ft=markdown :
-->
