#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <algorithm>

class Matrix {
public:
    Matrix(size_t rows, size_t columns);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix();

    size_t getRows() const;
    size_t getColumns() const;

    class RowProxy {
    private:
        int32_t* row_data_;
        size_t column_count_;

    public:
        RowProxy() : row_data_(nullptr), column_count_(0) {}

        RowProxy(int32_t* data, size_t columns) : row_data_(data), column_count_(columns) {}

        int32_t& operator[](size_t col) {
            if (col >= column_count_) {
                throw std::out_of_range("");
            }
            return row_data_[col];
        }

        const int32_t& operator[](size_t col) const {
            if (col >= column_count_) {
                throw std::out_of_range("");
            }
            return row_data_[col];
        }
    };

    RowProxy& operator[](size_t row) {
        if (row >= row_count_) {
            throw std::out_of_range("");
        }
        return row_proxies_[row];
    }

    const RowProxy& operator[](size_t row) const {
        if (row >= row_count_) {
            throw std::out_of_range("");
        }
        return row_proxies_[row];
    }

    Matrix& operator*=(int32_t multiplier);
    Matrix operator+(const Matrix& other) const;

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    void swap(Matrix& other) noexcept;

private:
    size_t row_count_;
    size_t column_count_;
    int32_t* data_;
    RowProxy* row_proxies_;
};

#endif
