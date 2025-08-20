/******************************************************************************
 initialize.cpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#include <cmath>
#include <random>

#include "analyze.hpp"

void make_spherical_df(const int n, double m[], double x[][3], double v[][3], const double r_v, const double eps2) {
    std::random_device rnd; /* get seed from device random generator */
    std::mt19937 mt(rnd()); /* set seed for Mersenne Twister */

    std::uniform_real_distribution<> uni_dist(-1.0, 1.0);
    /* Set mass and position */
    for (int i = 0; i < n; i++) {
        /* Total mass (M) = 1 */
        m[i] = 1.0 / n;

        /* Position: generate [-1, 1] double with MT, and then filter*/
        while (true) {
            double tmp_x = uni_dist(mt);
            double tmp_y = uni_dist(mt);
            double tmp_z = uni_dist(mt);

            if (tmp_x * tmp_x + tmp_y * tmp_y + tmp_z * tmp_z < 1.0) {
                x[i][0] = tmp_x;
                x[i][1] = tmp_y;
                x[i][2] = tmp_z;
                break;
            }
        }
    }

    const double v_sigma = sqrt(-2.0 * r_v * calc_potential_energy(n, m, x, eps2) / 3.0);
    std::normal_distribution<> norm_dist(0.0, v_sigma);
    /* Set velocity from maxwell distribution */
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 3; k++) {
            v[i][k] = norm_dist(mt);
        }
    }
}
