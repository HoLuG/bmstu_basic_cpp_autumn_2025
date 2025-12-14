#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <limits>
#include <utility>

template <typename T>
class Vector {
private:
    T *data_ptr;
    size_t elem_count;
    size_t reserved_space;

public:
    Vector();
    explicit Vector(size_t count);
    Vector(size_t count, const T& value);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data();
    const T* data() const;

    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void reserve(size_t new_cap);
    size_t capacity() const;
    void shrink_to_fit();

    void push_back(const T& value);
    void push_back(T&& value);
    template <typename... Args>
    void emplace(size_t pos, Args&&... args);
    template <typename... Args>
    void emplace_back(Args&&... args);
    void pop_back();
    void insert(size_t pos, const T& value);
    void resize(size_t count);
    void resize(size_t count, const T& value);
    void reverse();
};

template <>
class Vector<bool> {
private:
    unsigned char *data_ptr;
    size_t elem_count;
    size_t reserved_space;

    class BitReference {
    private:
        unsigned char& byte;
        unsigned char mask;

    public:
        BitReference(unsigned char& byte, unsigned char bit_pos);
        BitReference& operator=(bool value);
        BitReference& operator=(const BitReference& other);
        operator bool() const;
    };

public:
    Vector();
    explicit Vector(size_t count);
    Vector(size_t count, bool value);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    BitReference operator[](size_t pos);
    bool operator[](size_t pos) const;
    bool front() const;
    BitReference front();
    bool back() const;
    BitReference back();
    unsigned char* data();
    const unsigned char* data() const;

    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void reserve(size_t new_cap);
    size_t capacity() const;
    void shrink_to_fit();

    void push_back(bool value);
    void emplace(size_t pos, bool value);
    void emplace_back(bool value);
    void pop_back();
    void insert(size_t pos, bool value);
    void resize(size_t count);
    void resize(size_t count, bool value);
    void reverse();

private:
    size_t byte_count(size_t bit_count) const;
};

template <typename T>
Vector<T>::Vector() : data_ptr(nullptr), elem_count(0), reserved_space(0) {}

template <typename T>
Vector<T>::Vector(size_t count) : elem_count(count), reserved_space(count) {
    data_ptr = new T[count]();
}

template <typename T>
Vector<T>::Vector(size_t count, const T& value) : elem_count(count), reserved_space(count) {
    data_ptr = new T[count];
    for (size_t i = 0; i < count; i++) {
        data_ptr[i] = value;
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other) : elem_count(other.elem_count), reserved_space(other.reserved_space) {
    if (reserved_space > 0) {
        data_ptr = new T[reserved_space];
        size_t n = elem_count;
        for (size_t i = 0; i < n; i++) {
            data_ptr[i] = other.data_ptr[i];
        }
    } else {
        data_ptr = nullptr;
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : data_ptr(other.data_ptr), elem_count(other.elem_count), reserved_space(other.reserved_space) {
    other.data_ptr = nullptr;
    other.elem_count = 0;
    other.reserved_space = 0;
}

template <typename T>
Vector<T>::~Vector() {
    delete[] data_ptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        delete[] data_ptr;

        elem_count = other.elem_count;
        reserved_space = other.reserved_space;
        
        if (reserved_space > 0) {
            data_ptr = new T[reserved_space];
            size_t n = elem_count;
            for (size_t i = 0; i < n; i++) {
                data_ptr[i] = other.data_ptr[i];
            }
        } else {
            data_ptr = nullptr;
        }
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
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

template <typename T>
T& Vector<T>::operator[](size_t pos) {
    return data_ptr[pos];
}

template <typename T>
const T& Vector<T>::operator[](size_t pos) const {
    return data_ptr[pos];
}

template <typename T>
T& Vector<T>::front() {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_ptr[0];
}

template <typename T>
const T& Vector<T>::front() const {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_ptr[0];
}

template <typename T>
T& Vector<T>::back() {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_ptr[elem_count - 1];
}

template <typename T>
const T& Vector<T>::back() const {
    if (elem_count == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_ptr[elem_count - 1];
}

template <typename T>
T* Vector<T>::data() {
    return data_ptr;
}

template <typename T>
const T* Vector<T>::data() const {
    return data_ptr;
}

template <typename T>
bool Vector<T>::empty() const {
    return elem_count == 0;
}

template <typename T>
size_t Vector<T>::size() const {
    return elem_count;
}

template <typename T>
size_t Vector<T>::max_size() const {
    return std::numeric_limits<size_t>::max() / sizeof(T);
}

template <typename T>
void Vector<T>::reserve(size_t new_cap) {
    if (new_cap > reserved_space) {
        T* new_data = new T[new_cap];
        size_t n = elem_count;
        if (data_ptr != nullptr && n > 0) {
            for (size_t i = 0; i < n; i++) {
                new_data[i] = data_ptr[i];
            }
        }
        delete[] data_ptr;
        data_ptr = new_data;
        reserved_space = new_cap;
    }
}

template <typename T>
size_t Vector<T>::capacity() const {
    return reserved_space;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (elem_count < reserved_space) {
        if (elem_count == 0) {
            delete[] data_ptr;
            data_ptr = nullptr;
            reserved_space = 0;
        } else {
            T* new_data = new T[elem_count];
            size_t n = elem_count;
            for (size_t i = 0; i < n; i++) {
                new_data[i] = data_ptr[i];
            }
            delete[] data_ptr;
            data_ptr = new_data;
            reserved_space = elem_count;
        }
    }
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (elem_count == reserved_space) {
        size_t new_cap = (reserved_space == 0) ? 1 : reserved_space * 2;
        reserve(new_cap);
    }
    data_ptr[elem_count++] = value;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (elem_count == reserved_space) {
        size_t new_cap = (reserved_space == 0) ? 1 : reserved_space * 2;
        reserve(new_cap);
    }
    data_ptr[elem_count++] = std::move(value);
}

template <typename T>
template <typename... Args>
void Vector<T>::emplace(size_t pos, Args&&... args) {
    if (pos > elem_count) {
        throw std::out_of_range("Position out of range");
    }

    if (elem_count == reserved_space) {
        size_t new_cap = (reserved_space == 0) ? 1 : reserved_space * 2;
        reserve(new_cap);
    }

    for (size_t i = elem_count; i > pos; i--) {
        data_ptr[i] = std::move(data_ptr[i - 1]);
    }

    data_ptr[pos] = T(std::forward<Args>(args)...);
    elem_count++;
}

template <typename T>
template <typename... Args>
void Vector<T>::emplace_back(Args&&... args) {
    if (elem_count == reserved_space) {
        size_t new_cap = (reserved_space == 0) ? 1 : reserved_space * 2;
        reserve(new_cap);
    }
    data_ptr[elem_count++] = T(std::forward<Args>(args)...);
}

template <typename T>
void Vector<T>::pop_back() {
    if (elem_count > 0) {
        elem_count--;
    }
}

template <typename T>
void Vector<T>::insert(size_t pos, const T& value) {
    if (pos > elem_count) {
        throw std::out_of_range("Position out of range");
    }

    if (elem_count == reserved_space) {
        size_t new_cap = (reserved_space == 0) ? 1 : reserved_space * 2;
        reserve(new_cap);
    }

    for (size_t i = elem_count; i > pos; i--) {
        data_ptr[i] = std::move(data_ptr[i - 1]);
    }

    data_ptr[pos] = value;
    elem_count++;
}

template <typename T>
void Vector<T>::resize(size_t count) {
    if (count > reserved_space) {
        reserve(count);
    }

    for (size_t i = elem_count; i < count; i++) {
        data_ptr[i] = T();
    }

    elem_count = count;
}

template <typename T>
void Vector<T>::resize(size_t count, const T& value) {
    if (count > reserved_space) {
        reserve(count);
    }

    for (size_t i = elem_count; i < count; i++) {
        data_ptr[i] = value;
    }

    elem_count = count;
}

template <typename T>
void Vector<T>::reverse() {
    if (elem_count <= 1) {
        return;
    }
    size_t left = 0;
    size_t right = elem_count - 1;
    while (left < right) {
        T temp = data_ptr[left];
        data_ptr[left] = data_ptr[right];
        data_ptr[right] = temp;
        left++;
        right--;
    }
}

#endif
