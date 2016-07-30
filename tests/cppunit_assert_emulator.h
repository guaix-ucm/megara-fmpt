#ifndef CPPUNIT_ASSERT_EMULATOR_H
#define CPPUNIT_ASSERT_EMULATOR_H

extern bool success;

extern void CPPUNIT_ASSERT(bool b);

//For execute in make, replace in all files:
// //#include "cppunit_assert_emulator.h"             ->  //#include "cppunit_assert_emulator.h"
// //CPPUNIT_TEST_SUITE_REGISTRATION(               ->  CPPUNIT_TEST_SUITE_REGISTRATION(
// CPPUNIT_ASSERT_                                  ->  CPPUNIT_ASSERT

//For execute in qmake, replace in all files:
// //#include "cppunit_assert_emulator.h"           ->  //#include "cppunit_assert_emulator.h"
// CPPUNIT_TEST_SUITE_REGISTRATION(TestVCLemu);     ->  //CPPUNIT_TEST_SUITE_REGISTRATION(TestVCLemu);
// CPPUNIT_ASSERT                                   ->  CPPUNIT_ASSERT_

#endif // CPPUNIT_ASSERT_EMULATOR_H
