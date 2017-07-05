dnl Process this file with autoconf to produce a configure script.
AC_INIT([PHOEG],[0.0.1],[hadrien.melot@umons.ac.be],[phoeg])
AM_INIT_AUTOMAKE

dnl Include some macros.
AC_CONFIG_MACRO_DIR([m4])

dnl AC_TRY_CFLAGS (CFLAGS, [ACTION-IF-WORKS], [ACTION-IF-FAILS])
dnl Check if $CC supports a given set of cflags
AC_DEFUN([AC_TRY_CFLAGS],
    [AC_MSG_CHECKING([if $CC supports $1 flags])
    SAVE_CFLAGS="$CFLAGS"
    CFLAGS="$1"
    AC_RUN_IFELSE([AC_LANG_SOURCE([[main(){exit(0);}]])],[ac_cv_try_cflags_ok=yes],[ac_cv_try_cflags_ok=no],[ac_cv_try_cflags_ok=no])
    CFLAGS="$SAVE_CFLAGS"
    AC_MSG_RESULT([$ac_cv_try_cflags_ok])
    if test x"$ac_cv_try_cflags_ok" = x"yes"; then
        ifelse([$2],[],[:],[$2])
    else
        ifelse([$3],[],[:],[$3])
    fi])

dnl AC_TRY_CXXFLAGS (CXXFLAGS, [ACTION-IF-WORKS], [ACTION-IF-FAILS])
dnl Check if $CXX supports a given set of cxxflags
AC_DEFUN([AC_TRY_CXXFLAGS],
    [AC_MSG_CHECKING([if $CXX supports $1 flags])
    SAVE_CXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$1"
    AC_RUN_IFELSE([AC_LANG_SOURCE([[main(){exit(0);}]])],[ac_cv_try_cxxflags_ok=yes],[ac_cv_try_cxxflags_ok=no],[ac_cv_try_cxxflags_ok=no])
    CXXFLAGS="$SAVE_CXXFLAGS"
    AC_MSG_RESULT([$ac_cv_try_cxxflags_ok])
    if test x"$ac_cv_try_cxxflags_ok" = x"yes"; then
        ifelse([$2],[],[:],[$2])
    else
        ifelse([$3],[],[:],[$3])
    fi])

dnl CVT_YESNO([YES-NO VARIABLE],[0-1 VARIABLE])
dnl Make a 0-1 output variable from a yes/no shell variable
AC_DEFUN([CVT_YESNO],[if test x"$$1" = x"yes"; then
$2=1
else
$2=0
fi
AC_SUBST($2)])


dnl Checks for system features
AC_CANONICAL_HOST


dnl Checks for compilers

user_cflags="$CFLAGS"
user_cxxflags="$CXXFLAGS"
AC_PROG_CC
CFLAGS=$user_cflags
AC_PROG_CXX
CXXFLAGS=$user_cxxflags
AC_PROG_RANLIB
AM_PROG_LEX
AC_PROG_YACC

dnl For configuration header
AC_CONFIG_HEADER([src/config.h])



dnl Check for needed programs (if not present, configure stop)

dnl Checks for BOOST library
AX_BOOST_BASE([1.58],,
  [AC_MSG_ERROR([This program needs Boost,
                 but it was not found in your system])])


dnl Checks for C compiler and sets CFLAGS if not set by user


if test x"$user_cflags" = x"" ; then
  AC_TRY_CFLAGS([-O4],[CFLAGS="$CFLAGS -O4"]);
  if test x"$ac_cv_try_cflags_ok" = x"no"; then
     AC_TRY_CFLAGS([-O3],[CFLAGS="$CFLAGS -O3"]);
     if test x"$ac_cv_try_cflags_ok" = x"no"; then
         AC_TRY_CFLAGS([-O2],[CFLAGS="$CFLAGS -O2"],[CFLAGS="$CFLAGS -O"]);
     fi
  fi
fi

echo CFLAGS=$CFLAGS

dnl Checks for CXX compiler and sets CXXFLAGS if not set by user


if test x"$user_cxxflags" = x"" ; then
  AC_TRY_CXXFLAGS([-O4],[CXXFLAGS="$CXXFLAGS -O4"]);
  if test x"$ac_cv_try_cxxflags_ok" = x"no"; then
     AC_TRY_CXXFLAGS([-O3],[CXXFLAGS="$CXXFLAGS -O3"]);
     if test x"$ac_cv_try_cxxflags_ok" = x"no"; then
         AC_TRY_CXXFLAGS([-O2],[CXXFLAGS="$CXXFLAGS -O2"],[CXXFLAGS="$CXXFLAGS -O"]);
     fi
  fi
fi

echo CXXFLAGS=$CXXFLAGS


dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(stddef.h unistd.h sys/types.h stdlib.h string.h errno.h)
CVT_YESNO(ac_cv_header_stddef_h,header_stddef_h)
CVT_YESNO(ac_cv_header_unistd_h,header_unistd_h)
CVT_YESNO(ac_cv_header_stdlib_h,header_stdlib_h)
CVT_YESNO(ac_cv_header_string_h,header_string_h)
CVT_YESNO(ac_cv_header_signal_h,header_signal_h)
CVT_YESNO(ac_cv_header_sys_wait_h,header_sys_wait_h)
CVT_YESNO(ac_cv_header_sys_types_h,header_sys_types_h)
CVT_YESNO(ac_cv_header_errno_h,header_errno_h)

AC_TYPE_PID_T
CVT_YESNO(ac_cv_type_pid_t,have_pid_t)

AC_MSG_CHECKING(if malloc is declared in stdlib.h or malloc.h)
AC_EGREP_HEADER([(^|[^a-zA-Z_0-9])malloc[^a-zA-Z_0-9]],stdlib.h,
  malloc_dec=1,malloc_dec=0)
if test $malloc_dec -eq 0; then
  AC_EGREP_HEADER([(^|[^a-zA-Z_0-9])malloc[^a-zA-Z_0-9]],malloc.h,
  malloc_dec=2,malloc_dec=0)
fi
AC_SUBST(malloc_dec)
AC_MSG_RESULT($malloc_dec)

AC_MSG_CHECKING(if ftell is declared in stdio.h)
AC_EGREP_HEADER([(^|[^a-zA-Z_0-9])ftell[^a-zA-Z_0-9]],stdio.h,
  ftell_dec=1,ftell_dec=0)
AC_SUBST(ftell_dec)
AC_MSG_RESULT($ftell_dec)

AC_MSG_CHECKING(if popen is declared in stdio.h)
AC_EGREP_HEADER([(^|[^a-zA-Z_0-9])popen[^a-zA-Z_0-9]],stdio.h,
  popen_dec=1,popen_dec=0)
AC_SUBST(popen_dec)
AC_MSG_RESULT($popen_dec)

dnl Checks for sizes of integer types; avoid 64-bit if necessary
AC_CHECK_SIZEOF(int,4)
AC_SUBST(ac_cv_sizeof_int)
AC_CHECK_SIZEOF(long,4)
AC_SUBST(ac_cv_sizeof_long)
AC_CHECK_SIZEOF(long long,0)
AC_SUBST(ac_cv_sizeof_long_long)
lok=0
AC_SUBST(lok)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
CVT_YESNO(ac_cv_c_const,have_const)

dnl Checks for library functions.
AC_CHECK_FUNC(isatty,have_isatty=1,have_isatty=0)
AC_SUBST(have_isatty)
AC_CHECK_FUNC(times,have_times=1,have_times=0)
AC_SUBST(have_times)
AC_CHECK_FUNC(time,have_time=1,have_time=0)
AC_SUBST(have_time)
AC_CHECK_FUNC(gettimeofday,have_gettimeofday=1,have_gettimeofday=0)
AC_SUBST(have_gettimeofday)
AC_CHECK_FUNC(times,have_times=1,have_times=0)
AC_SUBST(have_times)
AC_CHECK_FUNC(getrusage,have_getrusage=1,have_getrusage=0)
AC_SUBST(have_getrusage)
AC_CHECK_FUNC(perror,have_perror=1,have_perror=0)
AC_SUBST(have_perror)
AC_CHECK_FUNC(pipe,have_pipe=1,have_pipe=0)
AC_SUBST(have_pipe)
AC_CHECK_FUNC(wait,have_wait=1,have_wait=0)
AC_SUBST(have_wait)
AC_CHECK_FUNC(popen,have_popen=1,have_popen=0)
AC_SUBST(have_popen)

AC_CHECK_FUNCS([floor mkdir pow sqrt strchr])

AC_CHECK_PROGS(sort_prog,gsort sort,no_sort_found)

AC_MSG_CHECKING(if INFINITY is declared in math.h)
AC_EGREP_CPP(yes,
[#include <math.h>
#ifdef INFINITY
  yes
#endif
], has_math_inf=1, has_math_inf=0)
AC_SUBST(has_math_inf)
AC_MSG_RESULT($has_math_inf)

AC_CONFIG_FILES(src/Makefile Makefile)

AC_OUTPUT
