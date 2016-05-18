if TESTS_ENABLED
#TESTS = main_test_vclemu main_testScalars main_testSlideArray main_testFileMethods
TESTS = main_testScalars main_testSlideArray main_testFileMethods

#check_PROGRAMS = main_test_vclemu main_testScalars main_testSlideArray main_testFileMethods
check_PROGRAMS = main_testScalars main_testSlideArray main_testFileMethods

##test1_SOURCES = test1.cpp gtest.h gtest.cpp
#main_testScalars_SOURCES = main_test_vclemu.cpp test_vclemu.h test_vclemu.cpp
main_testScalars_SOURCES = main_testScalars.cpp testScalars.h testScalars.cpp
main_testSlideArray_SOURCES = main_testSlideArray.cpp testSlideArray.h testSlideArray.cpp
main_testFileMethods_SOURCES = main_testFileMethods.cpp testFileMethods.h testFileMethods.cpp

AM_CPPFLAGS = -I$(top_srcdir) -I$(CPPUNIT_CFLAGS)
LDADD = $(top_builddir)/src/libfmtp.la $(CPPUNIT_LIBS)
else
TESTS = no_tests.sh
XFAIL_TESTS = no_tests.sh

dist_check_SCRIPTS = no_tests.sh
endif
