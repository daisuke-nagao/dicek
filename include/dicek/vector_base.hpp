#ifndef UUID_23E3CA5E_8877_11E7_86C9_10050143A0C0
#define UUID_23E3CA5E_8877_11E7_86C9_10050143A0C0

#include "scalar_traits.hpp"

namespace dicek
{
    namespace math
    {
        /* Curiously Recurring Template Pattern */
        template<typename derived, typename _scalar_type, typename _scalar_traits = scalar_traits<_scalar_type>>
        class vector_base
        {
        public:
          typedef _scalar_traits scalar_traits;
          typedef typename scalar_traits::type scalar_type;

          derived operator+( const derived& rhs ) const
          {
              const derived* this_ = static_cast<const derived*>( this );
              return this_->add_impl( rhs );
          }

          derived scale( scalar_type val ) const
          {
              const derived* this_ = static_cast<const derived*>( this );
              return this_->scale_impl( val );
          }

        private:
        };
    }
}

#endif /* UUID_23E3CA5E_8877_11E7_86C9_10050143A0C0 */
