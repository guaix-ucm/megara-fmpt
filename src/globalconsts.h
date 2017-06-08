#ifndef GLOBALCONSTS_H
#define GLOBALCONSTS_H

//#define QMAKE //define using qmake

//For debugging using qmake, uncomment "#define QMAKE"
//For compile using autotools, comment "#define QMAKE".

#ifdef QMAKE //if using qmake
#define DATADIR string(getCurrentDir()+"/data")
#endif

//In autotools over Ubuntu:
//  DATADIR "/urs/local/share/megara-fmpt"
//In qmake we need define DATADIR.

#endif // GLOBALCONSTS_H
