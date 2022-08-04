export module zui;

#include <utility>

export namespace zui {

template <typename Resource>
class unique_handle {
public:
  unique_handle() noexcept : m_resource{nullptr} {}

  unique_handle(Resource *resource) noexcept : m_resource{resource} {}

  ~unique_handle() noexcept { cleanup(); }

  unique_handle(const unique_handle &) = delete;
  unique_handle &operator=(const unique_handle &) = delete;

  unique_handle(unique_handle &&object) noexcept {
    std::exchange(m_resource, object.m_resource);
  }

  void operator=(unique_handle &&object) noexcept {
    cleanup();
    m_resource = std::exchange(object.m_resource, nullptr);
  }

  Resource *operator->() const noexcept { return this; }

  Resource &operator*() const noexcept { return *(this->m_resource); }

private:
  void cleanup() noexcept { m_resource ? delete m_resource : void(0); }

  Resource *m_resource;
};

} // namespace zui
