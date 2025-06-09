#include "../inc/bigfloat.hpp"

big_float big_float::ln(const big_float &eps) const {
    if (*this <= big_float(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }
    if (eps <= big_float(0)) {
        throw std::invalid_argument("Epsilon must be positive for ln computation");
    }

    // handle case: ln(1) = 0
    if (*this == big_float(1)) {
        return big_float(0);
    }

    // scale x to a in [0.5, 1] by dividing/multiplying by 2
    big_float x = *this;
    big_int k(0);
    while (x > big_float(1)) {
        x /= big_float(2);
        k += 1;
    }
    while (x < big_float("0.5")) {
        x *= big_float(2);
        k -= 1;
    }

    // calculate ln(2) with sufficient precision (cached statically)
    static big_float ln2 = big_float(0);
    static big_float last_eps = big_float(0);
    if (ln2 == big_float(0) || eps < last_eps) {
        ln2 = big_float(2).ln_internal(eps / big_float(2));
        last_eps = eps;
    }

    // calculte ln(a) using alternative series (Lebnitsa)
    // w = (a-1)/(a+1), so ln(a) = ln((1+w)/(1-w)) = 2 * sum(w^(2n+1)/(2n+1))
    big_float w = (x - big_float(1)) / (x + big_float(1));
    big_float w_squared = w.pow(2);
    big_float term = w;
    big_float result = w;
    big_float n = big_float(3); // denominator for next term
    int max_iter = 1000000;
    int iter = 0;

    // sum series until term is smaller than eps
    while (term.abs() > eps) {
        term *= w_squared; // x^(2n+1) = x^(2n-1) * x^2
        result += term / n; // x^(2n+1)/(2n+1)
        n += big_float(2); // next denominator: 2n+3
        iter++;
        if (iter > max_iter) {
            throw std::runtime_error("ln: series did not converge within max iterations");
        }
    }

    // ln(x) = 2 * series + k * ln(2)
    big_float final_result = result * big_float(2) + big_float(k) * ln2;
    final_result.simplify();
    return final_result;
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