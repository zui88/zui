#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace zui {

class logger {
public:
  static inline std::shared_ptr<spdlog::logger> &GetLogger() {
  if (!s_logger) {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_logger = spdlog::stdout_color_st("zui core");
  }
  return s_logger;
}

private:
  logger() = default;
  ~logger() = default;

public:
  logger(logger &) = delete;
  logger &operator=(logger &) = delete;

private:
  static inline std::shared_ptr<spdlog::logger> s_logger;
};

} // namespace zui

#define ZUI_TRACE(...)  zui::logger::GetLogger()->trace(__VA_ARGS__)
// #define ZUI_TRACE(...)
#define ZUI_INFO(...)   zui::logger::GetLogger()->info(__VA_ARGS__)
#define ZUI_ERROR(...)  zui::logger::GetLogger()->error(__VA_ARGS__)
