/******************************************************************************
 analyze.cpp: practice 1
 Taichi Kuriyama
******************************************************************************/

#include <cmath>

double calc_kinetic_energy(const int n, const double m[], const double v[][3], const double eps2) {
    double K = 0.0;
    for (int i = 0; i < n; i++) {
        K += m[i] * (v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]);
    }
    K *= 0.5;

    return K;
}

double calc_potential_energy(const int n, const double m[], const double x[][3], const double eps2) {
    double W = 0.0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double r2 = 0.0;
            for (int k = 0; k < 3; k++) {
                r2 += (x[j][k] - x[i][k]) * (x[j][k] - x[i][k]);
            }

            W -= m[i] * m[j] / sqrt(r2 + eps2);
        }
    }
    return W;
}

double calc_total_energy(const int n, const double m[], const double x[][3], const double v[][3], const double eps2) {
    return calc_kinetic_energy(n, m, v, eps2) + calc_potential_energy(n, m, x, eps2);
}

double calc_virial_ratio(const int n, const double m[], const double x[][3], const double v[][3], const double eps2) {
    return -calc_kinetic_energy(n, m, v, eps2) / calc_potential_energy(n, m, x, eps2);
}
