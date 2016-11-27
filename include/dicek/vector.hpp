#ifndef UUID_E2494444_AF31_11E6_9A65_0800274CD854
#define UUID_E2494444_AF31_11E6_9A65_0800274CD854

#include <array>

namespace dicek
{
    namespace math
    {
        template<unsigned long _DIM, typename _real>
        class vector {
        public:
            static const unsigned long DIM = _DIM;
            typedef _real real_type;

            vector()
                : m_elm()
            {
            }

            real_type& at( unsigned long index )
            {
                return m_elm.at( index );
            }

            const real_type& at( unsigned long index ) const
            {
                return m_elm.at( index );
            }

            real_type& operator[]( unsigned long index )
            {
                return m_elm[ index ];
            }

            const real_type& operator[]( unsigned long index ) const
            {
                return m_elm[ index ];
            }

            vector operator+( const vector& rhs ) const
            {
                vector ret;

                for( unsigned long index = 0; index < DIM; ++index ) {
                    ret[ index ] = (*this)[ index ] + rhs[ index ];
                }

                return ret;
            }

        private:
            std::array<real_type, DIM> m_elm;
        };
    }
}

#endif /* UUID_E2494444_AF31_11E6_9A65_0800274CD854 */
