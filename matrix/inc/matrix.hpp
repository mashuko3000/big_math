#ifndef UNTITLED_matrix_HPP
#define UNTITLED_matrix_HPP

#include "bigfloat.hpp"
class vec;

class matrix{
    class matrix_proxy{
        matrix& matrix_;
        size_t row_;
        size_t col_;
    public:
        matrix_proxy(matrix& m, size_t row) : matrix_(m), row_(row) {}
        matrix_proxy& operator[](size_t col) {
            col_ = col;
            return *this;
        }
        matrix_proxy& operator=(const big_float& value) {
            matrix_.set_value(row_, col_, value);
            return *this;
        }
        operator big_float() const {
            return matrix_.get_value(row_, col_);
        }
    };

    big_float** data_;
    size_t row_, col_;
    big_float epsilon_;

public:
    matrix(size_t rows, size_t cols, const big_float& eps = big_float::DEFAULT_EPSILON);
    matrix(const big_float* data, size_t rows, size_t cols, const big_float& eps = big_float::DEFAULT_EPSILON);
    matrix(const char* str, const big_float& eps = big_float::DEFAULT_EPSILON);

public:
    matrix(const matrix& other);
    matrix(matrix&& other) noexcept;
    matrix& operator=(const matrix& other);
    matrix& operator=(const matrix&& other);
    ~matrix() noexcept;

public:
    matrix_proxy operator[](size_t row);
    const big_float get_value(size_t row, size_t col) const;

public:
    matrix operator+(const matrix& other) const;
    matrix& operator+=(const matrix& other);
    matrix operator-(const matrix& other) const;
    matrix& operator-=(const matrix& other);
    matrix operator*(const matrix& other) const;
    matrix operator*(const big_float& scalar) const;
    matrix& operator*=(const matrix& other);
    matrix& operator*=(const big_float& scalar);

public:
    matrix transpose() const;

    big_float determinant() const;
    matrix inverse() const;
    size_t rank() const;
    vec gauss_solve(const vec& b) const;

public:
    bool operator==(const matrix& other) const;
    bool operator!=(const matrix& other) const;

public:
    friend std::ostream& operator<<(std::ostream& os, const matrix& m);
    friend std::istream& operator>>(std::istream& is, matrix& m);

public:
    size_t get_rows() const { return row_; }
    size_t get_cols() const { return col_; }

private:
    void clear();
    void copy(const matrix& other);
    void move(matrix&& other);
    void set_value(size_t row, size_t col, const big_float& value);
    void parse_string(const char* str);
    bool is_valid_index(size_t row, size_t col) const {return row < row_ && col < col_;}
};

#endif //UNTITLED_matrix_HPP
