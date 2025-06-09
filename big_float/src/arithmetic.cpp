#include "../inc/bigfloat.hpp"


big_float &big_float::operator+=(const big_float &other) & {
    if (denominator_ == 0 || other.denominator_ == 0) {
        throw std::invalid_argument("Zero denominator in +=");
    }
    numerator_ = numerator_ * other.denominator_ + other.numerator_ * denominator_;
    denominator_ = denominator_ * other.denominator_;
    simplify();
    return *this;
}

big_float &big_float::operator-=(const big_float &other) & {
    numerator_ = numerator_ * other.denominator_ - other.numerator_ * denominator_;
    denominator_ = denominator_ * other.denominator_;
    simplify();
    return *this;
}

big_float &big_float::operator*=(const big_float &other) & {
    numerator_ = numerator_ * other.numerator_;
    denominator_ = denominator_ * other.denominator_;
    simplify();
    return *this;
}

big_float &big_float::operator/=(const big_float &other) & {
    if (other.numerator_ == 0) {
        throw std::invalid_argument("Division by zero");
    }
    numerator_ *= other.denominator_;
    denominator_ *= other.numerator_;
    simplify();
    return *this;
}

big_float big_float::operator%(const big_float &other) const {
    big_float quotient = *this/other;
    big_int num = quotient.numerator_;
    big_int den = quotient.denominator_;
    big_int q = num/den;
    big_float result = *this - other * big_float(q);
    result.simplify();
    return result.abs();
}

big_float big_float::operator+(const big_float &other) const {
    big_float result(*this);
    return result += other;
}

big_float big_float::operator-(const big_float &other) const {
    big_float result(*this);
    return result -= other;
}

big_float big_float::operator*(const big_float &other) const {
    big_float result(*this);
    return result *= other;
}

big_float big_float::operator/(const big_float &other) const {
    big_float result(*this);
    return result /= other;
}