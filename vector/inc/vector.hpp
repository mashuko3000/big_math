#ifndef UNTITLED_vec_HPP
#define UNTITLED_vec_HPP

#include "bigfloat.hpp"


class vec{
    struct node{
        unsigned long long index_;
        big_float coef_;
        node* next;
        node(const unsigned long long idx, const big_float coef) : index_(idx), coef_(coef), next(nullptr) {}
    };
    node* head_;
    size_t dimension_;
    big_float epsilon_;

public:
    vec(size_t dim, const big_float& eps);
    vec(const big_float* coords, size_t dim, const big_float& eps = big_float::DEFAULT_EPSILON);
    vec(const char* str, const big_float& eps = big_float::DEFAULT_EPSILON);

public:
    vec(const vec& other);
    vec& operator=(const vec& other);
    vec(vec&& other) noexcept;
    vec& operator=(vec&& other) noexcept;
    ~vec() noexcept;

public:
    bool operator==(const vec& other) const;
    bool operator!=(const vec& other) const;

public:
    friend std::ostream& operator<<(std::ostream& os, const vec& v);
    friend std::istream& operator>>(std::istream& is, vec& v);

public:
    big_float& operator[](size_t index);
    const big_float& operator[](size_t index) const;
    const big_float get_coeff(size_t index) const;

public:
    vec operator+(const vec& other) const &;
    vec operator+(const vec& other) &&;
    vec& operator+=(const vec& other);

    vec operator-(const vec& other) const &;
    vec operator-(const vec& other) &&;
    vec& operator-=(const vec& other);

    big_float operator*(const vec& other) const;
    vec operator*(const big_float& scalar) const;
    vec& operator*=(const big_float& scalar);

    vec operator/(const big_float& scalar) const;

public:
    vec operator-() const;

public:
    big_float length() const;
    vec normalize() const &;
    vec normalize() &&;
    bool are_orthogonal(const vec& other) const;
    bool are_parallel(const vec& other) const;

public:
    big_float dot_product_by_angle(const vec& other) const;
    big_float angle_cos(const vec& other) const;
    vec projection(const vec& other) const;

public:
    vec cross_product_3d(const vec& other) const &;
    big_float mixed_product_3d(const vec& v1, const vec& v2) const;
/*
public:
    vec cross_product_7d(const vec& other) const;
    big_float mixed_product_7d(const vec& v1, const vec& v2, const vec& v3,
                               const vec& v4, const vec& v5, const vec& v6) const;
*/
public:
    size_t get_dimension() const { return dimension_; }

private:
    // Helpers
    void clear();
    void copy(const vec& other);
    void move(vec&& other) noexcept;
    void insert_sorted(size_t index, const big_float& coeff);
    void parse_string(const char* str);
    bool is_valid_index(size_t index) const { return index <= dimension_; }
};


#endif //UNTITLED_vec_HPP
