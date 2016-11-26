#include <cxxtest/TestSuite.h>
#include "Vector.hpp"

class VectorTest : public CxxTest::TestSuite {
public:
    void testInstance( void )
    {
        using namespace dicek::math;
        Vector<3, double> v;
    }

    struct traits {

    };
    void testInstanceTraits( void )
    {
        using namespace dicek::math;
        Vector<3, double, traits > v;
    }
};
