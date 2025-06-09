#ifndef UNTITLED_big_float_HPP
#define UNTITLED_big_float_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>
using big_int = boost::multiprecision::cpp_int;

class big_float{
private:
    big_int numerator_;
    big_int denominator_;

private:
    void simplify();
    void parse_number(const char *str, int &index, big_int &result);
    big_float reciprocal() const;
    big_float ln_internal(const big_float &eps) const;

public:
    static big_int power_base(const big_int& base, const big_int& exp);

public:
    static const big_float DEFAULT_EPSILON;
    
    big_float();
    big_float(const char *str);
    big_float(const big_int &numerator, const big_int &denominator);
    big_float(const big_int &other);
    big_float(int other);

public:
    big_float(const big_float &other);
    big_float(big_float &&other) noexcept;
    big_float &operator=(const big_float &other);
    big_float &operator=(big_float &&other) noexcept;
    ~big_float() = default;

public:
    big_float abs() const;

public:
    big_float operator-() const;

public:
    big_float &operator+=(const big_float &other) &;
    big_float &operator-=(const big_float &other) &;
    big_float &operator*=(const big_float &other) &;
    big_float &operator/=(const big_float &other) &;

public:
    big_float operator+(const big_float &other) const;
    big_float operator-(const big_float &other) const;
    big_float operator*(const big_float &other) const;
    big_float operator/(const big_float &other) const;
    big_float operator%(const big_float &other) const;

public:
    bool operator==(const big_float &second) const;
    bool operator!=(const big_float &second) const;
    bool operator<=(const big_float &second) const;
    bool operator>=(const big_float &second) const;
    bool operator<(const big_float &second) const;
    bool operator>(const big_float &second) const;

public:
    std::string to_decimal(int precision) const;
    friend std::ostream &operator<<(std::ostream &out, const big_float &num) noexcept;
    friend std::istream &operator>>(std::istream &in, big_float &num);

public:
    static big_float PI(const big_float &eps);
    big_float truncate() const;

public:
    big_float pow(unsigned int exp) const;
    big_float pow(big_int exp) const;
    big_float exp(const big_float &eps) const;
    big_float sqrt(const big_float &eps) const;
    static big_int factorial(const big_int &n);
    static big_int factorial(const unsigned long &n);

public:
    friend big_float sin(const big_float &base, const big_float &eps);
    friend big_float cos(const big_float &base, const big_float &eps);
    friend big_float tan(const big_float &base, const big_float &eps);
    friend big_float cot(const big_float &base, const big_float &eps);
    friend big_float sec(const big_float &base, const big_float &eps);
    friend big_float csc(const big_float &base, const big_float &eps);
    friend big_float arcsin(const big_float &base, const big_float &eps);
    friend big_float arccos(const big_float &base, const big_float &eps);
    friend big_float arctg(const big_float &base, const big_float &eps);
    friend big_float arcctg(const big_float &base, const big_float &eps);
    friend big_float arcsec(const big_float &base, const big_float &eps);
    friend big_float arccosec(const big_float &base, const big_float &eps);

public:
    friend big_float binomialCoefficient(unsigned long n, unsigned long k);
    friend big_float bernoulli(unsigned long n);

public:
    big_float log2(const big_float &eps) const;
    big_float ln(const big_float &eps) const;
    big_float log10(const big_float &eps) const;
};


void test_constructors();
void test_assignment();
void test_arithmetic();
void test_comparison();
void test_abs();
void test_modulo();
void test_pi();
void test_pow();
void test_sqrt();
void test_sin();
void test_cos();
void test_tan();
void test_cot();
void test_sec();
void test_csc();
void test_arcsin();
void test_arccos();
void test_arctg();
void test_arcctg();
void test_arcsec();
void test_arccosec();
big_int factorial(const big_int& n);
big_int power_base(const big_int &base, unsigned int exp);

#endif //UNTITLED_big_float_HPP
