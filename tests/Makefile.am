if TESTS_ENABLED
TESTS = test1

check_PROGRAMS = test1

test1_SOURCES = test1.cpp gtest.h gtest.cpp

AM_CPPFLAGS = -I$(top_srcdir) -I$(CPPUNIT_CFLAGS)
LDADD = $(top_builddir)/src/libfmtp.la $(CPPUNIT_LIBS)
else
TESTS = no_tests.sh
XFAIL_TESTS = no_tests.sh

dist_check_SCRIPTS = no_tests.sh
endif
