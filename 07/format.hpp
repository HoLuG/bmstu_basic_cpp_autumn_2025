#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <string>
#include <exception>
#include <sstream>
#include <cctype>
#include <vector>
#include <concepts>
#include <limits>

class FormatException : public std::exception {
protected:
    std::string message;
public:
    explicit FormatException(const std::string& msg);
    const char* what() const noexcept override;
};

class InvalidBracesException : public FormatException {
public:
    explicit InvalidBracesException(const std::string& msg);
};

class ArgumentIndexException : public FormatException {
public:
    explicit ArgumentIndexException(const std::string& msg);
};

namespace detail {
    template<typename T>
    concept OutputStreamable = requires(std::ostream& os, const T& val) {
        os << val;
    };

    template<OutputStreamable T>
    std::string convertToString(const T& val) {
        std::ostringstream buffer;
        buffer << val;
        return buffer.str();
    }

    inline size_t parseIndexInPlace(const std::string& str, size_t start, size_t end, size_t bracePos) {
        if (start == end) {
            throw InvalidBracesException("Empty braces at position " + std::to_string(bracePos));
        }
        
        size_t index = 0;
        for (size_t i = start; i < end; i++) {
            char ch = str[i];
            if (!std::isdigit(static_cast<unsigned char>(ch))) {
                throw InvalidBracesException("Non-digit character in braces at position " + std::to_string(bracePos));
            }
            
            size_t digit = ch - '0';
            size_t maxVal = std::numeric_limits<size_t>::max();
            if (index > (maxVal - digit) / 10) {
                throw ArgumentIndexException("Index value is too large at position " + std::to_string(bracePos));
            }
            index = index * 10 + digit;
        }
        
        return index;
    }
}

template<typename... Args>
requires (detail::OutputStreamable<Args> && ...)
std::string format(const std::string& formatStr, const Args&... args) {
    constexpr size_t argCount = sizeof...(Args);
    
    std::vector<std::string> argStrings;
    argStrings.reserve(argCount);
    (argStrings.push_back(detail::convertToString(args)), ...);
    
    std::string result;
    result.reserve(formatStr.length());
    
    size_t i = 0;
    const size_t len = formatStr.length();
    
    while (i < len) {
        if (formatStr[i] == '{') {
            if (i + 1 >= len) {
                throw InvalidBracesException("Unclosed opening brace at position " + std::to_string(i));
            }
            
            size_t start = i + 1;
            size_t closePos = start;
            
            while (closePos < len) {
                if (formatStr[closePos] == '{') {
                    throw InvalidBracesException("Nested opening brace at position " + std::to_string(closePos));
                }
                if (formatStr[closePos] == '}') {
                    break;
                }
                closePos++;
            }
            
            if (closePos >= len) {
                throw InvalidBracesException("No closing brace for opening brace at position " + std::to_string(i));
            }
            
            size_t argIndex = detail::parseIndexInPlace(formatStr, start, closePos, i);
            
            if (argIndex >= argCount) {
                throw ArgumentIndexException("Index " + std::to_string(argIndex) +
                    " is out of range (only " + std::to_string(argCount) + " arguments provided)");
            }
            
            result += argStrings[argIndex];
            i = closePos + 1;
        } else if (formatStr[i] == '}') {
            throw InvalidBracesException("Closing brace without opening brace at position " + std::to_string(i));
        } else {
            result += formatStr[i];
            i++;
        }
    }
    
    return result;
}

#endif