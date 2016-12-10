/*
MIT License

Copyright (c) 2016 Daisuke NAGAO

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef UUID_50C55706_B490_11E6_BFE5_0800274CD854
#define UUID_50C55706_B490_11E6_BFE5_0800274CD854

#include <cxxtest/TestSuite.h>
#include <typeinfo>
#include "dicek/vector.hpp"

class vector3dTest : public CxxTest::TestSuite {
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
        TS_ASSERT_EQUALS( w[ 0 ], 3 );
        TS_ASSERT_EQUALS( w[ 1 ], 6 );
        TS_ASSERT_EQUALS( w[ 2 ], 9 );
    }

    void test_map( void )
    {
        vector3d v;
        v[ 0 ] = 1;
        v[ 1 ] = 2;
        v[ 2 ] = 3;

        const auto& _v = v;

        vector3d w = _v.map( []( double val ) { return val*val; } );
        TS_ASSERT_EQUALS( w[ 0 ], 1 );
        TS_ASSERT_EQUALS( w[ 1 ], 4 );
        TS_ASSERT_EQUALS( w[ 2 ], 9 );

        vector3d x = _v.map( []( double val ) { return sin( val ); } );
        TS_ASSERT_EQUALS( x[ 0 ], sin( 1.0 ) );
        TS_ASSERT_EQUALS( x[ 1 ], sin( 2.0 ) );
        TS_ASSERT_EQUALS( x[ 2 ], sin( 3.0 ) );
    }

    void test_inner_product( void )
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

        vector3d::scalar_type x = inner_product( _v, _w );

        TS_ASSERT_EQUALS( x, 1 * 10 + 2 * 20 + 3 * 30 );
    }

    void test_norm( void )
    {
        vector3d v;
        v[ 0 ] = 1;
        v[ 1 ] = 2;
        v[ 2 ] = 3;

        double n = norm( v, 2.0 );
        TS_ASSERT_DELTA( sqrt( 1 + 4 + 9 ), n, FLT_EPSILON );

        n = norm( v, 3.0 );
        TS_ASSERT_DELTA( pow( (double)( 1 + 8 + 27 ), 1 / 3.0 ), n, FLT_EPSILON );
    }
};

#endif /* UUID_50C55706_B490_11E6_BFE5_0800274CD854 */
