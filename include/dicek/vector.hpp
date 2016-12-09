#ifndef UUID_E2494444_AF31_11E6_9A65_0800274CD854
#define UUID_E2494444_AF31_11E6_9A65_0800274CD854

#include <array>
#include "scalar_traits.hpp"

namespace dicek
{
    namespace math
    {
        template<unsigned long _DIM, typename _scalar_type, typename _scalar_traits = scalar_traits<_scalar_type>>
        class vector {
        public:
            static const unsigned long DIM = _DIM;
            typedef _scalar_traits scalar_traits;
            typedef typename scalar_traits::type scalar_type;

            vector()
                : m_elm()
            {
            }

            scalar_type& at( unsigned long index )
            {
                return m_elm.at( index );
            }

            const scalar_type& at( unsigned long index ) const
            {
                return m_elm.at( index );
            }

            scalar_type& operator[]( unsigned long index )
            {
                return m_elm[ index ];
            }

            const scalar_type& operator[]( unsigned long index ) const
            {
                return m_elm[ index ];
            }

            vector operator+( const vector& rhs ) const
            {
                vector ret;

                for( unsigned long index = 0; index < DIM; ++index ) {
                    ret[ index ] = ( *this )[ index ] + rhs[ index ];
                }

                return ret;
            }

            template< typename F >
            vector map( F f ) const
            {
                vector ret;

                for( unsigned long index = 0; index < DIM; ++index ) {
                    ret[ index ] = f( ( *this )[ index ] );
                }

                return ret;
            }

            vector scale( scalar_type val ) const
            {
                return map( [ val ]( scalar_type x ) { return val * x; } );
            }

        private:
            std::array<scalar_type, DIM> m_elm;
        };

        template< typename V >
        typename V::scalar_type inner_product( const V& lhs, const V& rhs )
        {
            typedef typename V::scalar_type scalar_type;
            scalar_type ret = scalar_type();

            for( unsigned long index = 0; index < V::DIM; ++index ) {
                ret += lhs[ index ] * V::scalar_traits::conj( rhs[ index ] );
            }

            return ret;
        }
    }
}

#endif /* UUID_E2494444_AF31_11E6_9A65_0800274CD854 */