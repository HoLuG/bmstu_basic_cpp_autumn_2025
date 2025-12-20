#include "BigInt.hpp"
#include <algorithm>
#include <sstream>
#include <stddef.h>

BigInt::BigInt() : length(0), negative(false) {
    data = new int32_t[1];
    data[0] = 0;
    length = 1;
}

BigInt::BigInt(int32_t value) : length(0), negative(false) {
    int64_t abs_value = static_cast<int64_t>(value);
    if (abs_value < 0) {
        negative = true;
        abs_value = -abs_value;
    }

    int64_t temp = abs_value;
    length = 0;
    while (temp > 0) {
        temp /= 10;
        length++;
    }

    data = new int32_t[length];
    temp = abs_value;
    for (size_t i = 0; i < length; i++) {
        data[i] = temp % 10;
        temp /= 10;
    }
}

BigInt::BigInt(const std::string& str) : length(0), negative(false) {
    if (str.empty()) {
        data = new int32_t[1];
        data[0] = 0;
        length = 1;
        return;
    }

    size_t start_pos = 0;
    if (str[0] == '-') {
        negative = true;
        start_pos = 1;
    }

    length = str.length() - start_pos;

    if (length == 1 && str[start_pos] == '0') {
        negative = false;
    }
    data = new int32_t[length];
    for (size_t i = 0; i < length; i++) {
        data[length - 1 - i] = str[i + start_pos] - '0';
    }
    TrimZeros();
}

BigInt::BigInt(const BigInt& rhs) : length(rhs.length), negative(rhs.negative) {
    data = new int32_t[rhs.length];
    for (size_t i = 0; i < rhs.length; i++) {
        data[i] = rhs.data[i];
    }
}

BigInt::BigInt(BigInt&& rhs) noexcept : data(rhs.data), length(rhs.length), negative(rhs.negative) {
    rhs.data = nullptr;
    rhs.length = 0;
}

BigInt::~BigInt() {
    delete[] data;
}

BigInt& BigInt::operator=(const BigInt& rhs) {
    BigInt temp(rhs);
    this->swap(temp);
    return *this;
}

void BigInt::swap(BigInt& other) noexcept {
    std::swap(data, other.data);
    std::swap(length, other.length);
    std::swap(negative, other.negative);
}

BigInt& BigInt::operator=(BigInt&& rhs) noexcept {
    if (this != &rhs) {
        delete[] data;

        data = rhs.data;
        length = rhs.length;
        negative = rhs.negative;

        rhs.data = nullptr;
        rhs.length = 0;
    }

    return *this;
}

void BigInt::TrimZeros() {
    size_t actual_length = length;
    while (actual_length > 1 && data[actual_length - 1] == 0) {
        actual_length--;
    }

    if (actual_length < length) {
        AllocateMemory(actual_length);
    }
}

void BigInt::AllocateMemory(size_t new_length) {
    if (new_length == 0) {
        new_length = 1;
    }

    int32_t* new_data = new int32_t[new_length];

    size_t copy_len = std::min(length, new_length);
    for (size_t i = 0; i < copy_len; i++) {
        new_data[i] = data[i];
    }

    for (size_t i = copy_len; i < new_length; i++) {
        new_data[i] = 0;
    }

    delete[] data;
    data = new_data;
    length = new_length;
}

BigInt BigInt::operator-() const {
    BigInt result(*this);
    if (length > 1 || data[0] != 0) {
        result.negative = !negative;
    }
    return result;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
    if (negative && !rhs.negative) {
        return rhs - (-(*this));
    }

    if (!negative && rhs.negative) {
        return *this - (-rhs);
    }

    BigInt result;
    result.negative = negative;

    size_t max_len = std::max(length, rhs.length);
    size_t result_len = max_len + 1;

    result.AllocateMemory(result_len);

    int32_t carry = 0;
    for (size_t i = 0; i < result_len; i++) {
        int32_t sum = carry;
        if (i < length) {
            sum += data[i];
        }
        if (i < rhs.length) {
            sum += rhs.data[i];
        }

        result.data[i] = sum % 10;
        carry = sum / 10;
    }

    result.TrimZeros();
    return result;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
    *this = *this + rhs;
    return *this;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
    if (negative && !rhs.negative) {
        return -(-(*this) + rhs);
    }

    if (!negative && rhs.negative) {
        return *this + (-rhs);
    }

    if (negative && rhs.negative) {
        return (-rhs) - (-(*this));
    }

    if (*this < rhs) {
        return -(rhs - *this);
    }

    BigInt result;
    result.AllocateMemory(length);

    int32_t borrow = 0;
    for (size_t i = 0; i < length; i++) {
        int32_t diff = data[i] - borrow;
        if (i < rhs.length) {
            diff -= rhs.data[i];
        }

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.data[i] = diff;
    }

    result.TrimZeros();
    return result;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    *this = *this - rhs;
    return *this;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
    if ((length == 1 && data[0] == 0) || (rhs.length == 1 && rhs.data[0] == 0)) {
        return BigInt(0);
    }
    
    bool result_sign = negative != rhs.negative;

    BigInt result;
    result.AllocateMemory(length + rhs.length);
    result.negative = result_sign;
    
    for (size_t i = 0; i < length; i++) {
        int32_t carry = 0;
        for (size_t j = 0; j < rhs.length || carry > 0; j++) {
            int32_t multiplier = 0;
            if (j < rhs.length) {
                multiplier = rhs.data[j];
            }
            int64_t product = result.data[i + j] + data[i] * multiplier + carry;

            result.data[i + j] = product % 10;
            carry = product / 10;
        }
    }

    result.TrimZeros();

    return result;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    *this = *this * rhs;
    return *this;
}

bool BigInt::operator==(const BigInt& rhs) const {
    if (negative != rhs.negative || length != rhs.length) {
        return false;
    }

    for (size_t i = 0; i < length; i++) {
        if (data[i] != rhs.data[i]) {
            return false;
        }
    }

    return true;
}

bool BigInt::operator!=(const BigInt& rhs) const {
    return !(*this == rhs);
}

bool BigInt::operator<(const BigInt& rhs) const {
    if (negative && !rhs.negative) {
        return true;
    }

    if (!negative && rhs.negative) {
        return false;
    }

    bool abs_smaller;

    if (length != rhs.length) {
        abs_smaller = length < rhs.length;
    } else {
        abs_smaller = false;
        for (ssize_t i = static_cast<ssize_t>(length) - 1; i >= 0; i--) {
            if (data[i] != rhs.data[i]) {
                abs_smaller = data[i] < rhs.data[i];
                break;
            }
        }
    }
    if (negative) {
        abs_smaller = !abs_smaller;
    }
    return abs_smaller;
}

bool BigInt::operator<=(const BigInt& rhs) const {
    return (*this < rhs) || (*this == rhs);
}

bool BigInt::operator>(const BigInt& rhs) const {
    return !(*this <= rhs);
}

bool BigInt::operator>=(const BigInt& rhs) const {
    return (*this > rhs) || (*this == rhs);
}

std::ostream& operator<<(std::ostream& stream, const BigInt& num) {
    if (num.length == 0) {
        stream << 0;
        return stream;
    }

    if (num.negative) {
        stream << '-';
    }

    for (ssize_t i = static_cast<ssize_t>(num.length) - 1; i >= 0; i--) {
        stream << num.data[i];
    }

    return stream;
}

std::string BigInt::ToString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}