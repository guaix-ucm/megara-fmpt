#Take care executing autoclear.sh and autogen.sh.

#Remove the following files will not produce directly setback,
#but will produce the need for execute autogen.sh:
rm -r autom4te.cache;
rm -r m4;
rm compile;
rm config.guess;
rm config.h;
rm config.log;
rm config.status;
rm config.sub;
rm depcomp;
rm fmpt_saa.log;
rm libtool;
rm ltmain.sh;
rm Makefile;
rm data/Makefile;
rm data/Makefile.in;
rm src/Makefile;
rm src/Makefile.in;
rm missing;
rm stamp-h1;
rm test-driver;

#Remove install-sh, produces followin error:
#	"autoreconf: automake failed with exit status: 1"
#And will require execute autogen.sh twice.
rm install-sh; 

#Execute autogen.sh may produce changes in the folloowing files:
rm aclocal.m4
rm config.h.in;
rm configure;
rm Makefile.in;

#This changes can affect to the structure of directories 
#built during test execution, so it can produce errors 
#in the following tests:
#	testMotionProgramValidator
#	testPositionerCenter
#	testFileMethods

#These errors may be corrected updating the relative paths
#used in the files:
#	testMotionProgramValidator.cpp
#	testPositionerCenter.cpp
#	testFileMethods.cpp
#For example replacing "../../" by "../../../".



