/**
 * @file initialize.cpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#ifndef restrict
#define restrict
#endif

#include <cmath>
#include <random>

#include "type.hpp"
#include "analyze.hpp"

/**
 * @brief Initializes particle positions and velocities for a spherical distribution.
 *
 * @param n Number of particles.
 * @param x Array of particle positions, size n. Return value.
 * @param v Array of particle velocities, size n. Return value.
 * @param r_v Virial ratio.
 * @param eps2 Softening parameter squared.
 */
void make_spherical_df(const int n, double4 x[restrict], double3 v[restrict], const double r_v, const double eps2) {
    std::random_device rnd; /* get seed from device random generator */
    std::mt19937 mt(rnd()); /* set seed for Mersenne Twister */

    std::uniform_real_distribution<> uni_dist(-1.0, 1.0);
    /* Set mass and position */
    for (int i = 0; i < n; i++) {
        /* Total mass (M) = 1 */
        x[i].w = 1.0 / n;

        /* Position: generate uniform distribution [-1, 1] with MT, and then filter */
        while (true) {
            double tmp_x = uni_dist(mt);
            double tmp_y = uni_dist(mt);
            double tmp_z = uni_dist(mt);

            if (tmp_x * tmp_x + tmp_y * tmp_y + tmp_z * tmp_z < 1.0) {
                x[i].x = tmp_x;
                x[i].y = tmp_y;
                x[i].z = tmp_z;
                break;
            }
        }
    }

    const double v_sigma = sqrt(-2.0 * r_v * calc_potential_energy(n, x, eps2) / 3.0);
    std::normal_distribution<> norm_dist(0.0, v_sigma);
    /* Set velocity from maxwell distribution */
    for (int i = 0; i < n; i++) {
        v[i].x = norm_dist(mt);
        v[i].y = norm_dist(mt);
        v[i].z = norm_dist(mt);
    }
}
