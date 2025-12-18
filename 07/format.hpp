#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <string>
#include <exception>
#include <sstream>
#include <cctype>
#include <vector>
#include <set>

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
    std::string convertToString(const T& val) {
        std::ostringstream buffer;
        buffer << val;
        return buffer.str();
    }

    inline bool isValidDigit(char ch) {
        return std::isdigit(static_cast<unsigned char>(ch));
    }

    inline size_t parseIndex(const std::string& str, size_t pos) {
        if (str.empty()) {
            throw InvalidBracesException("Empty braces at position " + std::to_string(pos));
        }
        
        for (char ch : str) {
            if (!isValidDigit(ch)) {
                throw InvalidBracesException("Non-digit character in braces at position " + std::to_string(pos));
            }
        }
        
        try {
            return std::stoull(str);
        } catch (const std::out_of_range&) {
            throw ArgumentIndexException("Index value is too large at position " + std::to_string(pos));
        } catch (const std::invalid_argument&) {
            throw InvalidBracesException("Invalid index at position " + std::to_string(pos));
        }
    }

    inline size_t findCloseBrace(const std::string& str, size_t start) {
        for (size_t i = start + 1; i < str.length(); i++) {
            if (str[i] == '{') {
                throw InvalidBracesException("Nested opening brace at position " + std::to_string(i));
            }
            if (str[i] == '}') {
                return i;
            }
        }
        throw InvalidBracesException("No closing brace for opening brace at position " + std::to_string(start));
    }

    inline void collectUsedIndices(const std::string& fmt, std::set<size_t>& indices) {
        size_t pos = 0;
        while (pos < fmt.length()) {
            if (fmt[pos] == '{') {
                if (pos + 1 >= fmt.length()) {
                    throw InvalidBracesException("Unclosed opening brace at position " + std::to_string(pos));
                }
                size_t closePos = findCloseBrace(fmt, pos);
                std::string indexStr = fmt.substr(pos + 1, closePos - pos - 1);
                size_t idx = parseIndex(indexStr, pos);
                indices.insert(idx);
                pos = closePos + 1;
            } else if (fmt[pos] == '}') {
                throw InvalidBracesException("Closing brace without opening brace at position " + std::to_string(pos));
            } else {
                pos++;
            }
        }
    }
}

template<typename... Args>
std::string format(const std::string& formatStr, const Args&... args) {
    constexpr size_t argCount = sizeof...(Args);
    std::set<size_t> usedIndices;
    detail::collectUsedIndices(formatStr, usedIndices);
    if (!usedIndices.empty()) {
        size_t maxIndex = *usedIndices.rbegin();
        if (maxIndex >= argCount) {
            throw ArgumentIndexException("Index " + std::to_string(maxIndex) +
                " is out of range (only " + std::to_string(argCount) + " arguments provided)");
        }
    }
    
    std::vector<std::string> argCache;
    if (!usedIndices.empty() && argCount > 0) {
        size_t maxIdx = *usedIndices.rbegin();
        argCache.resize(maxIdx + 1);
        size_t count = 0;
        auto convertUpTo = [&](const auto& arg) {
            if (count <= maxIdx) {
                argCache[count] = detail::convertToString(arg);
            }
            count++;
        };
        if constexpr (sizeof...(Args) > 0) {
            (convertUpTo(args), ...);
        }
    }
    
    std::string result;
    result.reserve(formatStr.length());
    size_t i = 0;
    const size_t len = formatStr.length();
    
    while (i < len) {
        if (formatStr[i] == '{') {
            size_t closePos = detail::findCloseBrace(formatStr, i);
            std::string indexStr = formatStr.substr(i + 1, closePos - i - 1);
            size_t argIndex = detail::parseIndex(indexStr, i);
            
            if (argIndex >= argCount) {
                throw ArgumentIndexException("Index " + std::to_string(argIndex) +
                    " is out of range (only " + std::to_string(argCount) + " arguments provided)");
            }
            
            result += argCache[argIndex];
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