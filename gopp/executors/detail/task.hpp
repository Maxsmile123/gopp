#pragma once

// https://github.com/Naios/function2
#include <function2/function2.hpp>

namespace gopp::executors::detail {

using Task = fu2::unique_function<void()>;

}  // namespace gopp::executors::detail
