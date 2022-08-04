#pragma once

#include <limits>
#include <cstddef>

namespace zui {

  
  typedef std::size_t counter_type;

  
  template<typename T>
  struct counter_functions {
    static constexpr const counter_type MIN = std::numeric_limits<counter_type>::min();
    static constexpr const counter_type MAX = std::numeric_limits<counter_type>::max();

    enum flag : counter_type {
      empty = MIN,
    };
    
    void                       cnt_inc()                       noexcept { this->operator++(); }
    void                       cnt_dec()                       noexcept { this->operator--(); }
    void                       cnt_reset()                     noexcept { mh_underlying->cnt_set( MIN ); }

    [[nodiscard]] counter_type operator()() const noexcept { return mh_underlying->cnt_get(); }
    // just preinkrement
    counter_type operator++() noexcept {
      auto tmpCnt = mh_underlying->cnt_get();
      if (tmpCnt < MAX) 
	return mh_underlying->cnt_set(tmpCnt + 1);
      return tmpCnt;
    }
    counter_type operator--() noexcept {
      auto tmpCnt = mh_underlying->cnt_get();
      if (tmpCnt != MIN)
	return mh_underlying->cnt_set(tmpCnt - 1);
      return tmpCnt;
    }

    [[nodiscard]] static inline counter_type cnt_min() noexcept {
      return MIN;
    }
    [[nodiscard]] static inline counter_type cnt_max() noexcept {
      return MAX;
    }

  private:
    T* mh_underlying = static_cast<T*>(this);
  };

#define CNT_SET(XCNT)                                                          \
  counter_type cnt_set(counter_type cnt) noexcept { XCNT = cnt; }
#define CNT_GET(XCNT)                                                          \
  [[nodiscard]] counter_type cnt_get() const noexcept { return XCNT; }

  /**
   * A small class for counting references
   */
  struct counter : counter_functions<counter>{
    CNT_SET(m_cnt)
    CNT_GET(m_cnt)
  private:
    counter_type m_cnt { MIN };
  };


  /**
   * A small class for counting static references
   */
  template<typename Derived>
  struct static_counter : counter_functions<static_counter<Derived>>{
    CNT_SET(s_cnt)
    CNT_GET(s_cnt)
  private:
    static inline counter_type s_cnt {};
  };


}
