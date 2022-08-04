#pragma once

#include <limits>
#include <cstddef>

namespace zui {

  
  typedef std::size_t counter_type;


  /**
   * A small class for counting references
   */
  struct counter {
    static constexpr const counter_type MIN = std::numeric_limits<counter_type>::min();
    static constexpr const counter_type MAX = std::numeric_limits<counter_type>::max();

    enum flag : counter_type {
      empty = MIN,
    };
    
    counter () = default;
    ~counter () = default;
    counter (counter_type cnt)
      : m_cnt{ cnt } { }
    /**
     * Reset the couter
     */
    counter(const counter &cnt) : m_cnt{ MIN } {}

    void                       cnt_inc()                       noexcept { this->operator++(); }
    void                       cnt_dec()                       noexcept { this->operator--(); }
    void                       cnt_reset()                     noexcept { m_cnt = MIN; }
    void                       cnt_set(counter_type cnt)       noexcept { m_cnt = cnt; }
    [[nodiscard]] counter_type cnt_get()                 const noexcept { return m_cnt; }

    [[nodiscard]] counter_type operator()() const noexcept { return m_cnt; }
    // just preinkrement
    counter_type operator++() noexcept {
      if (m_cnt < MAX) 
	m_cnt++;
      return m_cnt;
    }
    counter_type operator--() noexcept {
      if (m_cnt != MIN)
	m_cnt--;
      return m_cnt;
    }
    /**
     * Reset the couter
     */
    counter &operator=(counter_type& cnt) noexcept {
      cnt = MIN;
      return *this;
    }

    [[nodiscard]] static inline counter_type cnt_min() noexcept {
      return MIN;
    }
    [[nodiscard]] static inline counter_type cnt_max() noexcept {
      return MAX;
    }

  private:
    counter_type m_cnt { MIN };
  };


  template<typename Derived>
  struct static_counter {
    counter_type cnt_inc() noexcept {
      ++s_count;
      return cnt_get();
    }

    counter_type cnt_dec() noexcept {
      --s_count;
      return cnt_get();
    }

    counter_type operator++() noexcept { return cnt_inc(); }
    counter_type operator--() noexcept { return cnt_dec(); }

    counter_type cnt_get() const noexcept { return s_count.cnt_get(); }

  private:
    static inline counter s_count;
  };


}
