#include "../inc/bigfloat.hpp"

using namespace std;
// Расчёт Пи по формуле Рамануджана
big_float big_float::PI(const big_float& eps) {
    if(eps <= big_float(0)){
        throw std::invalid_argument("Epsilon should be more than 0, dolbayob");
    }
    big_int one(1);
    big_float sum(0);


    big_float const_1103(1103); // the constant from the formula
    big_float const_26390(26390); // the constant from the formula
    big_float const_396_4 = big_float(396).pow(4); // (4 * 99)^4
    big_float const_2 = big_float(2); // the constant from the formula
    big_float const_9801(9801); // the constant from the formula

    big_float sqrt_2 = big_float(big_int(2), big_int(1)).sqrt(eps) * const_2; // 2√2

    unsigned long k = 0;

    while (true) {
        big_int factorial_4k = big_float::factorial(4 * k); // (4k)!
        big_int factorial_k = big_float::factorial(k);     // k!
        big_float pow_396_4k = big_float(396).pow(4 * k); // 396^(4k)

        big_float numerator = big_float(factorial_4k) * (const_1103 + const_26390 * big_float(k));
        big_float denominator = big_float(factorial_k);
        denominator = denominator.pow(4); // (k!)^4
        denominator *= pow_396_4k; // (k!)^4 * (4 * 99)^4k

        big_float current = numerator / denominator;

        sum += current; // new member of series

        // check if the absolute value of 'current' is below the epsilon threshold
        // this means we've reached an acceptable solution/approximation
        if (current.abs() < eps) {
            break; // exit the loop
        }

        //safety check to prevent infinite loops
        ++k;
        if (k > 10000) {
            throw std::runtime_error("PI_Ramanujan: max iterations exceeded");
        }
    }

    //end of the formula
    big_float sum_ = sqrt_2 * sum;
    big_float pi = const_9801 / sum_;
    pi.simplify();

    return pi;
}

big_float sin(const big_float &base, const big_float &eps){
    // calculate π and 2π for angle normalization
    big_float pi = big_float::PI(eps);
    big_float two_pi = pi * big_float(2);
    big_float half_pi = pi / big_float(2);

    // normalize input angle to [-2π, 2π] range
    big_float x = base;
    if (x.abs() > two_pi) {
        x = x % (two_pi); // use modulo to bring angle within one period
    }

    // reduce angle to [0, π] range and remember sign change
    bool negate = false;
    if (x > pi) {
        // sin(x) = -sin(x - π) = sin(2π - x) for x in (π, 2π]
        x = two_pi - x;
        negate = true;
    }

    if (x > half_pi) {
        // sin(x) = sin(π - x) for x in (π/2, π]
        x = pi - x;
    }
    if (x < half_pi) {
        // sin(x) = -sin(π + x) for x in (-π, -π/2)
        x = big_float(-pi) - x;
    }

    big_float result = x; // base for sum
    big_float term = x; // the first term of series
    big_float x_squared = x.pow(2); // x^2
    unsigned long n = 1; // counter

    // sin(x) = Σ (-1)ⁿ x²ⁿ⁺¹/(2n+1)!
    while (term.abs() > eps) {
        term = term * (-x_squared) / big_float(2 * n * (2 * n + 1));
        result = result + term;
        n++;

        /* Explanation, why there is no factorial and pow
         * each subsequent term can be derived by their previous one, like
         * a(n) = a(n - 1) * (-x²) / (2n * (2n + 1)) what equally to a(n) = (-1)ⁿ * x²ⁿ⁺¹ / (2n + 1)!
         * */
        result.simplify();
        term.simplify();
    }

    return negate ? -result : result;
}

big_float cos(const big_float &x, const big_float &eps) {
    big_float pi = big_float::PI(eps); // π
    big_float two_pi = pi * big_float(2); // 2π
    big_float x_reduced = x; // number for decrease
    bool negate = false;

    if (x_reduced.abs() > two_pi) {
        x_reduced = x_reduced % (two_pi);
    }

    // negative input, like (cos(-x) = cos(x))
    if (x_reduced < big_float(0)) {
        x_reduced = -x_reduced;
    }

    // reducing to [0, 2PI]
    if (x_reduced > pi) {
        x_reduced = two_pi - x_reduced; // this is like cos(x) = cos(2PI - x)
    }
    if (x_reduced > pi / big_float(2)) {
        x_reduced = pi - x_reduced; // this is like cos(x) = -cos(PI - x)
        negate = !negate;
    }

    // Taylor series
    // cos(x) = Σ (-1)ⁿ x²ⁿ/(2n)!
    big_float term(1);
    big_float result(1);
    big_float x_squared = x_reduced.pow(2);
    unsigned long n = 1;
    int sign = -1;

    while (term.abs() > eps) {
        // each term calculate like this a(n) = a(n - 1) * x² / ((2n - 1) * 2n)
        big_float denom = big_float((2 * n - 1) * (2 * n));
        term = term * x_squared / denom;
        result += (sign > 0 ? term : -term);
        sign = -sign;
        ++n;
    }

    result.simplify();
    return negate ? -result : result;
}

big_float binomialCoefficient(unsigned long n, unsigned long k) {
    if (k > n) {
        throw std::invalid_argument("k cannot be greater than n in binomial coefficient");
    }

    big_float res(1);
    if (k > n - k) k = n - k;
    for (unsigned long i = 0; i < k; ++i) {
        res *= big_float(n - i);
        res /= big_float(i + 1);
    }
    res.simplify();
    return res;
}

big_float bernoulli(unsigned long n) {
    static big_float* cache = nullptr;
    static unsigned long cache_size = 0;

    if (cache == nullptr) {
        cache_size = 64;
        try {
            cache = new big_float[cache_size];
            for (unsigned long i = 0; i < cache_size; ++i) {
                cache[i] = big_float(0);
            }
            // Set base cases
            cache[0] = big_float(1);
            cache[1] = big_float("-1/2");
        } catch (const std::bad_alloc& e) {
            throw std::bad_alloc();
        }
    }

    if (n >= cache_size) {
        unsigned long new_size = n * 2;
        big_float* new_cache = nullptr;
        try {
            new_cache = new big_float[new_size];
            for (unsigned long i = 0; i < new_size; ++i) {
                new_cache[i] = (i < cache_size) ? cache[i] : big_float(0);
            }
            delete[] cache;
            cache = new_cache;
            cache_size = new_size;
        } catch (const std::bad_alloc& e) {
            delete[] new_cache;
            throw std::bad_alloc();
        }
    }

    if (cache[n] != big_float(0) || n == 0 || n == 1) {
        return cache[n];
    }

    if (n % 2 != 0) {
        cache[n] = big_float(0);
        return cache[n];
    }

    // Compute B_n using recursive sum
    big_float sum(0);
    for (unsigned long k = 0; k < n; ++k) {
        // Compute binomial coefficient C(n+1, k)
        big_float coef = binomialCoefficient(n + 1, k);
        sum += coef * bernoulli(k);
    }

    cache[n] = -sum / big_float(n + 1);
    cache[n].simplify();
    return cache[n];
}


// series only for |x| < pi\2
big_float tan(const big_float &base, const big_float &eps) {
    big_float pi = big_float::PI(eps);
    big_float two(2);
    big_float two_pi = pi * two;
    big_float half_pi = pi / two;
    big_float x = base;
    bool negate = false;

    // (tan(x) = tan(x mod π)) reduce to [-π, π]
    if (x.abs() > pi) {
        x = x % (pi);
    }

    if (x > half_pi) { // (π/2, π]: tan(x) = -tan(π - x)
        x = pi - x;
        negate = !negate;
        x.simplify();
    } else if (x <= -half_pi) { // [-π, -π/2): tan(x) = tan(π + x)
        x = pi + x;
        negate = !negate;
        x.simplify();
    }
    if (x < big_float(0)) { // (tan(-x) = -tan(x))
        x = -x;
        negate = !negate;
        x.simplify();
    }

    if (x.abs() > half_pi - eps) {
        throw std::invalid_argument("Tangent is not defined in π/2 + kπ");
    }


    // tan(x) = Σ [B₂ₙ * (-4)ⁿ * (1-4ⁿ)/(2n)!] * x²ⁿ⁻¹
    big_float result(0);
    big_float term;
    big_float x_squared = x.pow(2);
    big_float x_power = x;
    big_float pow_neg4_n = big_float(-4);
    big_float pow_4_n = big_float(4);
    unsigned long n = 1;

    while (true) {
        big_float Bn = bernoulli(2 * n);
        // B₂ₙ * (-4)ⁿ * (1 - 4ⁿ)
        big_float coef = Bn * pow_neg4_n * (big_float(1) - pow_4_n);
        // (2n)!
        big_float denom = factorial(2 * n);
        // (coef/(2n)!) * x^(2n-1)
        term = (coef / denom) * x_power;

        if (term.abs() < eps) {
            break;
        }

        result += term;
        ++n;
        x_power *= x_squared;
        pow_neg4_n *= big_float(-4);
        pow_4_n *= big_float(4);

        if (n > 300) {
            throw std::runtime_error("tan: max iterations exceeded");
        }
    }
    result.simplify();
    return negate ? -result : result;
}

big_float cot(const big_float &base, const big_float &eps){
    big_float pi = big_float::PI(eps);
    big_float x = base;

    if (x < big_float(0)) {
        x = -x;  // cot(-x) = -cot(x)
    }
    if (x > pi / big_float(2)) {
        x = pi - x; // reduce to [0, PI/2]
    }

    if (x.abs() < eps) {
        throw std::invalid_argument("Cotangent is not defined near k*PI");
    }
    // cot(π/2) = 0, but tan is not defined
    if ((x - pi / big_float(2)).abs() < eps) {
        return big_float(0);
    }

    big_float tan_val = tan(x, eps);

    if (tan_val.abs() < eps) {
        throw std::invalid_argument("Cotangent is not defined near PI/2 + k*PI");
    }
    return tan_val.reciprocal();
}
big_float sec(const big_float &base, const big_float &eps){
    big_float pi = big_float::PI(eps);
    big_float half_pi = pi / big_float(2);

    big_float x = base;
    big_float two_pi = pi * big_float(2);

    x = x % (two_pi);
    if (x < big_float(0)) x += two_pi; // convert to positive equivalent angle, like cos
    // x is close to π/2 or to -π/2
    big_float distance_to_pole = (x % half_pi).abs();
    if (distance_to_pole < eps ||
        (half_pi - distance_to_pole).abs() < eps) {
        throw std::invalid_argument("Secant is undefined at π/2 + kπ");
    }
    big_float cos_val = cos(x, eps);
    return cos_val.reciprocal();
}

big_float csc(const big_float &base, const big_float &eps){
    big_float sin_val = sin(base, eps);
    if (sin_val.abs() < eps) {
        throw std::invalid_argument("Cosecant is not defined in kπ");
    }
    return sin_val.reciprocal();
}
big_float arcsin(const big_float &base, const big_float &eps) {
    if (base.abs() > big_float(1)) {
        throw std::invalid_argument("arcsin is defined only for |x| <= 1");
    }

    big_float pi = big_float::PI(eps);
    big_float half_pi = pi / big_float(2);

    //base cases
    if (base == big_float(1)) return half_pi;
    if (base == big_float(-1)) return -half_pi;
    if (base == big_float(0)) return big_float(0);

    big_float result = base;
    big_float term = base;
    big_float x_squared = base.pow(2);
    unsigned long n = 1;

    // arcsin(x) = x + Σ [((2n-1)!! / (2n)!!) * x^(2n+1) / (2n+1)]
    while (true) {
        // a(n) = a(n-1) * (2n-1) * x^2 / (2 * n)
        term = term * x_squared * big_float(2 * n - 1) / big_float(2 * n);
        // a(n) / (2n+1)
        big_float next_term = term / big_float(2 * n + 1);
        if (next_term.abs() < eps) {
            break;
        }
        result += next_term;
        ++n;
    }
    result.simplify();
    return result;
}

big_float arccos(const big_float &base, const big_float &eps) {
    if (base.abs() > big_float(1)) {
        throw std::invalid_argument("arccos is defined only for |x| <= 1");
    }

    big_float pi = big_float::PI(eps);
    big_float half_pi = pi / big_float(2);

    // basic cases
    if (base == big_float(1)) return big_float(0);
    if (base == big_float(-1)) return pi;
    if (base == big_float(0)) return half_pi;

    big_float x = base;

    big_float arcsin_result = x;
    big_float term = x;
    big_float x_squared = x * x;
    big_int n = 1;
    // arcsin(x) = x + Σ [((2n-1)!! / (2n)!!) * x^(2n+1) / (2n+1)]
    while (true) {
        // x^(2n+1) * (2n-1)!! / (2n)!!
        term = term * x_squared * big_float(2 * n - 1) / big_float(2 * n);
        // a(n) / (2n+1)
        big_float next_term = term / big_float(2 * n + 1);

        if (next_term.abs() < eps) {
            break;
        }

        arcsin_result += next_term;
        ++n;

        if (n > 10000000) {
            throw std::runtime_error("arccos: max iterations exceeded (arcsin part)");
        }
    }

    // arccos = π/2 - arcsin(x)
    big_float result = half_pi - arcsin_result;
    result.simplify();

    return result;
}

big_float arctg(const big_float &x, const big_float &eps) {
    if (eps <= big_float(0)) {
        throw std::invalid_argument("Epsilon must be positive for arctg computation");
    }

    // basic cases
    if (x == big_float(0)) {
        return big_float(0);
    }
    if (x == big_float(1)) {
        return big_float::PI(eps) / big_float(4);
    }
    if (x == big_float(-1)) {
        return -big_float::PI(eps) / big_float(4);
    }

    big_float result = x;
    big_float term = x;
    big_float x_squared = x * x;
    big_int n = 1;
    big_float sign = big_float(-1);
    // for |x| > 1, use arctg(x) = ±π/2 - arctg(1/x)
    if (x.abs() > big_float(1)) {
        big_float half_pi = big_float::PI(eps) / big_float(2);
        big_float recip = x.reciprocal();
        result = (x > big_float(0)) ? half_pi - arctg(recip, eps)
                                    : -half_pi - arctg(recip, eps);
        // for 0.5 < |x| <= 1 use arctg(x) = π/4 + arctg((x-1)/(x+1))
    } else if (x.abs() > big_float("1/2")) {
        big_float arg = (x - big_float(1)) / (x + big_float(1));
        result = big_float::PI(eps) / big_float(4) + arctg(arg, eps);
        if (x < big_float(0)) {
            result = -result;
        }
    } else {
        //arctg(x) = x - x^3/3 + x^5/5 - ...
        while (term.abs() > eps) {
            term *= x_squared;
            big_float next_term = sign * term / big_float(2 * n + 1);

            if (next_term.abs() < eps) {
                break;
            }

            result += next_term;
            sign = -sign;
            ++n;

            if (n > 100000) {
                throw std::runtime_error("arctg: maximum iterations exceeded");
            }
        }
    }

    result.simplify();
    return result;
}

big_float arcctg(const big_float &base, const big_float &eps){
    if (eps <= big_float(0)) {
        throw std::invalid_argument("Epsilon must be positive for arcctg computation");
    }
    big_float pi = big_float::PI(eps);
    big_float half_pi = pi / big_float(pi);

    if (base == big_float(0)) {
        return half_pi;
    }

    big_float x = base;

    if (x == big_float(0)) {
        return pi / big_float(2);
    }

    big_float inv_x = x.reciprocal();
    big_float arctan_result = arctg(inv_x, eps);

    big_float result;
    // For x > 0: arccot(x) = arctan(1/x)
    // For x < 0: arccot(x) = π + arctan(1/x)
    result = (base > big_float(0)) ? arctan_result : pi + arctan_result;

    result.simplify();
    return result;
}

big_float arcsec(const big_float &base, const big_float &eps){
    if (eps <= big_float(0)) {
        throw std::invalid_argument("Epsilon must be positive for arcsec computation");
    }

    if (base.abs() < 1) {
        throw std::invalid_argument("arcsec is defined only for |x| >= 1");
    }

    if (base == big_float(1)) {
        return big_float(0);
    }

    if (base == big_float(-1)) {
        return big_float::PI(eps);
    }

    return arccos(base.reciprocal(), eps);
}
big_float arccosec(const big_float &base, const big_float &eps){
    if (eps <= big_float(0)) {
        throw std::invalid_argument("Epsilon must be positive for arccosec computation");
    }

    if (base.abs() < big_float(1)) {
        throw std::invalid_argument("Arccosecant is defined only for |x| >= 1");
    }

    big_float half_pi = big_float::PI(eps) / big_float(2);
    if (base == big_float(1)) {
        return half_pi;
    }
    if (base == big_float(-1)) {
        return -half_pi;
    }
    return arcsin(base.reciprocal(), eps);
}

