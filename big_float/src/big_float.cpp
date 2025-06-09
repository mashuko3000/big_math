#include "../inc/bigfloat.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <cctype>
#include <stdexcept>
using big_int = boost::multiprecision::cpp_int;
const big_float big_float::DEFAULT_EPSILON = big_float("1/10000");

big_float::big_float() : numerator_(0), denominator_(1) {}

big_float::big_float(const char *str) {
    if (!str || str[0] == '\0') {
        throw std::invalid_argument("Empty input string");
    }

    int index = 0;
    while (str[index] == ' ') {
        ++index;
    }

    int slash_index = -1;
    int len = index;
    while (str[len] != '\0') {
        if (str[len] == '/') {
            slash_index = len;
            break;
        }
        ++len;
    }

    try {
        if (slash_index == -1) {
            parse_number(str, index, numerator_);
            denominator_ = 1;
        } else {
            parse_number(str, index, numerator_);
            while (str[index] == ' ') {
                ++index;
            }
            if (str[index] != '/') {
                throw std::invalid_argument("Expected '/' after numerator");
            }
            ++index;
            while (str[index] == ' ') {
                ++index;
            }
            parse_number(str, index, denominator_);
            if (denominator_ == 0) {
                throw std::invalid_argument("Denominator cannot be zero");
            }
        }

        while (str[index] == ' ') {
            ++index;
        }
        if (str[index] != '\0') {
            throw std::invalid_argument("Invalid characters after number");
        }

        simplify();
    } catch (...) {
        throw;
    }
}

big_float::big_float(const big_int &numerator, const big_int &denominator)
        : numerator_(numerator), denominator_(denominator) {
    simplify();
}

big_float::big_float(const big_int &other) : numerator_(other), denominator_(1) {}

big_float::big_float(int other) : numerator_(other), denominator_(1) {
    simplify();
}

big_float::big_float(const big_float &other)
        : numerator_(other.numerator_), denominator_(other.denominator_) {}

big_float::big_float(big_float &&other) noexcept
        : numerator_(std::move(other.numerator_)), denominator_(std::move(other.denominator_)) {}

big_float &big_float::operator=(const big_float &other) {
    if (this != &other) {
        numerator_ = other.numerator_;
        denominator_ = other.denominator_;
    }
    return *this;
}

big_float &big_float::operator=(big_float &&other) noexcept {
    if (this != &other) {
        numerator_ = std::move(other.numerator_);
        denominator_ = std::move(other.denominator_);
    }
    return *this;
}


