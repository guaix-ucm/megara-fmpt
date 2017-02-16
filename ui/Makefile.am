lib_LTLIBRARIES = libfmpt.la
libfmpt_la_SOURCES = PloterShapes.cpp \
    GenerateFrames.cpp \
    mainWindow.cpp \
    PloterShapes.h \
    GenerateFrames.h \
    mainWindow.h

## Version info is in current : revision : age form
## A library supports interfaces from current downto current - age
## Revision is the version of the current interface

## If the source code has changed, rev += 1

libfmpt_la_LDFLAGS = -version-info 0:0:0

pkginclude_HEADERS = PloterShapes.h \
    GenerateFrames.h \
    mainWindow.h

bin_PROGRAMS = fmpt_saa
fmpt_saa_SOURCES = main.cpp
fmpt_saa_LDADD = libfmpt.la
fmpt_saa_CPPFLAGS = $(AM_CPPFLAGS)

AM_CPPFLAGS = -I$(top_srcdir) -DDATADIR=\"$(pkgdatadir)\"

