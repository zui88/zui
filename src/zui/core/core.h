#pragma once

#include "logger.h"
#include <signal.h>

#define zui_assert(x, ...) {if (!(x)) { ZUI_ERROR(__VA_ARGS__); raise (SIGTRAP); }}
