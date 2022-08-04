#pragma once

#include "../resource/loader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <string_view>

namespace zui {

/**
 * @brief formatted file loader (ffile_loader) stores entire file into
 *        a string object
 * 
 * @example
 *
 * int main() {
 *   zui::file_loader fLoader;
 *
 *   auto res = fLoader.get_resource("test.txt");
 *   std::cout << *res << std::endl;
 *
 *   return 0;
 * }
 */
struct file_loader : loader<file_loader, std::string> {
  [[nodiscard]] handle<std::string> load(std::string_view path) const {
    std::ifstream file{ path.data() };
    if (file.is_open()) {
      std::stringstream ss;
      ss << file.rdbuf();
      return make_handle<std::string>(ss.str());
    }
    return make_handle<std::string>();
  }
};

/**
 * @brief binary file loader (bfile_loader) stores entire file into
 *        a string object.  
 */
// struct bfile_loader : loader<bfile_loader, std::string> {
//   [[nodiscard]] resource_handle<std::string> load(const char *path) const;
// };

} // namespace zui
