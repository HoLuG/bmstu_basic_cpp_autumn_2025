#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdexcept>
#include <iostream>
#include <cstdint>

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

        explicit RowProxy(size_t columns) : column_count_(columns) {
            row_data_ = new int32_t[column_count_];
            for (size_t i = 0; i < column_count_; i++) {
                row_data_[i] = 0;
            }
        }

        RowProxy(const RowProxy& other) : column_count_(other.column_count_) {
            row_data_ = new int32_t[column_count_];
            for (size_t i = 0; i < column_count_; i++) {
                row_data_[i] = other.row_data_[i];
            }
        }

        RowProxy& operator=(const RowProxy& other) {
            if (this != &other) {
                int32_t* new_data = new int32_t[other.column_count_];
                for (size_t i = 0; i < other.column_count_; i++) {
                    new_data[i] = other.row_data_[i];
                }
                delete[] row_data_;
                row_data_ = new_data;
                column_count_ = other.column_count_;
            }
            return *this;
        }

        ~RowProxy() {
            delete[] row_data_;
        }

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

private:
    size_t row_count_;
    size_t column_count_;
    RowProxy* row_proxies_;
};

#endif
