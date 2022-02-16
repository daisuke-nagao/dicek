/*
MIT License

Copyright (c) 2022 Daisuke NAGAO

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: 
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
#ifndef UUID_6F484ACB_9C23_4013_A905_B5DAC701113A
#define UUID_6F484ACB_9C23_4013_A905_B5DAC701113A

#include <cstddef>
#include <memory_resource>
#include <optional>
#include <utility>
#include <vector>

namespace dicek::math::linalg {
template<typename scalar_traits>
class vector {
 public:
  using scalar_type = typename scalar_traits::scalar_type;

  /* constructor (1) */
  vector() : length_(0), allocator_(), ref_count_(nullptr), elm_(nullptr){};
  /* constructor (2) */
  vector(std::size_t length, std::pmr::polymorphic_allocator<std::byte> alloc = std::pmr::polymorphic_allocator<std::byte>()) : length_(length), allocator_(alloc), ref_count_(nullptr), elm_(nullptr) {
    using scalar_type_allocator_type                 = typename std::allocator_traits<decltype(allocator_)>::template rebind_alloc<scalar_type>;
    using scalar_type_allocator_traits               = std::allocator_traits<scalar_type_allocator_type>;
    scalar_type_allocator_type scalar_type_allocator = allocator_;
    elm_                                             = scalar_type_allocator_traits::allocate(scalar_type_allocator, length_);
    scalar_type_allocator_traits::construct(scalar_type_allocator, elm_);

    using size_t_allocator_type            = typename std::allocator_traits<decltype(allocator_)>::template rebind_alloc<size_t>;
    using size_t_allocator_traits          = std::allocator_traits<size_t_allocator_type>;
    size_t_allocator_type size_t_allocator = allocator_;
    ref_count_                             = size_t_allocator_traits::allocate(size_t_allocator, 1);
    size_t_allocator_traits::construct(size_t_allocator, ref_count_);
    ++*ref_count_;
  }
  /* constructor (3) */
  vector(scalar_type* buf, std::size_t length) : length_(length), allocator_(std::pmr::null_memory_resource()), ref_count_(nullptr), elm_(buf) {}
  /* copy constructor */
  vector(const vector& rhs) : length_(rhs.length_), allocator_(rhs.allocator_), ref_count_(rhs.ref_count_), elm_(rhs.elm_) {
    if (ref_count_ != nullptr) {
      ++*ref_count_;
    }
  }
  /* move constructor */
  vector(vector&& rhs) : length_(std::exchange(rhs.length_, 0)), allocator_(std::move(rhs.allocator_)), ref_count_(std::exchange(rhs.ref_count_, nullptr)), elm_(std::exchange(rhs.elm_, nullptr)) {}

  /* destructor */
  ~vector() noexcept {
    bool need_free = true;
    if (ref_count_ != nullptr) {
      --*ref_count_;
      if (*ref_count_ != 0) {
        need_free = false;
      }
    }
    if (need_free) {
      using scalar_type_allocator_type                 = typename std::allocator_traits<decltype(allocator_)>::template rebind_alloc<scalar_type>;
      using scalar_type_allocator_traits               = std::allocator_traits<scalar_type_allocator_type>;
      scalar_type_allocator_type scalar_type_allocator = allocator_;
      scalar_type_allocator_traits::destroy(scalar_type_allocator, elm_);
      scalar_type_allocator_traits::deallocate(scalar_type_allocator, elm_, length_);

      using size_t_allocator_type            = typename std::allocator_traits<decltype(allocator_)>::template rebind_alloc<size_t>;
      using size_t_allocator_traits          = std::allocator_traits<size_t_allocator_type>;
      size_t_allocator_type size_t_allocator = allocator_;
      size_t_allocator_traits::destroy(size_t_allocator, ref_count_);
      size_t_allocator_traits::deallocate(size_t_allocator, ref_count_, 1);
    }
  }

  vector& operator=(const vector&) = delete;

  using iterator = scalar_type*;
  iterator begin() {
    return elm_;
  }
  iterator end() {
    return elm_ + length_;
  }

  std::size_t size() const {
    return length_;
  }

  const scalar_type& at(size_t idx) const {
    if (idx >= length_) {
      throw std::out_of_range("vector::at: idx >= this->size()");
    }
    return elm_[idx];
  }

  scalar_type& at(size_t idx) {
    return const_cast<scalar_type&>(const_cast<const vector*>(this)->at(idx));
  }

  const scalar_type* data() const {
    return elm_;
  }

  std::optional<std::size_t> ref_count() const {
    if (ref_count_ != nullptr) {
      return *ref_count_;
    } else {
      return std::nullopt;
    }
  }

  scalar_type* data() {
    return elm_;
  }

  vector clone(std::pmr::polymorphic_allocator<std::byte> allocator) {
    vector r(size(), allocator);
    for (std::size_t i = 0; i < size(); ++i) {
      r.at(i) = at(i);
    }
    return r;
  }

  vector clone() {
    return clone(get_allocator());
  }

  std::pmr::polymorphic_allocator<std::byte> get_allocator() const noexcept {
    return allocator_;
  }

 private:
  std::size_t length_;
  std::pmr::polymorphic_allocator<std::byte> allocator_;
  std::size_t* ref_count_;
  scalar_type* elm_;
};
}  // namespace dicek::math::linalg

#endif /* UUID_6F484ACB_9C23_4013_A905_B5DAC701113A */
