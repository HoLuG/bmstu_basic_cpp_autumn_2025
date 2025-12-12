#pragma once

#include <cstdint>
#include <string>
#include <functional>

using func_digit_ptr = std::function<void(std::uint64_t)>;
using func_str_ptr   = std::function<void(const std::string&)>;

void parse(const std::string& text,
           func_digit_ptr digit_callback = func_digit_ptr{},
           func_str_ptr   string_callback = func_str_ptr{});
