lib_LTLIBRARIES = libfmtp.la
libfmtp_la_SOURCES = vclemu.cpp \
    Exceptions.cpp \
    Vectors.cpp \
    Constants.cpp \
    Scalars.cpp \
    Geometry.cpp \
    Strings.cpp \
    StrPR.cpp \
    StrSimbolic.cpp \
    TextFile.cpp \
    ItemsList.cpp \
    SlideArray.cpp \
    Vector.cpp \
    PointersList.cpp \
    PointersSlideArray.cpp \
    Quantificator.cpp \
    Function.cpp \
    ComposedMotionFunction.cpp \
    RampFunction.cpp \
    SquareFunction.cpp \
    MotionFunction.cpp \
    Figure.cpp \
    Barrier.cpp \
    ContourFigureList.cpp \
    Arm.cpp \
    PairPositionAngles.cpp \
    Cilinder.cpp \
    Actuator.cpp \
    RoboticPositioner.cpp \
    ExclusionArea.cpp \
    Instruction.cpp \
    RoboticPositionerList1.cpp \
    RoboticPositionerList2.cpp \
    RoboticPositionerList3.cpp \
    ExclusionAreaList.cpp \
    FiberConnectionModel.cpp \
    FiberMOSModel1.cpp \
    FiberMOSModel2.cpp \
    FileMethods.cpp \
    MessageInstruction.cpp \
    SkyPoint.cpp \
    ProjectionPoint.cpp \
    Tile.cpp \
    TelescopeProjectionModel.cpp \
    MotionProgram.cpp \
    TargetPoint.cpp \
    TargetPointList.cpp \
    MotionProgramValidator.cpp \
    MotionProgramGenerator.cpp \
    vclemu.h \
    Exceptions.h \
    Vectors.h \
    Constants.h \
    Scalars.h \
    Geometry.h \
    Strings.h \
    StrPR.h \
    StrSimbolic.h \
    TextFile.h \
    ItemsList.h \
    SlideArray.h \
    Vector.h \
    PointersList.h \
    PointersSlideArray.h \
    Quantificator.h \
    Function.h \
    ComposedMotionFunction.h \
    RampFunction.h \
    SquareFunction.h \
    MotionFunction.h \
    Figure.h \
    Barrier.h \
    ContourFigureList.h \
    FiberMOSModelConstants.h \
    Arm.h \
    PairPositionAngles.h \
    Cilinder.h \
    Actuator.h \
    RoboticPositioner.h \
    ExclusionArea.h \
    Instruction.h \
    RoboticPositionerList1.h \
    RoboticPositionerList2.h \
    RoboticPositionerList3.h \
    ExclusionAreaList.h \
    FiberConnectionModel.h \
    FiberMOSModel1.h \
    FiberMOSModel2.h \
    FileMethods.h \
    MessageInstruction.h \
    SkyPoint.h \
    ProjectionPoint.h \
    Tile.h \
    TelescopeProjectionModel.h \
    MotionProgram.h \
    TargetPoint.h \
    TargetPointList.h \
    MotionProgramValidator.h \
    MotionProgramGenerator.h

## Version info is in current : revision : age form
## A library supports interfaces from current downto current - age
## Revision is the version of the current interface

## If the source code has changed, rev += 1

libfmtp_la_LDFLAGS = -version-info 0:0:0

pkginclude_HEADERS = vclemu.h \
    Exceptions.h \
    Vectors.h \
    Constants.h \
    Scalars.h \
    Geometry.h \
    Strings.h \
    StrPR.h \
    StrSimbolic.h \
    TextFile.h \
    ItemsList.h \
    SlideArray.h \
    Vector.h \
    PointersList.h \
    PointersSlideArray.h \
    Quantificator.h \
    Function.h \
    ComposedMotionFunction.h \
    RampFunction.h \
    SquareFunction.h \
    MotionFunction.h \
    Figure.h \
    Barrier.h \
    ContourFigureList.h \
    FiberMOSModelConstants.h \
    Arm.h \
    PairPositionAngles.h \
    Cilinder.h \
    Actuator.h \
    RoboticPositioner.h \
    ExclusionArea.h \
    Instruction.h \
    RoboticPositionerList1.h \
    RoboticPositionerList2.h \
    RoboticPositionerList3.h \
    ExclusionAreaList.h \
    FiberConnectionModel.h \
    FiberMOSModel1.h \
    FiberMOSModel2.h \
    FileMethods.h \
    MessageInstruction.h \
    SkyPoint.h \
    ProjectionPoint.h \
    Tile.h \
    TelescopeProjectionModel.h \
    MotionProgram.h \
    TargetPoint.h \
    TargetPointList.h \
    MotionProgramValidator.h \
    MotionProgramGenerator.h

bin_PROGRAMS = fmpt_saa
fmpt_saa_SOURCES = main.cpp
fmpt_saa_LDADD = libfmtp.la
fmpt_saa_CPPFLAGS = $(AM_CPPFLAGS)

AM_CPPFLAGS = -I$(top_srcdir) -DDATADIR=\"$(pkgdatadir)\"

