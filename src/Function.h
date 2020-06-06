#ifndef LAB_INTEGRALS_FUNCTION_H
#define LAB_INTEGRALS_FUNCTION_H

#include <cmath>
#include <emmintrin.h>
#include <iostream>
#include "utils.h"

class Function {
private:
    double *a, *b, *c, *half, *last, *result;
    __m128d mmx0, mmx1, mmx2, mmx3;

public:
    Function() {
        // All the values should be aligned by 16 before loading to mmx registers
        a = new_aligned_two_doubles();
        a[0] = -20;
        a[1] = -20;
        b = new_aligned_two_doubles();
        b[0] = -0.14142135623730953;
        b[1] = -0.14142135623730953;
        c = new_aligned_two_doubles();
        c[0] = 2 * M_PI;
        c[1] = 2 * M_PI;
        half = new_aligned_two_doubles();
        half[0] = 0.5;
        half[1] = 0.5;
        last = new_aligned_two_doubles();
        last[0] = 22.718281828459045;
        last[1] = 22.718281828459045;
        result = new_aligned_two_doubles();
    }

    ~Function() {
        delete a;
        delete b;
        delete c;
        delete half;
        delete last;
        delete result;
    }


    // The function is following:
    // a * exp(b * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(c * x) + cos(c * y))) - a + exp(1);
    // Calculate the sum of values in 2 points: (xs[0], ys[0]), (xs[1], ys[1])
    virtual inline double sumOfValuesAt(double *xs, double *ys) {
        mmx0 = _mm_load_pd(xs);             // x
        mmx1 = _mm_load_pd(ys);             // y
        mmx2 = _mm_mul_pd(mmx0, mmx0);      // x * x
        mmx3 = _mm_mul_pd(mmx1, mmx1);      // y * y
        mmx2 = _mm_add_pd(mmx2, mmx3);      // x * x + y * y
        mmx2 = _mm_sqrt_pd(mmx2);           // sqrt(x * x + y * y)
        mmx3 = _mm_load_pd(b);              // b * sqrt(0.5)
        mmx3 = _mm_mul_pd(mmx3, mmx2);      // b * sqrt(0.5) * sqrt(x * x + y * y)
        exp_pd(mmx3);                       // exp(b * sqrt(0.5) * sqrt(x * x + y * y))
        mmx2 = _mm_load_pd(a);              // a
        mmx3 = _mm_mul_pd(mmx3, mmx2);      // a * exp(b * sqrt(0.5) * sqrt(x * x + y * y))
        mmx2 = _mm_load_pd(c);              // c
        mmx0 = _mm_mul_pd(mmx0, mmx2);      // c * x
        mmx1 = _mm_mul_pd(mmx1, mmx2);      // c * y
        cos_pd(mmx0);                       // cos(c * x)
        cos_pd(mmx1);                       // cos(c * y)
        mmx0 = _mm_add_pd(mmx0, mmx1);      // cos(c * x) + cos(c * y)
        mmx1 = _mm_load_pd(half);           // 0.5
        mmx0 = _mm_mul_pd(mmx0, mmx1);      // 0.5 * (cos(c * x) + cos(c * y))
        exp_pd(mmx0);                       // exp(0.5 * (cos(c * x) + cos(c * y)))
        mmx1 = _mm_load_pd(last);           // -a + exp(1)
        mmx3 = _mm_sub_pd(mmx3,
                          mmx0);            // a * exp(b * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(c * x) + cos(c * y)))
        mmx3 = _mm_add_pd(mmx3,
                          mmx1);            // a * exp(b * sqrt(0.5 * (x * x + y * y))) - exp(0.5 * (cos(c * x) + cos(c * y))) - a + exp(1)
        _mm_store_pd(result, mmx3);
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