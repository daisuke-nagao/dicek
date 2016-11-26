#ifndef UUID_E2494444_AF31_11E6_9A65_0800274CD854
#define UUID_E2494444_AF31_11E6_9A65_0800274CD854

#include <array>

namespace dicek
{
    namespace math
    {
        template<unsigned long _DIM, typename _real, typename _real_traits = void>
        class vector {
        public:
            static const unsigned long DIM = _DIM;
            typedef _real real_type;

            real_type& at( unsigned long index )
            {
                return m_elm.at( index );
            }

            const real_type& at( unsigned long index ) const
            {
                return m_elm.at( index );
            }

        private:
            std::array<real_type, DIM> m_elm;
        };
    }
}

#endif /* UUID_E2494444_AF31_11E6_9A65_0800274CD854 */
