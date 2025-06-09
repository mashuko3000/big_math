#include "../inc/bigfloat.hpp"

using big_int = boost::multiprecision::cpp_int;

void big_float::simplify() {
    if (denominator_ == 0) {
        throw std::invalid_argument("Denominator cannot be zero");
    }
    big_int gcd = boost::multiprecision::gcd(numerator_, denominator_);
    numerator_ /= gcd;
    denominator_ /= gcd;

    if (numerator_ < 0 && denominator_ > 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    } else if (numerator_ < 0 && denominator_ < 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
}

void big_float::parse_number(const char *str, int &index, big_int &result) {
    if (str == nullptr) {
        throw std::runtime_error("input string's empty");
    }

    size_t capacity = 16;
    size_t length = 0;
    char *buffer = new char[capacity];
    bool negative = false;

    try {
        while (str[index] == ' ') {
            ++index;
        }

        if (str[index] == '-') {
            negative = true;
            ++index;
        } else if (str[index] == '+') {
            negative = false;
            ++index;
        }

        if (!std::isdigit(str[index]) && str[index] != '\0') {
            delete[] buffer;
            throw std::invalid_argument("No digits found in number");
        }

        if (!std::isdigit(str[index])) {
            buffer[0] = '0';
            buffer[1] = '\0';
            length = 1;
        } else {
            while (std::isdigit(str[index])) {
                if (length + 1 >= capacity) {
                    size_t new_capacity = capacity * 2;
                    char *new_buffer = new char[new_capacity];
                    for (size_t i = 0; i < length; ++i) {
                        new_buffer[i] = buffer[i];
                    }
                    delete[] buffer;
                    buffer = new_buffer;
                    capacity = new_capacity;
                }
                buffer[length++] = str[index++];
            }
            buffer[length] = '\0';
        }
        result = big_int(buffer);
        if (negative) {
            result = -result;
        }
        delete[] buffer;
    } catch (...) {
        delete[] buffer;
        throw;
    }
}

big_float big_float::abs() const {
    return big_float(boost::multiprecision::abs(numerator_), boost::multiprecision::abs(denominator_));
}

big_float big_float::operator-() const {
    return big_float(numerator_, -denominator_);
}

big_int big_float::factorial(const big_int &n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }
    if (n == 0 || n == 1) {
        return big_int(1);
    }
    big_int result = 1;
    for (big_int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

big_int big_float::factorial(const unsigned long &n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }
    if (n == 0 || n == 1) {
        return big_int(1);
    }
    big_int result = 1;
    for (big_int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

big_float big_float::pow(unsigned int exp) const {
    if (exp == 0) {
        if (numerator_ == 0) {
            throw std::invalid_argument("Zero base with zero exponent is undefined");
        }
        return big_float(1);
    }
    big_float result(1);
    big_float base(*this);
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    result.simplify();
    return result;
}

big_float big_float::reciprocal() const {
    if (numerator_ == 0) {
        throw std::invalid_argument("Division by zero");
    }
    big_float result(denominator_, numerator_);
    result.simplify();
    return result;
}

big_float big_float::sqrt(const big_float &eps) const {
    if (*this < big_float(0)) {
        throw std::invalid_argument("Cannot calculate square root of a negative number");
    }
    if (*this == big_float(0)) {
        return big_float(0);
    }

    double approx_double = (double)numerator_.convert_to<double>() / denominator_.convert_to<double>();
    if (approx_double <= 0.0) {
        throw std::invalid_argument("Cannot calculate square root of non-positive number");
    }
    approx_double = std::sqrt(approx_double);

    big_float guess(approx_double);
    if (guess == big_float(0)) {
        guess = big_float(1);
    }

    big_float two(2);
    big_float prev_guess;
    int max_iter = 1000000;
    int iter = 0;

    do {
        prev_guess = guess;
        if (guess == big_float(0)) {
            throw std::runtime_error("sqrt: guess became zero, division by zero avoided");
        }
        guess = (guess + (*this) / guess) / two;

        big_float diff = (guess - prev_guess).abs();
        big_float rel_diff = diff / guess.abs();
        if (rel_diff < eps && iter > 0) {
            break;
        }

        iter++;
        if (iter > max_iter) {
            throw std::runtime_error("sqrt: did not converge within max iterations");
        }
    } while (true);

    return guess;
}

big_int factorial(const big_int& n)
{
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }

    if (n == 0 || n == 1)
        return 1;

    big_int result = 1;
    for(big_int i = 2; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}

/*
big_int power_base(const big_int &base, unsigned int exp) {
    big_int result = 1;
    big_int current = base;
    while (exp > 0) {
        if (exp & 1) {
            result *= current;
        }
        current *= current;
        exp >>= 1;
    }
    return result;
}
*/
/*
big_int big_float::power_base(const big_int& base, const big_int& exp) {
    if (exp < 0) throw std::invalid_argument("Negative exponent not supported");
    if (exp == 0) return 1;

    big_int result = 1;
    big_int current = base;
    big_int e = exp;

    while (e > 0) {
        if ((e % 2) == 1) result *= current;
        current *= current;
        e /= 2;
    }
    return result;
}
*/