#include "parser.hpp"
#include <cctype>
#include <string>
#include <limits>

namespace {
    bool is_number_token(const std::string& token) {
        if (token.empty()) {
            return false;
        }

        for (unsigned char ch : token) {
            if (!std::isdigit(ch)) {
                return false;
            }
        }

        std::size_t first_non_zero = 0;
        while (first_non_zero < token.size() && token[first_non_zero] == '0') {
            first_non_zero++;
        }

        if (first_non_zero == token.size()) {
            return true;
        }

        std::string core = token.substr(first_non_zero);
        
        constexpr std::size_t max_digits = 20;
        
        if (core.size() < max_digits) {
            return true;
        }
        if (core.size() > max_digits) {
            return false;
        }
        
        static const std::string max_uint64_str = std::to_string(std::numeric_limits<std::uint64_t>::max());
        return core <= max_uint64_str;
    }

}

void parse(const std::string& text,
           func_digit_ptr digit_callback,
           func_str_ptr   string_callback)
{
    if (text.empty()) {
        return;
    }

    auto handle_token = [&](const std::string& token) {
        if (token.empty()) {
            return;
        }

        if (is_number_token(token)) {
            if (digit_callback) {
                digit_callback(std::stoull(token));
            }
        } else if (string_callback) {
            string_callback(token);
        }
    };

    std::string cur_token;
    for (unsigned char ch : text) {
        if (std::isspace(ch)) {
            handle_token(cur_token);
            cur_token.clear();
        } else {
            cur_token.push_back(static_cast<char>(ch));
        }
    }
    handle_token(cur_token);
}
