#include "vector.hpp"

Vector<bool>::BitReference::BitReference(unsigned char& byte, unsigned char bit_pos)
    : byte(byte), mask(1 << bit_pos) {}

Vector<bool>::BitReference& Vector<bool>::BitReference::operator=(bool value) {
    if (value) {
        byte |= mask;
    } else {
        byte &= ~mask;
    }
    return *this;
}

Vector<bool>::BitReference& Vector<bool>::BitReference::operator=(const BitReference& other) {
    return *this = static_cast<bool>(other);
}

Vector<bool>::BitReference::operator bool() const {
    return (byte & mask) != 0;
}

Vector<bool>::Vector() : data_ptr(nullptr), elem_count(0), reserved_space(0) {}

Vector<bool>::Vector(size_t count) : elem_count(count), reserved_space(count) {
    data_ptr = new unsigned char[byte_count(count)]();
}

Vector<bool>::Vector(size_t count, bool value) : elem_count(count), reserved_space(count) {
    size_t bytes = byte_count(count);
    data_ptr = new unsigned char[bytes]();

    if (value) {
        for (size_t i = 0; i < bytes; i++) {
            data_ptr[i] = 0xFF;
        }

        size_t bits_in_last_byte = count % 8;
        if (bits_in_last_byte != 0) {
            data_ptr[bytes - 1] = (1 << bits_in_last_byte) - 1;
        }
    }
}

Vector<bool>::Vector(const Vector& other) : elem_count(other.elem_count), reserved_space(other.reserved_space) {
    size_t bytes = byte_count(reserved_space);
    data_ptr = new unsigned char[bytes];
    for (size_t i = 0; i < bytes; i++) {
        data_ptr[i] = other.data_ptr[i];
    }
}

Vector<bool>::Vector(Vector&& other) noexcept : data_ptr(other.data_ptr), elem_count(other.elem_count), reserved_space(other.reserved_space) {
    other.data_ptr = nullptr;
    other.elem_count = 0;
    other.reserved_space = 0;
}

Vector<bool>::~Vector() {
    delete[] data_ptr;
}

Vector<bool>& Vector<bool>::operator=(const Vector& other) {
    if (this != &other) {
        delete[] data_ptr;

        elem_count = other.elem_count;
        reserved_space = other.reserved_space;

        size_t bytes = byte_count(reserved_space);
        data_ptr = new unsigned char[bytes];
        for (size_t i = 0; i < bytes; i++) {
            data_ptr[i] = other.data_ptr[i];
        }
    }
    return *this;
}

Vector<bool>& Vector<bool>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        delete[] data_ptr;

        data_ptr = other.data_ptr;
        elem_count = other.elem_count;
        reserved_space = other.reserved_space;

        other.data_ptr = nullptr;
        other.elem_count = 0;
        other.reserved_space = 0;
    }
    return *this;
}

Vector<bool>::BitReference Vector<bool>::operator[](size_t pos) {
    return BitReference(data_ptr[pos / 8], pos % 8);
}

bool Vector<bool>::operator[](size_t pos) const {
    return (data_ptr[pos / 8] & (1 << (pos % 8))) != 0;
}

bool Vector<bool>::front() const {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return (*this)[0];
}

Vector<bool>::BitReference Vector<bool>::front() {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return (*this)[0];
}

bool Vector<bool>::back() const {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return (*this)[elem_count - 1];
}

Vector<bool>::BitReference Vector<bool>::back() {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return (*this)[elem_count - 1];
}

unsigned char* Vector<bool>::data() {
    return data_ptr;
}

const unsigned char* Vector<bool>::data() const {
    return data_ptr;
}

bool Vector<bool>::empty() const {
    return elem_count == 0;
}

size_t Vector<bool>::size() const {
    return elem_count;
}

size_t Vector<bool>::max_size() const {
    return std::numeric_limits<size_t>::max();
}

void Vector<bool>::reserve(size_t new_cap) {
    if (new_cap > reserved_space) {
        size_t new_bytes = byte_count(new_cap);
        size_t old_bytes = byte_count(elem_count);

        unsigned char* new_data = new unsigned char[new_bytes]();

        for (size_t i = 0; i < old_bytes; i++) {
            new_data[i] = data_ptr[i];
        }

        delete[] data_ptr;
        data_ptr = new_data;
        reserved_space = new_cap;
    }
}

size_t Vector<bool>::capacity() const {
    return reserved_space;
}

void Vector<bool>::shrink_to_fit() {
    if (elem_count < reserved_space) {
        if (elem_count == 0) {
            delete[] data_ptr;
            data_ptr = nullptr;
            reserved_space = 0;
        } else {
            size_t new_bytes = byte_count(elem_count);
            unsigned char* new_data = new unsigned char[new_bytes];
            size_t old_bytes = byte_count(reserved_space);
            size_t copy_bytes = (new_bytes < old_bytes) ? new_bytes : old_bytes;
            
            for (size_t i = 0; i < copy_bytes; i++) {
                new_data[i] = data_ptr[i];
            }
            delete[] data_ptr;
            data_ptr = new_data;
            reserved_space = elem_count;
        }
    }
}

void Vector<bool>::push_back(bool value) {
    if (elem_count == reserved_space) {
        reserve(reserved_space == 0 ? 8 : reserved_space * 2);
    }

    (*this)[elem_count] = value;
    elem_count++;
}

void Vector<bool>::emplace(size_t pos, bool value) {
    insert(pos, value);
}

void Vector<bool>::emplace_back(bool value) {
    push_back(value);
}

void Vector<bool>::pop_back() {
    if (elem_count > 0) {
        elem_count--;
    }
}

void Vector<bool>::insert(size_t pos, bool value) {
    if (pos > elem_count) {
        throw std::out_of_range("Position out of range");
    }

    if (elem_count == reserved_space) {
        reserve(reserved_space == 0 ? 8 : reserved_space * 2);
    }

    for (size_t i = elem_count; i > pos; i--) {
        (*this)[i] = (*this)[i - 1];
    }

    (*this)[pos] = value;
    elem_count++;
}

void Vector<bool>::resize(size_t count) {
    resize(count, false);
}

void Vector<bool>::resize(size_t count, bool value) {
    if (count > reserved_space) {
        reserve(count);
    }

    if (count > elem_count) {
        for (size_t i = elem_count; i < count; i++) {
            (*this)[i] = value;
        }
    }

    elem_count = count;
}

void Vector<bool>::reverse() {
    if (elem_count <= 1) {
        return;
    }
    size_t left = 0;
    size_t right = elem_count - 1;
    while (left < right) {
        bool temp = (*this)[left];
        (*this)[left] = (*this)[right];
        (*this)[right] = temp;
        left++;
        right--;
    }
}

size_t Vector<bool>::byte_count(size_t bit_count) const {
    return (bit_count + 7) / 8;
}
