#include <cxxtest/TestSuite.h>
#include <typeinfo>
#include "dicek/vector.hpp"

class VectorTest : public CxxTest::TestSuite {
public:
    void testInstance( void )
    {
        using namespace dicek::math;
        vector<3, double> v;
    }

    struct traits {

    };

    void testInstanceTraits( void )
    {
        using namespace dicek::math;
        vector<3, double, traits> v;
    }

    void testDIM( void )
    {
        using namespace dicek::math;
        static_assert( vector<3, double>::DIM == 3, "vector<3,double>::DIM is not 3" );
        TSM_ASSERT_EQUALS( "vector<3,double>::DIM is not 3", 3, ( vector<3, double>::DIM ) );
    }

    void test_real_type( void )
    {
        using namespace dicek::math;
        static_assert( std::is_same< vector<3, double>::real_type, double>::value, "vector<3, double>::real_type is not double" );
        TSM_ASSERT( "vector<3, double>::real_type is not double", typeid( vector<3, double>::real_type ) == typeid( double ) );
    }
};
