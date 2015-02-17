if TESTS_ENABLED
TESTS = main_testScalars

check_PROGRAMS = main_testScalars

##test1_SOURCES = test1.cpp gtest.h gtest.cpp
main_testScalars_SOURCES = main_testScalars.cpp testScalars.h testScalars.cpp
#main_testFileMethods_SOURCES = main_testFileMethods.cpp testFileMethods.h testFileMethods.cpp

AM_CPPFLAGS = -I$(top_srcdir) -I$(CPPUNIT_CFLAGS)
LDADD = $(top_builddir)/src/libfmtp.la $(CPPUNIT_LIBS)
else
TESTS = no_tests.sh
XFAIL_TESTS = no_tests.sh

dist_check_SCRIPTS = no_tests.sh
endif
