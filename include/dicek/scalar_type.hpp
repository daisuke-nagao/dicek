#ifndef UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854
#define UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854

#include <type_traits>

namespace dicek
{
    namespace math
    {
        template<typename T>
        struct scalar_type {
            typedef typename std::remove_cv<T>::type type;
        };
    }
}

#endif /* UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854 */
