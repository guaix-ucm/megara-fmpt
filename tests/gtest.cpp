
#include "gtest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(GTest);

void GTest::setUp() {
}

void GTest::tearDown() {
}

void GTest::testSimple() {
CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, 0.100001, 1e-3);
}
