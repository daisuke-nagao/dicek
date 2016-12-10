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
#ifndef UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854
#define UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854

#include <type_traits>
#include <cmath>
#include <complex>

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

            static type abs( const type& val )
            {
                using std::abs;
                return abs( val );
            }
        };

        template<typename U>
        struct scalar_traits<std::complex<U>> {
            typedef typename std::remove_reference<typename std::remove_cv<std::complex<U>>::type>::type type;

            static type conj( const type& val )
            {
                return std::conj( val );
            }

            static type abs( const type& val )
            {
                using std::abs;
                return abs( val );
            }
        };
    }
}

#endif /* UUID_DFDD573E_B92E_11E6_AB3C_0800274CD854 */
