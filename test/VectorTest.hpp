#include <cxxtest/TestSuite.h>
#include <typeinfo>
#include "dicek/vector.hpp"

class VectorTest : public CxxTest::TestSuite {
public:
    using vector3d = dicek::math::vector<3, double>;

    void testDIM( void )
    {
        static_assert( vector3d::DIM == 3, "vector<3,double>::DIM is not 3" );
        TSM_ASSERT_EQUALS( "vector<3,double>::DIM is not 3", 3, ( vector3d::DIM ) );
    }

    void test_scalar_type( void )
    {
        static_assert( std::is_same< vector3d::scalar_type, double>::value, "vector3d::scalar_type is not double" );
        TSM_ASSERT( "vector3d::scalar_type is not double", typeid( vector3d::scalar_type ) == typeid( double ) );
    }

    void test_default_constructor( void )
    {
        vector3d v;
        TSM_ASSERT_EQUALS( "Not zero", v[ 0 ], 0 );
        TSM_ASSERT_EQUALS( "Not zero", v[ 1 ], 0 );
        TSM_ASSERT_EQUALS( "Not zero", v[ 2 ], 0 );
    }

    void test_at( void )
    {
        vector3d v;
        v.at( 0 ) = 5;
        v.at( 1 ) = 7;
        v.at( 2 ) = 11;

        TSM_ASSERT_EQUALS( "at(0) is not 5", v.at( 0 ), 5 );
        TSM_ASSERT_EQUALS( "at(1) is not 7", v.at( 1 ), 7 );
        TSM_ASSERT_EQUALS( "at(2) is not 11", v.at( 2 ), 11 );
    }

    void test_at_const( void )
    {
        vector3d v;
        v.at( 0 ) = 13;
        v.at( 1 ) = 17;
        v.at( 2 ) = 19;

        const auto& ref = v;

        TSM_ASSERT_EQUALS( "at(0) is not 13", ref.at( 0 ), 13 );
        TSM_ASSERT_EQUALS( "at(1) is not 17", ref.at( 1 ), 17 );
        TSM_ASSERT_EQUALS( "at(2) is not 19", ref.at( 2 ), 19 );
    }

    void test_at_throw( void )
    {
        vector3d v;
        TSM_ASSERT_THROWS( "Exception not thrown", v.at( 3 ), std::out_of_range );
        TSM_ASSERT_THROWS( "Exception not thrown", const_cast<const decltype( v )&>( v ).at( 3 ), std::out_of_range );
    }

    void test_operator_square_bracket( void )
    {
        vector3d v;
        v[ 0 ] = 5;
        v[ 1 ] = 7;
        v[ 2 ] = 11;

        TSM_ASSERT_EQUALS( "operator[](0) is not 5", v[ 0 ], 5 );
        TSM_ASSERT_EQUALS( "operator[](1) is not 7", v[ 1 ], 7 );
        TSM_ASSERT_EQUALS( "operator[](2) is not 11", v[ 2 ], 11 );
    }

    void test_operator_square_bracket_const( void )
    {
        vector3d v;
        v[ 0 ] = 13;
        v[ 1 ] = 17;
        v[ 2 ] = 19;

        const auto& ref = v;

        TSM_ASSERT_EQUALS( "operator[](0) is not 13", ref[ 0 ], 13 );
        TSM_ASSERT_EQUALS( "operator[](1) is not 17", ref[ 1 ], 17 );
        TSM_ASSERT_EQUALS( "operator[](2) is not 19", ref[ 2 ], 19 );
    }

    void test_operator_add( void )
    {
        vector3d v, w;
        v[ 0 ] = 1;
        v[ 1 ] = 2;
        v[ 2 ] = 3;

        w[ 0 ] = 10;
        w[ 1 ] = 20;
        w[ 2 ] = 30;

        const auto& _v = v;
        const auto& _w = w;

        vector3d x = _v + _w;
        vector3d y = _w + _v;

        TSM_ASSERT_EQUALS( "operator+ is not 11", x[ 0 ], 11 );
        TSM_ASSERT_EQUALS( "operator+ is not 22", x[ 1 ], 22 );
        TSM_ASSERT_EQUALS( "operator+ is not 33", x[ 2 ], 33 );

        TSM_ASSERT_EQUALS( "operator+ is not 11", y[ 0 ], x[ 0 ] );
        TSM_ASSERT_EQUALS( "operator+ is not 22", y[ 1 ], x[ 1 ] );
        TSM_ASSERT_EQUALS( "operator+ is not 33", y[ 2 ], x[ 2 ] );
    }

    void test_scale( void )
    {
        vector3d v;
        v[ 0 ] = 1;
        v[ 1 ] = 2;
        v[ 2 ] = 3;

        vector3d w = v.scale( 3 );
        TSM_ASSERT_EQUALS( "operator+ is not 11", w[ 0 ], 3 );
        TSM_ASSERT_EQUALS( "operator+ is not 22", w[ 1 ], 6 );
        TSM_ASSERT_EQUALS( "operator+ is not 33", w[ 2 ], 9 );
    }
};
