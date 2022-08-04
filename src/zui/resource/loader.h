#pragma once

#include <utility>
#include "handle.h"

namespace zui {

  template<typename Loader, typename Resource>
  struct loader {

    template<typename ... Args>
    [[nodiscard]] handle<Resource> get_resource(Args ...args) const {
      return static_cast<const Loader*>(this)->load (std::forward<Args>(args)...);
    }

  };

}
