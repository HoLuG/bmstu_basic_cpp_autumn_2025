#include "matrix.hpp"

Matrix::Matrix(size_t rows, size_t columns) 
    : row_count_(rows), column_count_(columns) {
    data_ = new int32_t[row_count_ * column_count_];
    for (size_t i = 0; i < row_count_ * column_count_; i++) {
        data_[i] = 0;
    }
    
    row_proxies_ = new RowProxy[row_count_];
    for (size_t i = 0; i < row_count_; i++) {
        row_proxies_[i] = RowProxy(data_ + i * column_count_, column_count_);
    }
}

Matrix::Matrix(const Matrix& other) 
    : row_count_(other.row_count_), column_count_(other.column_count_) {
    data_ = new int32_t[row_count_ * column_count_];
    for (size_t i = 0; i < row_count_ * column_count_; i++) {
        data_[i] = other.data_[i];
    }
    
    row_proxies_ = new RowProxy[row_count_];
    for (size_t i = 0; i < row_count_; i++) {
        row_proxies_[i] = RowProxy(data_ + i * column_count_, column_count_);
    }
}

void Matrix::swap(Matrix& other) noexcept {
    std::swap(row_count_, other.row_count_);
    std::swap(column_count_, other.column_count_);
    std::swap(data_, other.data_);
    std::swap(row_proxies_, other.row_proxies_);
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        Matrix temp(other);
        swap(temp);
    }
    return *this;
}

Matrix::~Matrix() {
    delete[] data_;
    delete[] row_proxies_;
}

size_t Matrix::getRows() const {
    return row_count_;
}

size_t Matrix::getColumns() const {
    return column_count_;
}

Matrix& Matrix::operator*=(int32_t multiplier) {
    for (size_t i = 0; i < row_count_; i++) {
        for (size_t j = 0; j < column_count_; j++) {
            row_proxies_[i][j] *= multiplier;
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (row_count_ != other.row_count_ || column_count_ != other.column_count_) {
        throw std::invalid_argument("matrix dimensions must be equal");
    }

    Matrix result(row_count_, column_count_);
    for (size_t i = 0; i < row_count_; i++) {
        for (size_t j = 0; j < column_count_; j++) {
            result[i][j] = row_proxies_[i][j] + other.row_proxies_[i][j];
        }
    }
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if (row_count_ != other.row_count_ || column_count_ != other.column_count_) {
        return false;
    }
    for (size_t i = 0; i < row_count_; i++) {
        for (size_t j = 0; j < column_count_; j++) {
            if (row_proxies_[i][j] != other.row_proxies_[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.row_count_; i++) {
        for (size_t j = 0; j < matrix.column_count_; j++) {
            os << matrix[i][j];
            if (j < matrix.column_count_ - 1) {
                os << " ";
            }
        }
        if (i < matrix.row_count_ - 1) {
            os << std::endl;
        }
    }
    return os;
}
