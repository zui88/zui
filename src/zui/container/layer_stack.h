#pragma once

#include <iterator>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <vector>
#include <memory>

// requires at least c++17
namespace zui {

template <typename T>
class layer_stack {
public:
  ////////////////////////////////////////////////////////////////
  // 
  // types
  // 
  ////////////////////////////////////////////////////////////////
  using resource_handle = std::shared_ptr<T>;
  using weak_handle     = std::weak_ptr<T>;

private:
  typedef std::vector<resource_handle> container;

public:
  typedef typename container::iterator          iterator;
  typedef typename container::const_iterator    const_iterator;
  
  ////////////////////////////////////////////////////////////////
  // 
  // interface
  // 
  ////////////////////////////////////////////////////////////////

  /**
   *  @brief  Constructs an Element in its internal cache
   *  @param  args  The arguments for the construction of the passed element
   *  @return Returns a weak handle connected to the resource
  */

  template<typename Resource, typename ... Args>
  weak_handle            emplace_layer(Args&&... args);
  weak_handle            emplace_layer(resource_handle const& layer);
  resource_handle        pop_layer(resource_handle const& layer);

  template<typename Resource, typename ... Args>
  weak_handle            emplace_overlay(Args&&... args);
  weak_handle            emplace_overlay(resource_handle const& layer);
  resource_handle        pop_overlay(resource_handle const& layer);

  [[nodiscard]] bool  is_empty()         const noexcept { return m_stack.empty(); }
  [[nodiscard]] bool  is_layer_empty()   const noexcept { return m_offset == 0; }
  [[nodiscard]] bool  is_overlay_empty() const noexcept { return m_stack.begin() + m_offset == m_stack.end(); }

  void                clear_container()        noexcept { m_stack.clear(); m_offset = 0; }
  void                clear_layers()   {}
  void                clear_overlays() {}

  const_iterator      begin()            const noexcept { return m_stack.begin(); }
  const_iterator      end()              const noexcept { return m_stack.end(); }
  iterator            begin()                  noexcept { return m_stack.begin(); }
  iterator            end()                    noexcept { return m_stack.end();}
  const_iterator      cbegin()           const noexcept { return m_stack.cbegin(); }
  const_iterator      cend()             const noexcept { return m_stack.cend(); }

private:
  ////////////////////////////////////////////////////////////////
  // 
  // private methods
  // 
  ////////////////////////////////////////////////////////////////
  weak_handle            emplace_layer_impl(resource_handle const& layer);
  
  [[nodiscard]] iterator find_layer(iterator const& begin, iterator const& end, resource_handle const &layer) {
    return std::find_if(begin, end, [&layer](resource_handle const &sp) {
	  return sp.get() == layer.get();
	});
  }

  [[nodiscard]] resource_handle erase(iterator const &pos) {
    auto tmp_obj{*pos};
    m_stack.erase(pos);
    return tmp_obj;
  }

  ////////////////////////////////////////////////////////////////
  //
  // fields
  //
  ////////////////////////////////////////////////////////////////
  container   m_stack;
  std::size_t m_offset{ 0U };
};


  ////////////////////////////////////////////////////////////////
  //
  // Implementation
  //
  ////////////////////////////////////////////////////////////////
  template<typename T>
  template<typename Resource, typename ... Args>
  typename layer_stack<T>::weak_handle layer_stack<T>::emplace_layer(Args&&... args) {
    static_assert(std::is_base_of<T, Resource>::value, "Res must be decendant of T");
    return this->emplace_layer(resource_handle{new Resource{std::forward<Args>(args)...}});
  }

  
  template<typename T>
  typename layer_stack<T>::weak_handle layer_stack<T>::emplace_layer(resource_handle const& layer) {
    if (m_stack.empty())
      return *m_stack.emplace(m_stack.begin() + m_offset++, layer);
    else {
      // avoid duplication
      iterator pos = find_layer(m_stack.begin(), m_stack.begin() + m_offset, layer);
      // insert object and increment offset afterwards -> postcondition range: object)
      if (pos == m_stack.begin() + m_offset)
	return *m_stack.emplace(m_stack.begin() + m_offset++, layer);
    }
    // empty or duplicate
    return weak_handle{};
  }

  
  template <typename T>
  typename layer_stack<T>::resource_handle layer_stack<T>::pop_layer(resource_handle const& layer) {
    if (!m_stack.empty()) {
      iterator pos = find_layer(m_stack.begin(), m_stack.begin() + m_offset, layer);

      // found
      if (pos != m_stack.begin() + m_offset) {
	m_offset--;
	return erase(pos);
      }
    }
    // element not found and/or stack empty
    return resource_handle{};
  }

  
  template<typename T>
  template<typename Resource, typename ... Args>
  typename layer_stack<T>::weak_handle layer_stack<T>::emplace_overlay(Args&&... args) {
    static_assert(std::is_base_of<T, Resource>::value, "Res must be decendant of T");
    return this->emplace_overlay(resource_handle{new Resource{std::forward<Args>(args)...}});
  }

  
  template<typename T>
  typename layer_stack<T>::weak_handle layer_stack<T>::emplace_overlay(resource_handle const& layer) {
    iterator pos = find_layer(m_stack.begin() + m_offset, m_stack.end(), layer);

    // not an element yet
    if (pos == m_stack.end())
      return m_stack.emplace_back(layer);
    // duplicate
    return weak_handle{};
  }

  
  template <typename T>
  typename layer_stack<T>::resource_handle layer_stack<T>::pop_overlay(resource_handle const& layer) {
    if (!m_stack.empty()) {
       iterator pos = find_layer(m_stack.begin() + m_offset, m_stack.end(), layer);

       // found
       if (pos != m_stack.end())
	 return erase(pos);
    }
    // element not found and/or stack empty
    return resource_handle{};
  }

} // namespace zui
