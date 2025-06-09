#include "../inc/bigfloat.hpp"

big_float big_float::ln(const big_float &eps) const {
    if (*this <= big_float(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }

    big_float x = *this;
    big_int k(0);

    // Сведение аргумента к диапазону [0.5, 1.0] делением на степени 2
    while (x > big_float(1)) {
        x /= big_float(2);
        k += 1;
    }
    while (x < big_float(0.5)) {
        x *= big_float(2);
        k -= 1;
    }

    // Вычисление ln(2) с нужной точностью (только один раз!)
    static big_float ln2 = big_float(2).ln_internal(eps); // Используем внутреннюю функцию для вычисления ln(2)

    // Вычисление ln(x) для x в диапазоне [0.5, 1.0] с помощью альтернативного ряда
    big_float w = (x - big_float(1)) / (x + big_float(1));
    big_float w_squared = w * w;
    big_float term = w;
    big_float result = w;
    big_float n = big_float(3);

    while (term.abs() > eps) {
        term *= w_squared;
        result += term / n;
        n += big_float(2);
    }

    return result * big_float(2) + big_float(k) * ln2; // ln(x) = ln(a) + k * ln(2)
}

// Внутренняя функция для вычисления ln(x) без проверки аргументов, нужна для вычисления ln(2)
big_float big_float::ln_internal(const big_float &eps) const {
    big_float x = *this;
    big_float w = (x - big_float(1)) / (x + big_float(1));
    big_float w_squared = w * w;
    big_float term = w;
    big_float result = w;
    big_float n = big_float(3);

    while (term.abs() > eps) {
        term *= w_squared;
        result += term / n;
        n += big_float(2);
    }

    return result * big_float(2);
}

big_float big_float::log2(const big_float &eps) const {
    if (*this <= big_float(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }
    big_float ln2 = big_float(2).ln(eps);
    return this->ln(eps) / ln2;
}

big_float big_float::log10(const big_float &eps) const {
    if (*this <= big_float(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }
    big_float ln10 = big_float(10).ln(eps);
    return this->ln(eps) / ln10;
}