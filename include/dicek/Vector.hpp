#ifndef UUID_E2494444_AF31_11E6_9A65_0800274CD854
#define UUID_E2494444_AF31_11E6_9A65_0800274CD854

namespace dicek
{
    namespace math
    {
        template<unsigned long _DIM, typename _real, typename _real_traits = void>
        class vector {
        public:
            static const unsigned long DIM = _DIM;
            typedef _real real_type;
        };
    }
}

#endif /* UUID_E2494444_AF31_11E6_9A65_0800274CD854 */
