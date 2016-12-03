#ifndef UUID_A8A43C38_B929_11E6_AB3C_0800274CD854
#define UUID_A8A43C38_B929_11E6_AB3C_0800274CD854

#include <cxxtest/TestSuite.h>
#include <type_traits>

#include "dicek/scalar_type.hpp"

class scalar_typeTest : public CxxTest::TestSuite {
public:
    void test_plain_type( void )
    {
        static_assert( std::is_same<float, dicek::math::scalar_type<float>::type>::value, "two types are not the same" );
        TS_ASSERT( ( std::is_same<float, dicek::math::scalar_type<float>::type>::value ) );

        static_assert( std::is_same<double, dicek::math::scalar_type<double>::type>::value, "two types are not the same" );
        TS_ASSERT( ( std::is_same<double, dicek::math::scalar_type<double>::type>::value ) );
    }

    void test_const_type( void )
    {
        static_assert( std::is_same<float, dicek::math::scalar_type<const float>::type>::value, "two types are not the same" );
        TS_ASSERT( ( std::is_same<float, dicek::math::scalar_type<const float>::type>::value ) );
    }

    void test_volatile_type( void )
    {
        static_assert( std::is_same<float, dicek::math::scalar_type<volatile float>::type>::value, "two types are not the same" );
        TS_ASSERT( ( std::is_same<float, dicek::math::scalar_type<volatile float>::type>::value ) );
    }
};

#endif /* UUID_A8A43C38_B929_11E6_AB3C_0800274CD854 */
