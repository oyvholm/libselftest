# Makefile for lst-demo.c
# File ID: 9be86602-4a58-11f1-b178-83850402c3ce
# Author: Øyvind A. Holm <sunny@sunbase.org>

CC = cc
EXEC = lst-demo
LD = cc
PREFIX = /usr/local

CFILES  =
CFILES += lst-demo.c
CFILES += selftest.c
CFILES += tests.c
CFLAGS  =
CFLAGS += $$($(IS_DEV) && echo -O0 || echo -O2)
CFLAGS += $$(test -n "$(GCOV)" && echo -n "-fprofile-arcs -ftest-coverage")
CFLAGS += $$(test -n "$(PROF)" && echo -n "-pg")
CFLAGS += $(DEVFLAGS_STR)
CFLAGS += -D 'LST_TMPDIR="$(LST_TMPDIR)"'
CFLAGS += -Wall
CFLAGS += -c
DEPS = version.h $(HFILES) Makefile
DEVEL =
DEVFLAGS  =
DEVFLAGS += $$(test "$$(uname -o)" = "Android" || echo -fcf-protection=full)
DEVFLAGS += -Wbad-function-cast
DEVFLAGS += -Wcast-align
DEVFLAGS += -Wcast-qual
DEVFLAGS += -Wchar-subscripts
DEVFLAGS += -Wconversion
DEVFLAGS += -Wdangling-else
DEVFLAGS += -Wdate-time
DEVFLAGS += -Wdouble-promotion
DEVFLAGS += -Werror
DEVFLAGS += -Werror=implicit-function-declaration
DEVFLAGS += -Wextra
DEVFLAGS += -Wfloat-conversion
DEVFLAGS += -Wformat
DEVFLAGS += -Wformat-overflow
DEVFLAGS += -Wformat-security
DEVFLAGS += -Wformat-signedness
DEVFLAGS += -Wformat-truncation
DEVFLAGS += -Wformat-y2k
DEVFLAGS += -Winit-self
DEVFLAGS += -Wint-in-bool-context
DEVFLAGS += -Wlogical-not-parentheses
DEVFLAGS += -Wmisleading-indentation
DEVFLAGS += -Wmissing-braces
DEVFLAGS += -Wold-style-definition
DEVFLAGS += -Woverlength-strings
DEVFLAGS += -Wpedantic
DEVFLAGS += -Wredundant-decls
DEVFLAGS += -Wshadow
DEVFLAGS += -Wstrict-aliasing
DEVFLAGS += -Wstrict-overflow=5
DEVFLAGS += -Wstrict-prototypes
DEVFLAGS += -Wuninitialized
DEVFLAGS += -Wunused-const-variable
DEVFLAGS += -Wunused-function
DEVFLAGS += -fno-common
DEVFLAGS += -fstack-protector-strong
DEVFLAGS += -fwrapv
DEVFLAGS += -g3
DEVFLAGS += -pedantic
DEVFLAGS_STR = $$($(IS_DEV) && echo $(DEVFLAGS))
EXPORT_DATE = $$(grep EXEC_DATE version.h | cut -d '"' -f 2)
EXPORT_DIR = export
EXPORT_VERSION = $$(grep EXEC_VERSION version.h | cut -d '"' -f 2)
GNCOV_STR = $$(test -n "$(GNCOV)" && echo "-g")
HFILES  =
HFILES += lst-demo.h
HFILES += selftest.h
HTMLFILE = $(EXEC).html
INSTALL_TARGETS  =
INSTALL_TARGETS += $(PREFIX)/bin/$(EXEC)
INSTALL_TARGETS += $(PREFIX)/share/man/man1/$(MANPAGE)
LDFLAGS  =
LDFLAGS += $$(test -n "$(PROF)" && echo -n "-pg")
LIBS  =
LIBS += $$(test -n "$(GCOV)" && echo "-lgcov --coverage")
LONGLINES_FILES  =
LONGLINES_FILES += $(CFILES)
LONGLINES_FILES += $(HFILES)
LONGLINES_FILES += $(MANSRC)
LONGLINES_FILES += Gen-version
LONGLINES_FILES += Makefile
LST_TMPDIR = .$(EXEC)-test.tmp
MANPAGE = $(EXEC).1
MANSRC = $(MANPAGE).man
MAN_DATE = $$(grep EXEC_DATE version.h | cut -d '"' -f 2 | sed 's/-/\\\\-/g;')
OBJFILES  =
OBJFILES += $$([ -z "$(NO_SELFTEST)" ] && echo selftest.o)
OBJFILES += lst-demo.o
OBJFILES += tests.o
OBJS  =
OBJS += lst-demo.o
OBJS += selftest.o
OBJS += tests.o
PDFFILE = $(EXEC).pdf
TESTS = all

#  Not in Clang
# DEVFLAGS += -Wc99-c11-compat
# DEVFLAGS += -Wduplicated-branches
# DEVFLAGS += -Wformat-overflow=2
# DEVFLAGS += -Wjump-misses-init
# DEVFLAGS += -Wlogical-op
# DEVFLAGS += -Wmaybe-uninitialized

#  Don't use yet
# DEVFLAGS += -Waggregate-return
# DEVFLAGS += -Wformat=2
# DEVFLAGS += -Wmissing-declarations
# DEVFLAGS += -Wpadded

CMD_VALGRIND  = valgrind
CMD_VALGRIND += -q
CMD_VALGRIND += --error-exitcode=1
CMD_VALGRIND += --leak-check=full
CMD_VALGRIND += --show-leak-kinds=all
CMD_VALGRIND += --
IS_DEV = [ -z "$(NODEVEL)" \
           -a \( -e .devel -o -e ../.git/.devel -o -n "$(DEVEL)" \) ]
MAN_VERSION = $$(grep EXEC_VERSION version.h | cut -d '"' -f 2 \
                 | sed 's/-/\\\\-/g;')
TESTABLE_LINES = $$(grep -E '^ +\#\#\#\#\#:' *.c.gcov \
                    | grep -Fiv '/* gncov */' | wc -l)

.PHONY: all
all: $(EXEC) $(MANPAGE)

# Build the executable file. If STRIP is non-empty, the debugging symbols are 
# stripped from the binary to reduce the size.
$(EXEC): $(OBJS)
	$(LD) -o $(EXEC) $(LDFLAGS) $(OBJFILES) $(LIBS)
	[ -z "$(STRIP)" ] || strip $(EXEC)

# Export selftest.c and selftest.h to the directory stored in EXPORT_DIR. Used 
# by `make export`.
$(EXPORT_DIR)/%: FORCE version.h
	mkdir -p $(EXPORT_DIR)
	sed -e "s/@_LST_DATE_@/$(EXPORT_DATE)/g;" \
	    -e "s/@_LST_VERSION_@/$(EXPORT_VERSION)/g;" \
	<$* >$(EXPORT_DIR)/$*.tmp
	mv $(EXPORT_DIR)/$*.tmp $(EXPORT_DIR)/$*

# Create an HTML version of the man page for the executable defined by EXEC and 
# store it in the file defined by HTMLFILE. Used by `make html`.
$(HTMLFILE): $(MANPAGE)
	man -Thtml ./$(MANPAGE) >$@.tmp
	mv $@.tmp $@

# Create the man page for the program.
$(MANPAGE): $(MANSRC) version.h
	sed "s/RPL_DATE/$(MAN_DATE)/g; s/RPL_VERSION/$(MAN_VERSION)/g;" \
	<$(MANSRC) >$@.tmp
	mv $@.tmp $@

# Create a PDF version of the man page.
$(PDFFILE): $(MANPAGE)
	man -Tpdf ./$(MANPAGE) >$@.tmp
	mv $@.tmp $@

# Install the executable in `$(PREFIX)/bin`.
$(PREFIX)/bin/$(EXEC): $(EXEC)
	mkdir -p $(PREFIX)/bin
	install $(EXEC) $(PREFIX)/bin/$(EXEC)

# Install the man page in `$(PREFIX)/share/man/man1`.
$(PREFIX)/share/man/man1/$(MANPAGE): $(MANPAGE)
	mkdir -p $(PREFIX)/share/man/man1
	install $(MANPAGE) $(PREFIX)/share/man/man1/$(MANPAGE)

# Generate version.h, which defines the version and date strings and any 
# compile-time macros enabled on the command line, like NDEBUG.
version.h: Gen-version $(CFILES) $(HFILES) Makefile
	./Gen-version

lst-demo.o: lst-demo.c $(DEPS)
	$(CC) $(CFLAGS) lst-demo.c

selftest.o: selftest.c $(DEPS)
	$(CC) $(CFLAGS) selftest.c

# Create a `tags` file, used for source navigation by many text editors.
tags: $(CFILES) $(HFILES)
	ctags $(CFILES) $(HFILES)

tests.o: tests.c $(DEPS)
	$(CC) $(CFLAGS) tests.c

# asm: Generate assembly code for all files in CFILES, stored as `.s` files.
.PHONY: asm
asm: $(DEPS)
	$(CC) $(CFLAGS) -fverbose-asm -S $(CFILES)

# cflags: Print the list of compiler options and warning flags currently used.
.PHONY: cflags
cflags:
	@echo $(CFLAGS)

# clean: Delete all generated files except `tags`.
.PHONY: clean
clean:
	rm -f $(EXEC) $(OBJS)
	rm -f $(EXEC).core
	rm -f $(HTMLFILE) $(HTMLFILE).tmp
	rm -f $(MANPAGE) $(MANPAGE).tmp
	rm -f $(PDFFILE) $(PDFFILE).tmp
	rm -f *.gcda *.gcno *.gcov
	rm -f *.s
	rm -f gmon.out gmon.sum
	rm -f version.h
	rm -fr $(EXPORT_DIR)/selftest.c $(EXPORT_DIR)/selftest.h
	rm -fr .$(EXEC)-test.tmp
	rmdir $(EXPORT_DIR) 2>/dev/null || true

# dupdesc: Search for and print duplicated test descriptions. All test 
# descriptions should be unique to make it easier to track down failing tests 
# and to avoid ambiguous matches when grepping.
.PHONY: dupdesc
dupdesc:
	@$(MAKE) -s clean all \
	&& ./$(EXEC) --selftest | grep -v '^#' | cut -f 4- -d ' ' | sort \
	   | uniq -c | sed 's/^[[:space:]]*//g;' | grep -v '^1 ' \
	   | grep . && exit 1 || true

# edit: Edit all files tracked by Git in the current directory tree with 
# EDITOR.
.PHONY: edit
edit: tags
	$(EDITOR) $$(git ls-files)
	rm tags

# export: Create copies of selftest.c and selftest.h in an `export` directory 
# and insert the current library version and date (including the Git commit 
# hash) into the files. This makes it easy to identify which version was 
# installed. To specify a custom directory, use the EXPORT_DIR macro.
.PHONY: export
export:
	$(MAKE) -s $(EXPORT_DIR)/selftest.c
	$(MAKE) -s $(EXPORT_DIR)/selftest.h

# FORCE: Empty target used as a prerequisite to force a rule to always run.
.PHONY: FORCE
FORCE:

# gcov: Generate test coverage with `gcov`(1).
.PHONY: gcov
gcov:
	$(MAKE) -s clean test GCOV=1 DEVEL=1
	gcov $(CFILES)
	@echo $$(grep -E '^ +#####:' *.c.gcov | wc -l) untested lines >&2
	@echo $$(grep -E '^ +#####:' *.c.gcov | grep -Fi '/* gncov */' \
	         | wc -l) gncov lines >&2
	@echo $(TESTABLE_LINES) testable lines >&2

# gcov-cmt: Add `gcov` markers in the source code to lines that are not tested. 
# Lines that are hard to test, for example full disk, full memory, long paths 
# and so on, can be marked with the string `/* gncov */` to avoid marking them. 
# Set GNCOV to a non-empty value to mark `gncov` lines too.
.PHONY: gcov-cmt
gcov-cmt: gcov
	gcov-cmt $(GNCOV_STR) $(CFILES)

# gcov-cmt-clean: Delete `gcov` markers generated by `gcov-cmt` from the source 
# code.
.PHONY: gcov-cmt-clean
gcov-cmt-clean:
	gcov-cmt -d $(CFILES)

# gdb: Start gdb(1) with a breakpoint at main(), as defined in `gdbrc`. Extra 
# gdb options can be put in the file `gdbopts`. An example would be "-tty 
# /dev/[...]" to send the program output to another window.
.PHONY: gdb
gdb: $(EXEC)
	gdb -x gdbrc $$(cat gdbopts 2>/dev/null) $(EXEC)

# gncov-refresh: Remove `gncov` markers from lines that no longer need to be 
# excluded from coverage testing. This might be necessary if code changes or 
# compiler optimizations make previously untestable lines testable. For 
# example, if a function is inlined or optimized differently, lines that were 
# once skipped might now be covered by tests.
.PHONY: gncov-refresh
gncov-refresh:
	$(MAKE) -s gncov-remove
	$(MAKE) -s gcov-cmt DEVEL=1
	perl -pi -e 's, /\* gcov \*/, /\* gncov \*/,g;' $(CFILES)

# gncov-remove: Delete all `gncov` markers from the source files.
.PHONY: gncov-remove
gncov-remove:
	perl -pi -e 's, /\* gncov \*/,,gi;' $(CFILES)

# html: Create an HTML version of the man page.
.PHONY: html
html: $(HTMLFILE)

# install: Install `lst-demo` and the man page to the location defined by 
# PREFIX. Default location is `/usr/local`, but it can be installed to another 
# directory tree by specifying PREFIX.
.PHONY: install
install: $(INSTALL_TARGETS)

# longlines: Warn about lines longer than 79 characters in the files defined by 
# LONGLINES_FILES.
.PHONY: longlines
longlines:
	@for f in $(LONGLINES_FILES); do \
		if [ -f "$$f" ]; then \
			d="$$(expand "$$f" | sed 's/ $$//;' \
			      | perl -C3 -ne \
			        'chomp; \
			        print("$$.:$$_\n") if length > 79;')"; \
			echo "$$d" | grep -q . \
			&& (echo "$$f"; printf "%s\\n" "$$d"; echo " "); \
		fi; \
	done | grep . && exit 1 || true

# pdf: Create a PDF version of the man page.
.PHONY: pdf
pdf: $(PDFFILE)

# test: Run the test suite. The TESTS macro can have the following values:
# - `all` (default, runs both function and executable tests)
# - `func` (only function tests are executed)
# - `exec` (only executable tests using the binary are executed)
# - `exec,func` (same as `all`)
# - `none` (no tests are run)
.PHONY: test
test: $(EXEC)
	./$(EXEC) --selftest $(TESTS)

# testall: Run the test suite with several combinations of compile-time macros 
# (NDEBUG, NO_SELFTEST) and check for various source code errors.
.PHONY: testall
testall:
	$(MAKE) -s testcomb WHAT=test
	$(MAKE) -s testsrc

# testcomb: Run the test suite with various combinations of compile-time macros 
# (NDEBUG, NO_SELFTEST).
.PHONY: testcomb
testcomb:
	$(MAKE) clean $(WHAT) NDEBUG=1
	$(MAKE) clean $(WHAT) NO_SELFTEST=1
	$(MAKE) clean $(WHAT) NDEBUG=1 NO_SELFTEST=1
	$(MAKE) clean $(WHAT)

# testsrc: Run various checks on the source files to make sure they're in good 
# shape.
.PHONY: testsrc
testsrc:
	@echo Check files for long lines
	@$(MAKE) -s longlines
	@echo Check for duplicated test descriptions
	@$(MAKE) -s dupdesc
	@echo "Search for failed() in selftest.c"
	@grep -n '\<failed(' --color=always selftest.c && exit 1 || true
	@echo "Search for failed() in tests.c"
	@grep -n '\<failed(' --color=always tests.c && exit 1 || true

# tlok: Generate a short status report on stdout using the format `X:Y` where X 
# is the number of untested lines (`gncov` lines are not counted) and Y is the 
# number of tests executed by `make test`. This is the marker found in some 
# commit subject lines to keep track of the statistics.
.PHONY: tlok
tlok:
	@$(MAKE) -s gcov DEVEL=1 2>.make-tlok.tmp >&2; res=$$?; \
	if test $$res -eq 0; then \
		printf '%u:%u\n' \
		       "$(TESTABLE_LINES)" \
		       "$$(grep ^1\\.\\. .make-tlok.tmp \
		           | perl -pe 's/^1\.\.(\d+).*/$$1/;')"; \
	else \
		grep "Looks like you failed" <.make-tlok.tmp \
		| perl -p \
		  -e  's/.*failed (\d+) tests? of (\d+).*/$$1\/$$2 FAIL/;'; \
	fi; \
	[ -e $(EXEC) ] || echo FAIL; \
	rm .make-tlok.tmp

# tlokall: Print `tlok` status for function and executable tests on stdout.
.PHONY: tlokall
tlokall:
	@echo -n   "tlok TESTS=all       : "
	@$(MAKE) -s tlok TESTS=all
	@echo -n   "tlok TESTS=func      : "
	@$(MAKE) -s tlok TESTS=func
	@echo -n   "tlok TESTS=exec      : "
	@$(MAKE) -s tlok TESTS=exec
	@echo -n   "tlok TESTS=func,exec : "
	@$(MAKE) -s tlok TESTS=func,exec
	@echo -n   "tlok TESTS=none      : "
	@$(MAKE) -s tlok TESTS=none

# uninstall: Delete the installed version from the directory tree specified by 
# PREFIX.
.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_TARGETS)

# valgrind: Run all tests with Valgrind to find memory leaks and other 
# problems.
.PHONY: valgrind
valgrind: $(EXEC)
	./$(EXEC) --valgrind $(TESTS)
	$(CMD_VALGRIND) ./$(EXEC) --selftest $(TESTS)

# valgrindall: Run the test suite with several combinations of compile-time 
# macros (NDEBUG, NO_SELFTEST) and use Valgrind to check for memory leaks and 
# other problems.
.PHONY: valgrindall
valgrindall:
	$(MAKE) -s testcomb WHAT=valgrind
