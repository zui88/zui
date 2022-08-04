#pragma once


#include "../util/counter.h"
#include "zui/core/logger.h"
#include <type_traits>
#include <utility>


namespace zui {

  
  template<typename Resource>
  class handle {
    template<typename>
    friend class resource_handle;


  public:
    typedef Resource* pointer;
    typedef Resource& reference;

    
    handle()
      : handle<Resource>{ static_cast<pointer>(nullptr) } {}

    
    ~handle()
    {
      if (!m_value)
	return;

      m_value->cnt_dec();
      if (m_value->cnt_get() == counter::flag::empty) {
	cleanup();
        delete m_value;
      }
    }

    
    // Template constructor
    // good for implicit conversion
    template<typename U, typename = std::enable_if<std::is_same_v<U, Resource> || std::is_base_of_v<Resource, U>>>
    handle(U* resource)
        : m_value{new resource_value{counter::cnt_min(), resource}}
    {
      m_value->cnt_inc();
    }


    /**
     * @brief Share the ownership
     */
    handle(handle& rh)
      : m_value{rh.m_value}
    {
      m_value->cnt_inc();
    }

    
    handle(handle&& rh) {
      if (this != &rh)
	m_value = std::exchange(rh.m_value, nullptr);
    }

    
    /**
     * @brief Convey ownership so that two independend but equal
     *        resources are made; copy of the hold resource.
     * 
     * @tparam U
     * @param rh
     * @note Resource must be define an assignment operator
     */
    handle& operator=(handle &rh) {
      if (this != &rh) {
	Resource* newResource { new Resource };
	*newResource = *rh;	// ini
	cleanup();
	m_value->m_resource = newResource;
      }
      return *this;
    }

    handle &operator=(handle &&rh) {
      if (this != &rh) {
	cleanup();
	m_value->m_resource = std::exchange(rh.m_value->m_resource, nullptr);
      }
      return *this;
    }

    template<typename Base, typename = std::enable_if_t<std::is_base_of_v<Base, Resource>>>
    operator Base*()
    {
      return handle<Base>{ m_value->m_resource };
    }

    bool is_empty() const noexcept { return m_value->m_resource == nullptr; }
    operator bool() const noexcept { return !is_empty(); }

    
    Resource &operator*()  const noexcept { return *m_value->m_resource; }
    Resource *get()        const noexcept { return this->operator->(); }
    Resource *operator->() const noexcept { return m_value->m_resource; }

    
  private:
    struct resource_value : counter {
      pointer m_resource;
    };

    void cleanup() { m_value->m_resource ? delete m_value->m_resource : void(); }

    resource_value *m_value;
  };


  template<typename Resource, typename ... Args>
  handle<Resource> make_handle(Args&& ... args) {
    return handle<Resource>{ new Resource{ std::forward<Args>(args)... } };
  }

  
}
