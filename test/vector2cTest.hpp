#include <cxxtest/TestSuite.h>
#include <typeinfo>
#include <complex>
#include "dicek/vector.hpp"

class vector2cTest : public CxxTest::TestSuite {
public:
    using complexf = std::complex<float>;
    using vector2c = dicek::math::vector<3, complexf>;

    void testDIM( void )
    {
        static_assert( vector2c::DIM == 3, "vector<3,complexf>::DIM is not 3" );
        TSM_ASSERT_EQUALS( "vector<3,complexf>::DIM is not 3", 3, ( vector2c::DIM ) );
    }

    void test_scalar_type( void )
    {
        static_assert( std::is_same< vector2c::scalar_type, complexf>::value, "vector2c::scalar_type is not complexf" );
        TSM_ASSERT( "vector2c::scalar_type is not complexf", typeid( vector2c::scalar_type ) == typeid( complexf ) );
    }

    void test_default_constructor( void )
    {
        vector2c v;
        TSM_ASSERT_EQUALS( "Not zero", v[ 0 ], complexf() );
        TSM_ASSERT_EQUALS( "Not zero", v[ 1 ], complexf() );
        TSM_ASSERT_EQUALS( "Not zero", v[ 2 ], complexf() );
    }

    void test_at( void )
    {
        vector2c v;

        v.at( 0 ) = complexf( 5, 1 );
        v.at( 1 ) = complexf( 7, 2 );
        v.at( 2 ) = complexf( 11, 3 );

        TSM_ASSERT_EQUALS( "at(0) is not (5, 1)", v.at( 0 ), complexf( 5, 1 ) );
        TSM_ASSERT_EQUALS( "at(1) is not (7, 2)", v.at( 1 ), complexf( 7, 2 ) );
        TSM_ASSERT_EQUALS( "at(2) is not (11, 3)", v.at( 2 ), complexf( 11, 3 ) );
    }

    void test_at_const( void )
    {
        vector2c v;

        v.at( 0 ) = complexf( 5, 1 );
        v.at( 1 ) = complexf( 7, 2 );
        v.at( 2 ) = complexf( 11, 3 );

        const auto& ref = v;

        TSM_ASSERT_EQUALS( "at(0) is not (5, 1)", v.at( 0 ), complexf( 5, 1 ) );
        TSM_ASSERT_EQUALS( "at(1) is not (7, 2)", v.at( 1 ), complexf( 7, 2 ) );
        TSM_ASSERT_EQUALS( "at(2) is not (11, 3)", v.at( 2 ), complexf( 11, 3 ) );
    }

    void test_at_throw( void )
    {
        vector2c v;
        TSM_ASSERT_THROWS( "Exception not thrown", v.at( 3 ), std::out_of_range );
        TSM_ASSERT_THROWS( "Exception not thrown", const_cast<const decltype( v )&>( v ).at( 3 ), std::out_of_range );
    }

    //void test_operator_square_bracket( void )
    //{
    //    vector2c v;
    //    v[ 0 ] = 5;
    //    v[ 1 ] = 7;
    //    v[ 2 ] = 11;

    //    TSM_ASSERT_EQUALS( "operator[](0) is not 5", v[ 0 ], 5 );
    //    TSM_ASSERT_EQUALS( "operator[](1) is not 7", v[ 1 ], 7 );
    //    TSM_ASSERT_EQUALS( "operator[](2) is not 11", v[ 2 ], 11 );
    //}

    //void test_operator_square_bracket_const( void )
    //{
    //    vector2c v;
    //    v[ 0 ] = 13;
    //    v[ 1 ] = 17;
    //    v[ 2 ] = 19;

    //    const auto& ref = v;

    //    TSM_ASSERT_EQUALS( "operator[](0) is not 13", ref[ 0 ], 13 );
    //    TSM_ASSERT_EQUALS( "operator[](1) is not 17", ref[ 1 ], 17 );
    //    TSM_ASSERT_EQUALS( "operator[](2) is not 19", ref[ 2 ], 19 );
    //}

    //void test_operator_add( void )
    //{
    //    vector2c v, w;
    //    v[ 0 ] = 1;
    //    v[ 1 ] = 2;
    //    v[ 2 ] = 3;

    //    w[ 0 ] = 10;
    //    w[ 1 ] = 20;
    //    w[ 2 ] = 30;

    //    const auto& _v = v;
    //    const auto& _w = w;

    //    vector2c x = _v + _w;
    //    vector2c y = _w + _v;

    //    TSM_ASSERT_EQUALS( "operator+ is not 11", x[ 0 ], 11 );
    //    TSM_ASSERT_EQUALS( "operator+ is not 22", x[ 1 ], 22 );
    //    TSM_ASSERT_EQUALS( "operator+ is not 33", x[ 2 ], 33 );

    //    TSM_ASSERT_EQUALS( "operator+ is not 11", y[ 0 ], x[ 0 ] );
    //    TSM_ASSERT_EQUALS( "operator+ is not 22", y[ 1 ], x[ 1 ] );
    //    TSM_ASSERT_EQUALS( "operator+ is not 33", y[ 2 ], x[ 2 ] );
    //}

    //void test_scale( void )
    //{
    //    vector2c v;
    //    v[ 0 ] = 1;
    //    v[ 1 ] = 2;
    //    v[ 2 ] = 3;

    //    vector2c w = v.scale( 3 );
    //    TS_ASSERT_EQUALS( w[ 0 ], 3 );
    //    TS_ASSERT_EQUALS( w[ 1 ], 6 );
    //    TS_ASSERT_EQUALS( w[ 2 ], 9 );
    //}

    //void test_map( void )
    //{
    //    vector2c v;
    //    v[ 0 ] = 1;
    //    v[ 1 ] = 2;
    //    v[ 2 ] = 3;

    //    const auto& _v = v;

    //    vector2c w = _v.map( []( complexf val ) { return val*val; } );
    //    TS_ASSERT_EQUALS( w[ 0 ], 1 );
    //    TS_ASSERT_EQUALS( w[ 1 ], 4 );
    //    TS_ASSERT_EQUALS( w[ 2 ], 9 );

    //    vector2c x = _v.map( []( complexf val ) { return sin( val ); } );
    //    TS_ASSERT_EQUALS( x[ 0 ], sin( 1.0 ) );
    //    TS_ASSERT_EQUALS( x[ 1 ], sin( 2.0 ) );
    //    TS_ASSERT_EQUALS( x[ 2 ], sin( 3.0 ) );
    //}

    //void test_inner_product( void )
    //{
    //    vector2c v, w;
    //    v[ 0 ] = 1;
    //    v[ 1 ] = 2;
    //    v[ 2 ] = 3;

    //    w[ 0 ] = 10;
    //    w[ 1 ] = 20;
    //    w[ 2 ] = 30;

    //    const auto& _v = v;
    //    const auto& _w = w;

    //    vector2c::scalar_type x = inner_product( _v, _w );

    //    TS_ASSERT_EQUALS( x, 1 * 10 + 2 * 20 + 3 * 30 );
    //}
};
