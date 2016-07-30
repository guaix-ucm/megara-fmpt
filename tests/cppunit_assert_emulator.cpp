//#include "cppunit_assert_emulator.h"

bool success;
void CPPUNIT_ASSERT(bool b)
{
    success = b;
}
