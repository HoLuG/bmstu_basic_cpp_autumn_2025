#include "format.hpp"

FormatException::FormatException(const std::string& msg) : message(msg) {}

const char* FormatException::what() const noexcept {
    return message.c_str();
}

InvalidBracesException::InvalidBracesException(const std::string& msg)
    : FormatException("Invalid braces: " + msg) {}

ArgumentIndexException::ArgumentIndexException(const std::string& msg)
    : FormatException("Argument index error: " + msg) {}
