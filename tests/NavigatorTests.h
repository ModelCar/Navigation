// MyTestSuite1.h
#include "cxxtest/TestSuite.h"

#include "navigator.h"

class MyTestSuite1 : public CxxTest::TestSuite
{
public:
    void testAddition(void)
    {
        Navigator nav = new Navigator();
        TS_ASSERT_EQUALS(nav.instructions["TURN_SHARP_LEFT"], -3);
    }
};