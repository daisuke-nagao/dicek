#ifndef UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854
#define UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854

#include <type_traits>

namespace dicek
{
    namespace math
    {
        template<typename T>
        struct scalar_traits {
            typedef typename std::remove_reference<typename std::remove_cv<T>::type>::type type;

            static type conj( const type& val )
            {
                return val;
            }
        };

        template<typename U>
        struct scalar_traits<std::complex<U>> {
            typedef typename std::remove_reference<typename std::remove_cv<std::complex<U>>::type>::type type;

            static type conj( const type& val )
            {
                return std::conj( val );
            }
        };
    }
}

#endif /* UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854 */
