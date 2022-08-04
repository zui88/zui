#pragma once

#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "handle.h"
#include "loader.h"
#include "zui/core/logger.h"

namespace zui {

template <typename Resource>
class cache {
  static_assert(std::is_same_v<Resource, std::remove_reference_t<std::remove_const_t<Resource>>>, "Invalid resource type");
  typedef std::unordered_map<uint32_t, handle<Resource>> container;

  
public:
  template<typename Loader, typename ...Args>
  [[nodiscard]] handle<Resource> load (uint32_t id, Args&&... args) {
    if (auto it = m_container.find(id); it == m_container.cend()) {
      if(auto handle = temp<Loader>(std::forward<Args>(args)...); handle) {
	return m_container[id] = std::move(handle);
      }
    } else {
      return it->second;
    }

    return {};
  }

  
  template<typename Loader, typename ...Args>
  [[nodiscard]] std::pair<uint32_t, handle<Resource>> force_load (uint32_t id, Args&&... args) {
    return {};   
  }

  
private:
  template<typename Loader, typename ...Args>
  [[nodiscard]] handle<Resource> temp (Args&&... args) const {
    return Loader{}.get_resource(std::forward<Args>(args)...);
  }


private:
  container m_container;
};

} // namespace zui
