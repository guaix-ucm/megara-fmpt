#Take care executing autoclear.sh and autogen.sh.

#Execute autogen.sh may produce changes in the folloowing files:
#	aclocal.m4
#	config.h.in
#	configure
#	Makefile.in

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

#! /bin/sh

autoreconf -s -i -m -f  
#-I /usr/share/aclocal
