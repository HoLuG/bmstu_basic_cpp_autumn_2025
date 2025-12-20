#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <string>
#include <iostream>
#include <cstdint>

class BigInt {
private:
    int32_t *data;
    size_t length;
    bool negative;

    void TrimZeros();
    void AllocateMemory(size_t new_length);
    void swap(BigInt& other) noexcept;

public:
    BigInt();
    BigInt(int32_t value);
    BigInt(const std::string& str);
    BigInt(const BigInt& rhs);
    BigInt(BigInt&& rhs) noexcept;

    ~BigInt();

    BigInt& operator=(const BigInt& rhs);
    BigInt& operator=(BigInt&& rhs) noexcept;

    BigInt operator-() const;

    BigInt operator+(const BigInt& rhs) const;
    BigInt& operator+=(const BigInt& rhs);

    BigInt operator-(const BigInt& rhs) const;
    BigInt& operator-=(const BigInt& rhs);

    BigInt operator*(const BigInt& rhs) const;
    BigInt& operator*=(const BigInt& rhs);

    bool operator==(const BigInt& rhs) const;
    bool operator!=(const BigInt& rhs) const;
    bool operator<(const BigInt& rhs) const;
    bool operator<=(const BigInt& rhs) const;
    bool operator>(const BigInt& rhs) const;
    bool operator>=(const BigInt& rhs) const;

    friend std::ostream& operator<<(std::ostream& stream, const BigInt& num);

    std::string ToString() const;
};

#endif