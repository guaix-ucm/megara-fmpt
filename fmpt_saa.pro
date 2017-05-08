#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T21:10:21
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

##CONFIG += c++11 ## esto hace que funcione stod (incluyendo string o iostream), pero impide que funcione typeid (incluyendo typeinfo)

LIBS += /usr/lib/x86_64-linux-gnu/libuuid.so
LIBS += /usr/lib/x86_64-linux-gnu/libjsoncpp.so

TARGET = FMPT_SAA
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    src/VCLemu.cpp \
    src/Exceptions.cpp \
    #---------------------------------------------------------
    src/Constants.cpp \
    src/Scalars.cpp \
    src/Vectors.cpp \
    src/Geometry.cpp \
    #---------------------------------------------------------
    src/Strings.cpp \
    src/StrPR.cpp \
    src/TextFile.cpp \
    #---------------------------------------------------------
    src/SlideArray.cpp \
    src/PointersSlideArray.cpp \
    src/ItemsList.cpp \
    src/PointersList.cpp \
    src/Vector.cpp \
    #---------------------------------------------------------
    src/Quantificator.cpp \
    src/Function.cpp \
    src/MotionFunction.cpp \
    src/SquareFunction.cpp \
    src/RampFunction.cpp \
    src/ComposedMotionFunction.cpp \
    #---------------------------------------------------------
    src/Figure.cpp \
    src/ContourFigureList.cpp \
    #---------------------------------------------------------
    src/Barrier.cpp \
    src/adjacentitem.cpp \
    src/ExclusionArea.cpp \
    src/ExclusionAreaList.cpp \
    #---------------------------------------------------------
    src/Instruction.cpp \
    src/MessageInstruction.cpp \
    src/PairPositionAngles.cpp \
    src/MotionProgram.cpp \
    #---------------------------------------------------------
    src/Arm.cpp \
    src/Cilinder.cpp \
    src/Actuator.cpp \
    src/RoboticPositioner.cpp \
    src/RoboticPositionerList1.cpp \
    src/RoboticPositionerList2.cpp \
    src/RoboticPositionerList3.cpp \
    #---------------------------------------------------------
    src/FiberMOSModel.cpp \
    src/FiberConnectionModel.cpp \
    src/roboticpositionertuner.cpp \
    #---------------------------------------------------------
    src/SkyPoint.cpp \
    src/ProjectionPoint.cpp \
    src/Tile.cpp \
    src/TelescopeProjectionModel.cpp \
    #---------------------------------------------------------
    src/Allocation.cpp \
    src/AllocationList.cpp \
    src/MotionProgramValidator.cpp \
    src/MotionProgramGenerator.cpp \
    #---------------------------------------------------------
    src/PositionerCenter.cpp \
    src/FileMethods.cpp \
    src/FMOSA.cpp \
    src/OutputsParkProg.cpp \
    src/OutputsPairPPDP.cpp \
    #---------------------------------------------------------
    src/main.cpp

HEADERS += \
    src/VCLemu.h \
    src/Exceptions.h \
    #---------------------------------------------------------
    src/Constants.h \
    src/Scalars.h \
    src/Vectors.h \
    src/Geometry.h \
    #---------------------------------------------------------
    src/Strings.h \
    src/StrPR.h \
    src/TextFile.h \
    #---------------------------------------------------------
    src/SlideArray.h \
    src/PointersSlideArray.h \
    src/PointersList.h \
    src/ItemsList.h \
    src/Vector.h \
    #---------------------------------------------------------
    src/Quantificator.h \
    src/Function.h \
    src/MotionFunction.h \
    src/SquareFunction.h \
    src/RampFunction.h \
    src/ComposedMotionFunction.h \
    #---------------------------------------------------------
    src/Figure.h \
    src/ContourFigureList.h \
    #--------------------------------------------------------
    src/FiberMOSModelConstants.h \
    #---------------------------------------------------------
    src/Barrier.h \
    src/adjacentitem.h \
    src/ExclusionArea.h \
    src/ExclusionAreaList.h \
    #---------------------------------------------------------
    src/Instruction.h \
    src/MessageInstruction.h \
    src/PairPositionAngles.h \
    src/MotionProgram.h \
    #---------------------------------------------------------
    src/Arm.h \
    src/Cilinder.h \
    src/Actuator.h \
    src/RoboticPositioner.h \
    src/RoboticPositionerList1.h \
    src/RoboticPositionerList2.h \
    src/RoboticPositionerList3.h \
    #---------------------------------------------------------
    src/FiberMOSModel.h \
    src/FiberConnectionModel.h \
    src/roboticpositionertuner.h \
    #--------------------------------------------------------
    src/SkyPoint.h \
    src/ProjectionPoint.h \
    src/Tile.h \
    src/TelescopeProjectionModel.h \
    #---------------------------------------------------------
    src/Allocation.h \
    src/AllocationList.h \
    src/MotionProgramValidator.h \
    src/MotionProgramGenerator.h \
    #---------------------------------------------------------
    src/PositionerCenter.h \
    src/FileMethods.h \
    src/FMOSA.h \
    src/OutputsParkProg.h \
    src/OutputsPairPPDP.h \
    #----------------------------------------------
    src/globalconsts.h
