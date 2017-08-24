/*
MIT License

Copyright (c) 2017 Daisuke NAGAO

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

          derived add( const derived& rhs ) const
          {
              const derived* this_ = static_cast<const derived*>( this );
              return this_->add_impl( rhs );
          }

          derived operator+( const derived& rhs ) const
          {
              return add( rhs );
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
