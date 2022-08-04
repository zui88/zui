#pragma once

#include "../core/core.h"

#include <memory>      // destroy
#include <new>         // launder
#include <type_traits> // aligned storage
#include <utility>     // forward

namespace zui {

template <class T, std::size_t N>
class static_vector {
public:
  static_vector() = default;
  ~static_vector() {
    for (std::size_t pos = 0; pos < m_size; ++pos) {
      std::destroy_at(std::launder(reinterpret_cast<T *>(&data[pos])));
    }
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    zui_assert(m_size >= N, "vector too small");
    ::new (&data[m_size]) T(std::forward<Args>(args)...);
    ++m_size;
  }

private:
  std::aligned_storage_t<sizeof(T), alignof(T)> data[N];
  std::size_t m_size { 0U };
};

} // namespace zui
