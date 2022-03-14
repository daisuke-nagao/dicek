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
#include <dicek/scalar_traits.hpp>
#include <initializer_list>
#include <memory_resource>
#include <optional>
#include <utility>
#include <vector>

namespace dicek::math::linalg {

namespace details {
template<typename T, typename scalar_traits>
class vector_const_iterator;
template<typename T, typename scalar_traits>
class vector_iterator {
  using scalar_type = typename scalar_traits::scalar_type;

 public:
  using difference_type   = ptrdiff_t;
  using value_type        = scalar_type;
  using pointer           = scalar_type*;
  using reference         = scalar_type&;
  using iterator_category = std::random_access_iterator_tag;

  vector_iterator() {}
  vector_iterator(const vector_iterator&)     = default;
  vector_iterator(vector_iterator&&) noexcept = default;
  vector_iterator(pointer ptr, difference_type step = 1) : ptr_(ptr), step_(step) {}

  vector_iterator& operator=(const vector_iterator&) = default;
  vector_iterator& operator=(vector_iterator&&) noexcept = default;

  operator vector_const_iterator<T, scalar_traits>() const {
    return vector_const_iterator<T, scalar_traits>(ptr_, step_);
  }

  bool operator==(const vector_iterator& rhs) const noexcept {
    return three_way_comparison(rhs) == 0;
  }
  bool operator!=(const vector_iterator& rhs) const noexcept {
    return !(*this == rhs);
  }
  pointer operator->() {
    return ptr_;
  }
  vector_iterator& operator++() {
    ptr_ += step_;
    return *this;
  }
  vector_iterator operator++(int) {
    auto tmp = *this;
    operator++();
    return tmp;
  }
  vector_iterator& operator--() {
    ptr_ -= step_;
    return *this;
  }
  vector_iterator operator--(int) {
    auto tmp = *this;
    operator--();
    return tmp;
  }
  reference operator*() {
    return *ptr_;
  }
  reference operator*() const {
    return *ptr_;
  }

  difference_type step() const {
    return step_;
  }

  vector_iterator& operator+=(difference_type n) {
    ptr_ += step_ * n;
    return *this;
  }

  vector_iterator operator+(difference_type n) const {
    auto tmp = *this;
    tmp += n;
    return tmp;
  }

  vector_iterator& operator-=(difference_type n) {
    ptr_ -= step_ * n;
    return *this;
  }

  vector_iterator operator-(difference_type n) const {
    auto tmp = *this;
    tmp -= n;
    return tmp;
  }

  reference operator[](difference_type n) const {
    return ptr_[step_ * n];
  }

  difference_type operator-(const vector_iterator& rhs) const {
    return (ptr_ - rhs.ptr_) / step_;
  }

 public:
  bool operator<(const vector_iterator& rhs) const {
    return three_way_comparison(rhs) < 0;
  }
  bool operator<=(const vector_iterator& rhs) const {
    return three_way_comparison(rhs) <= 0;
  }
  bool operator>(const vector_iterator& rhs) const {
    return three_way_comparison(rhs) > 0;
  }
  bool operator>=(const vector_iterator& rhs) const {
    return three_way_comparison(rhs) >= 0;
  }

 private:
  int three_way_comparison(const vector_iterator& rhs) const {
    if (ptr_ < rhs.ptr_) {
      return -1;
    } else if (ptr_ > rhs.ptr_) {
      return 1;
    } else {
      return 0;
    }
  }

 private:
  pointer ptr_          = nullptr;
  difference_type step_ = 0;
};

template<typename T, typename scalar_traits>
class vector_const_iterator {
  using scalar_type = typename scalar_traits::scalar_type;

 public:
  using difference_type   = ptrdiff_t;
  using value_type        = const scalar_type;
  using pointer           = const scalar_type*;
  using reference         = const scalar_type&;
  using iterator_category = std::random_access_iterator_tag;

  vector_const_iterator() {}
  vector_const_iterator(const vector_const_iterator&)     = default;
  vector_const_iterator(vector_const_iterator&&) noexcept = default;
  vector_const_iterator(pointer ptr, difference_type step = 1) : ptr_(ptr), step_(step) {}
  vector_const_iterator(const vector_iterator<T, scalar_traits>& rhs) : ptr_(&*rhs), step_(rhs.step()) {}

  vector_const_iterator& operator=(const vector_const_iterator&) = default;
  vector_const_iterator& operator=(vector_const_iterator&&) noexcept = default;

  bool operator==(const vector_const_iterator& rhs) const noexcept {
    return three_way_comparison(rhs) == 0;
  }
  bool operator!=(const vector_const_iterator& rhs) const noexcept {
    return !(*this == rhs);
  }
  pointer operator->() {
    return ptr_;
  }
  vector_const_iterator& operator++() {
    ptr_ += step_;
    return *this;
  }
  vector_const_iterator operator++(int) {
    auto tmp = *this;
    ptr_ += step_;
    return tmp;
  }
  vector_const_iterator& operator--() {
    ptr_ -= step_;
    return *this;
  }
  vector_const_iterator operator--(int) {
    auto tmp = *this;
    ptr_ -= step_;
    return tmp;
  }
  reference operator*() {
    return *ptr_;
  }
  reference operator*() const {
    return *ptr_;
  }

  difference_type step() const {
    return step_;
  }

  vector_const_iterator& operator+=(difference_type n) {
    ptr_ += step_ * n;
    return *this;
  }

  vector_const_iterator operator+(difference_type n) const {
    auto tmp = *this;
    tmp += n;
    return tmp;
  }

  vector_const_iterator& operator-=(difference_type n) {
    ptr_ -= step_ * n;
    return *this;
  }

  vector_const_iterator operator-(difference_type n) const {
    auto tmp = *this;
    tmp -= n;
    return tmp;
  }

  reference operator[](difference_type n) const {
    return ptr_[step_ * n];
  }

  difference_type operator-(const vector_const_iterator& rhs) const {
    return (ptr_ - rhs.ptr_) / step_;
  }

 public:
  bool operator<(const vector_const_iterator& rhs) const {
    return three_way_comparison(rhs) < 0;
  }
  bool operator<=(const vector_const_iterator& rhs) const {
    return three_way_comparison(rhs) <= 0;
  }
  bool operator>(const vector_const_iterator& rhs) const {
    return three_way_comparison(rhs) > 0;
  }
  bool operator>=(const vector_const_iterator& rhs) const {
    return three_way_comparison(rhs) >= 0;
  }

 private:
  int three_way_comparison(const vector_const_iterator& rhs) const {
    if (ptr_ < rhs.ptr_) {
      return -1;
    } else if (ptr_ > rhs.ptr_) {
      return 1;
    } else {
      return 0;
    }
  }

 private:
  pointer ptr_          = nullptr;
  difference_type step_ = 0;
};

template<typename T, typename scalar_traits>
vector_iterator<T, scalar_traits> operator+(typename vector_iterator<T, scalar_traits>::difference_type n, vector_iterator<T, scalar_traits> i);
template<typename T, typename scalar_traits>
vector_const_iterator<T, scalar_traits> operator+(typename vector_const_iterator<T, scalar_traits>::difference_type n, vector_const_iterator<T, scalar_traits> i);
}  // namespace details

template<typename T, typename scalar_traits = dicek::math::scalar_traits<T>>
class vector {
 public:
  using scalar_type = typename scalar_traits::scalar_type;

  /* constructor (1) */
  vector(){};
  /* constructor (2) */
  vector(std::size_t length, std::pmr::memory_resource* alloc = std::pmr::get_default_resource()) : length_(length), step_(1), allocator_(alloc) {
    using scalar_type_allocator_type                 = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<scalar_type>;
    using scalar_type_allocator_traits               = std::allocator_traits<scalar_type_allocator_type>;
    scalar_type_allocator_type scalar_type_allocator = allocator_;
    elm_                                             = scalar_type_allocator_traits::allocate(scalar_type_allocator, length_);
    scalar_type_allocator_traits::construct(scalar_type_allocator, elm_);

    using size_t_allocator_type            = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<size_t>;
    using size_t_allocator_traits          = std::allocator_traits<size_t_allocator_type>;
    size_t_allocator_type size_t_allocator = allocator_;
    ref_count_                             = size_t_allocator_traits::allocate(size_t_allocator, 1);
    size_t_allocator_traits::construct(size_t_allocator, ref_count_);
    ++*ref_count_;
  }
  /* constructor (3) */
  vector(scalar_type* buf, std::size_t length, std::size_t step = 1) : length_((length - 1) / step + 1), step_(step), allocator_(std::pmr::null_memory_resource()), elm_(buf) {}
  /* constructor (4) */
  vector(std::initializer_list<scalar_type> ini, std::pmr::memory_resource* alloc = std::pmr::get_default_resource()) : vector(ini.size(), alloc) {
    std::copy(std::begin(ini), std::end(ini), std::begin(*this));
  }
  /* copy constructor */
  vector(const vector& rhs) : length_(rhs.length_), step_(rhs.step_), allocator_(rhs.allocator_), ref_count_(rhs.ref_count_), elm_(rhs.elm_) {
    if (ref_count_ != nullptr) {
      ++*ref_count_;
    }
  }
  /* move constructor */
  vector(vector&& rhs) noexcept
      : length_(std::exchange(rhs.length_, 0)), step_(std::exchange(rhs.step_, 0)), allocator_(std::move(rhs.allocator_)), ref_count_(std::exchange(rhs.ref_count_, nullptr)), elm_(std::exchange(rhs.elm_, nullptr)) {}

  /* destructor */
  ~vector() noexcept {
    bool need_free = true;
    if (ref_count_ != nullptr) {
      --*ref_count_;
      if (*ref_count_ != 0) {
        need_free = false;
      }
    }
    if (need_free && allocator_ != nullptr) {
      using scalar_type_allocator_type                 = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<scalar_type>;
      using scalar_type_allocator_traits               = std::allocator_traits<scalar_type_allocator_type>;
      scalar_type_allocator_type scalar_type_allocator = allocator_;
      if (elm_ != nullptr) {
        scalar_type_allocator_traits::destroy(scalar_type_allocator, elm_);
        scalar_type_allocator_traits::deallocate(scalar_type_allocator, elm_, length_);
      }

      using size_t_allocator_type            = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<size_t>;
      using size_t_allocator_traits          = std::allocator_traits<size_t_allocator_type>;
      size_t_allocator_type size_t_allocator = allocator_;
      if (ref_count_ != nullptr) {
        size_t_allocator_traits::destroy(size_t_allocator, ref_count_);
        size_t_allocator_traits::deallocate(size_t_allocator, ref_count_, 1);
      }
    }
  }

  vector& operator=(const vector& rhs) {
    if (this != &rhs) {
      vector(rhs).swap(*this);
    }
    return *this;
  }

  vector& operator=(vector&& rhs) noexcept {
    if (this != &rhs) {
      length_    = std::exchange(rhs.length_, 0);
      step_      = std::exchange(rhs.step_, 0);
      allocator_ = std::exchange(rhs.allocator_, nullptr);
      ref_count_ = std::exchange(rhs.ref_count_, nullptr);
      elm_       = std::exchange(rhs.elm_, nullptr);
    }
    return *this;
  }

  void swap(vector& rhs) noexcept {
    using std::swap;
    swap(*this, rhs);
  }

  using iterator       = details::vector_iterator<T, scalar_traits>;
  using const_iterator = details::vector_const_iterator<T, scalar_traits>;

  const_iterator begin() const {
    return const_iterator(elm_, step_);
  }
  const_iterator cbegin() const {
    return begin();
  }
  iterator begin() {
    return iterator(elm_, step_);
  }
  const_iterator end() const {
    return const_iterator(elm_ + length_ * step_, step_);
  }
  const_iterator cend() const {
    return end();
  }
  iterator end() {
    return iterator(elm_ + length_ * step_, step_);
  }

  std::size_t size() const {
    return length_;
  }

  const scalar_type& operator[](std::size_t idx) const {
    return *(elm_ + idx * step_);
  }
  const scalar_type& at(size_t idx) const {
    if (idx >= length_) {
      throw std::out_of_range("vector::at: idx >= this->size()");
    }
    return operator[](idx);
  }

  scalar_type& operator[](std::size_t idx) {
    return const_cast<scalar_type&>(const_cast<const vector*>(this)->operator[](idx));
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

  vector clone(std::pmr::memory_resource* allocator) const {
    vector r(size(), allocator);
    std::copy(this->begin(), this->end(), r.begin());
    return r;
  }

  vector clone() const {
    return clone(get_allocator());
  }

  std::pmr::memory_resource* get_allocator() const noexcept {
    return allocator_;
  }

 private:
  std::size_t length_ = 0;
  std::size_t step_;
  std::pmr::memory_resource* allocator_ = nullptr;
  std::size_t* ref_count_               = nullptr;
  scalar_type* elm_                     = nullptr;
};

namespace details {
template<typename T, typename scalar_traits>
vector_iterator<T, scalar_traits> operator+(typename vector_iterator<T, scalar_traits>::difference_type n, vector_iterator<T, scalar_traits> i) {
  return i += n;
}
template<typename T, typename scalar_traits>
vector_const_iterator<T, scalar_traits> operator+(typename vector_const_iterator<T, scalar_traits>::difference_type n, vector_const_iterator<T, scalar_traits> i) {
  return i += n;
}
}  // namespace details
}  // namespace dicek::math::linalg

#endif /* UUID_6F484ACB_9C23_4013_A905_B5DAC701113A */
