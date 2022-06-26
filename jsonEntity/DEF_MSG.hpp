/**
 * 定义了一些常用的msg
 */

#pragma once
#include "http_server.hpp"
#include <string_view>

namespace MSG {
using namespace std::literals;


constexpr auto OK = "OK"sv;
constexpr auto NEED_LOGIN      = "NEED_LOGIN"sv;
constexpr auto ALREADY_LOGINED = "ALREADY_LOGINED"sv;
constexpr auto WRONG_PASSWORD_OR_USERNAME = "WRONG_PASSWORD_OR_USERNAME"sv;

constexpr auto FINISH_JUDGE = "FINISH_JUDGE"sv;
} // end namespace MSG


