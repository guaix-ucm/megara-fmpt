
AC_INIT([megara-fmpt],[1.0.0])
AM_INIT_AUTOMAKE([foreign dist-xz])
LT_INIT

AC_PROG_CXX

PKG_CHECK_MODULES([CPPUNIT], [cppunit], [testen=1], [
AC_MSG_WARN([Cppunit is needed by the tests. Checking is disabled])
 testen=0])
AM_CONDITIONAL([TESTS_ENABLED], [test x$testen = x1])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_FILES([Makefile
                 data/Makefile
                 tests/Makefile
                 src/Makefile])
AC_OUTPUT
