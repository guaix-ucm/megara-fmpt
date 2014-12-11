lib_LTLIBRARIES = libfmtp.la
libfmtp_la_SOURCES = Actuator.cpp Actuator.h Arm.cpp Arm.h Barrier.cpp Barrier.h Cilinder.cpp \
Cilinder.h ComposedMotionFunction.cpp ComposedMotionFunction.h Constants.cpp \
Constants.h ContourFigureList.cpp ContourFigureList.h Exceptions.cpp Exceptions.h \
ExclusionArea.cpp ExclusionArea.h ExclusionAreaList.cpp ExclusionAreaList.h \
FiberConnectionModel.cpp FiberConnectionModel.h FiberMOSModel1.cpp \
FiberMOSModel1.h FiberMOSModel2.cpp FiberMOSModel2.h FiberMOSModelConstants.h \
Figure.cpp Figure.h FileMethods.cpp FileMethods.h Function.cpp Function.h \
Geometry.cpp Geometry.h Instruction.cpp Instruction.h ItemsList.cpp \
ItemsList.h MessageInstruction.cpp MessageInstruction.h MotionFunction.cpp \
MotionFunction.h MotionProgram.cpp MotionProgramGenerator.cpp MotionProgramGenerator.h \
MotionProgram.h PLVector.cpp PLVector.h PointersList.cpp PointersList.h \
PointersSlideArray.cpp PointersSlideArray.h Polypoint.cpp Polypoint.h \
ProjectionPoint.cpp ProjectionPoint.h Quantificator.cpp Quantificator.h \
RampFunction.cpp RampFunction.h RoboticPositioner.cpp RoboticPositioner.h \
RoboticPositionerList1.cpp RoboticPositionerList1.h RoboticPositionerList2.cpp \
RoboticPositionerList2.h RoboticPositionerList3.cpp RoboticPositionerList3.h \
Scalars.cpp Scalars.h SkyPoint.cpp SkyPoint.h SlideArray.cpp SlideArray.h \
SquareFunction.cpp SquareFunction.h Strings.cpp Strings.h StrPR.cpp StrPR.h \
StrSimbolic.cpp StrSimbolic.h TargetPoint.cpp TargetPoint.h TargetPointList.cpp \
TargetPointList.h TelescopeProjectionModel.cpp TelescopeProjectionModel.h \
TextFile.cpp TextFile.h Tile.cpp Tile.h \
vclemu.cpp vclemu.h Vector.cpp Vector.h Vectors.cpp Vectors.h


## Version info is in current : revision : age form
## A library supports interfaces from current downto current - age
## Revision is the version of the current interface

## If the source code has changed, rev += 1

libfmtp_la_LDFLAGS = -version-info 0:0:0

##pkginclude_HEADERS = Strings.h

bin_PROGRAMS = fmpa
fmpa_SOURCES = FMPA_SAA_main.cpp
fmpa_LDADD = libfmtp.la
fmpa_CPPFLAGS = $(AM_CPPFLAGS)

AM_CPPFLAGS = -I$(top_srcdir)

