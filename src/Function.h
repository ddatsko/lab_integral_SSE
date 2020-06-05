#ifndef LAB_INTEGRALS_FUNCTION_H
#define LAB_INTEGRALS_FUNCTION_H

#include <cmath>
#include <emmintrin.h>
#include <x86intrin.h>

class Function {
private:
    double a[2] = {-20, -20}, b[2] = {-0.14142135623730953, -0.14142135623730953}, c[2] = {2 * M_PI,
                                                                                                 2 * M_PI}, half[2] = {
            0.5, 0.5},
            last[2] = {22.718281828459045, 22.718281828459045};
    double result[2];
    __m128d xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;  // Not to use too much registers

public:


    // The function is following:
    // a * exp(b * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(c * x) + cos(c * y))) - a + exp(1);
    // Calculate the sum of values in 2 points: (xs[0], ys[0]), (xs[1], ys[1])
    virtual inline double sumOfValuesAt(double xs[2], double ys[2]) {
        xmm0 = _mm_load_pd(xs);
        xmm1 = _mm_load_pd(ys);
        xmm2 = _mm_mul_pd(xmm0, xmm0); // x * x
        xmm3 = _mm_mul_pd(xmm1, xmm1); // y * y
        xmm2 = _mm_add_pd(xmm2, xmm3); // x * x + y * y
        xmm2 = _mm_sqrt_pd(xmm2);      // sqrt(x * x + y * y)
        xmm3 = _mm_load_pd(b);         // b
        xmm3 = _mm_mul_pd(xmm3, xmm2); // b * sqrt(0.5) * sqrt(x * x + y * y)
        exp_pd(xmm3);               // exp(b * sqrt(0.5) * sqrt(x * x + y * y))
        xmm2 = _mm_load_pd(a);         // a
        xmm3 = _mm_mul_pd(xmm3, xmm2); // a * exp(b * sqrt(0.5) * sqrt(x * x + y * y))
        xmm4 = _mm_load_pd(c);         // c
        xmm0 = _mm_add_pd(xmm0, xmm4); // c * x
        xmm1 = _mm_add_pd(xmm1, xmm4); // c * y
        cos_pd(xmm0);               // cos(c * x)
        cos_pd(xmm1);               // cos(c * y)
        xmm0 = _mm_add_pd(xmm0, xmm1); // cos(c * x) + cos(c * y)
        xmm5 = _mm_load_pd(half);      // 0.5
        xmm0 = _mm_mul_pd(xmm0, xmm5); // 0.5 * (cos(c * x) + cos(c * y))
        exp_pd(xmm0);               // exp(0.5 * (cos(c * x) + cos(c * y)))
        xmm6 = _mm_load_pd(last);      // -a + exp(1)
        xmm3 = _mm_sub_pd(xmm3,
                          xmm0); // a * exp(b * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(c * x) + cos(c * y)))
        xmm3 = _mm_add_pd(xmm3,
                          xmm6); // a * exp(b * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(c * x) + cos(c * y))) - a + exp(1)
        _mm_store_pd(result, xmm3);
        return result[0] + result[1];
    }


    static inline void exp_pd(__m128d &value) {
#ifdef WITH_ICPC_INTRINSICS
        value = _mm_exp_pd(value);
#else
        // Я не придумав, як це зробити оптимальніше.
        double tmp_for_exp[2];
        _mm_store_pd(tmp_for_exp, value);
        tmp_for_exp[0] = exp(tmp_for_exp[0]);
        tmp_for_exp[1] = exp(tmp_for_exp[1]);
        value = _mm_load_pd(tmp_for_exp);
#endif
    }

    static inline void cos_pd(__m128d &value) {
#ifdef WITH_ICPC_INTRINSICS
        value = _mm_cos_pd(value);
#else
        // Я не придумав, як це зробити оптимальніше.
        double tmp_for_cos[2];
        _mm_store_pd(tmp_for_cos, value);
        tmp_for_cos[0] = cos(tmp_for_cos[0]);
        tmp_for_cos[1] = cos(tmp_for_cos[1]);
        value = _mm_load_pd(tmp_for_cos);
#endif
    }

};

#endif //LAB_INTEGRALS_FUNCTION_H