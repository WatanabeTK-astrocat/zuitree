/**
 * @file leapfrog.cpp
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

#include "type.hpp"

/**
 * @brief Updates the velocity of particles using the kick phase of the leapfrog integration.
 *
 * @param v Array of particle velocities, size n. Return value.
 * @param a Array of particle accelerations, size n.
 * @param n Number of particles.
 * @param dt Time step.
 */
void leap_frog_kick(double3 v[restrict], const double3 a[restrict], const int n, const double dt) {
#pragma acc data present(x[0:n], v[0:n])
    {   
        /* update v with a */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            v[i].x += a[i].x * dt;
            v[i].y += a[i].y * dt;
            v[i].z += a[i].z * dt;
        }
    }
}

/**
 * @brief Updates the position of particles using the drift phase of the leapfrog integration.
 *
 * @param x Array of particle positions, size n. Return value.
 * @param v Array of particle velocities, size n.
 * @param n Number of particles.
 * @param dt Time step.
 */
void leap_frog_drift(double4 x[restrict], const double3 v[restrict], const int n, const double dt) {
#pragma acc data present(x[0:n], v[0:n])
    {   
        /* update x with v */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            x[i].x += v[i].x * dt;
            x[i].y += v[i].y * dt;
            x[i].z += v[i].z * dt;
        }
    }
}
