#include "../inc/bigfloat.hpp"

std::ostream &operator<<(std::ostream &out, const big_float &num) noexcept {

    if (num.numerator_ == 0) {
        out << "0";
        return out;
    }
/*
    if (num.denominator_ < 0) {
        out << "-";
    } else {
        out << "";
    }
    out << num.numerator_ << "/" << abs(num.denominator_) << std::endl;
*/
    out << num.to_decimal(17);
    return out;
}

std::istream &operator>>(std::istream &in, big_float &num) {
    size_t capacity = 16;
    size_t length = 0;
    char *buffer = new char[capacity];

    try {
        char c;
        while (in.get(c)) {
            if (c == '\n' || c == '\r' || c == '\t') {
                continue;
            }
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
            buffer[length++] = c;
            if (c == ' ' || in.peek() == EOF || in.peek() == '\n') {
                break;
            }
        }
        buffer[length] = '\0';

        int slash_count = 0;
        for (size_t i = 0; buffer[i] != '\0'; ++i) {
            if (buffer[i] == '/') {
                slash_count++;
            }
        }
        if (slash_count > 1) {
            delete[] buffer;
            throw std::invalid_argument("Multiple slashes in input");
        }
        num = big_float(buffer);
        delete[] buffer;
        return in;
    } catch (...) {
        delete[] buffer;
        throw;
    }
}

std::string big_float::to_decimal(int precision) const {
    boost::multiprecision::cpp_dec_float_100 result = (numerator_.convert_to<boost::multiprecision::cpp_dec_float_100>()) / denominator_.convert_to<boost::multiprecision::cpp_dec_float_100>();
    return result.str(precision, std::ios_base::fixed);
}