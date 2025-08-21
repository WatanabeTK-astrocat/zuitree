/**
 * @file analyze.cpp
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

#include "type.hpp"

/**
 * @brief Calculates the kinetic energy of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param v particle velocities, size n.
 * @param eps2 softening parameter squared
 * @return double kinetic energy
 */
double calc_kinetic_energy(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2) {
    double K = 0.0;
    for (int i = 0; i < n; i++) {
        K += x[i].w * (v[i].x * v[i].x + v[i].y * v[i].y + v[i].z * v[i].z);
    }
    K *= 0.5;

    return K;
}

/**
 * @brief Calculates the potential energy of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param eps2 softening parameter squared
 * @return double potential energy
 * 
 * needs parallelization using GPU
 */
double calc_potential_energy(const int n, const double4 x[restrict], const double eps2) {
    double W = 0.0;
    for (int i = 0; i < n - 1; i++) {
        double3 dx;

        for (int j = i + 1; j < n; j++) {
            double r2 = 0.0;
            dx.x = x[j].x - x[i].x;
            dx.y = x[j].y - x[i].y;
            dx.z = x[j].z - x[i].z;

            double d2 = eps2 + dx.x * dx.x + dx.y * dx.y + dx.z * dx.z;

            W -= x[i].w * x[j].w / sqrt(d2);
        }
    }

    return W;
}

/**
 * @brief Calculates the total energy of a system of particles
 * 
 * @param K kinetic energy
 * @param W potential energy
 * @return double total energy
 */
double calc_total_energy(const double K, const double W) {
    return K + W;
}

/**
 * @brief Calculates the total energy of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param v particle velocities, size n.
 * @param eps2 softening parameter squared
 * @return double total energy
 * 
 * just a wrapper for calc_total_energy(K, W)
 */
double calc_total_energy(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2) {
    return calc_total_energy(calc_kinetic_energy(n, x, v, eps2), calc_potential_energy(n, x, eps2));
}

/**
 * @brief Calculates the virial ratio of a system of particles
 * 
 * @param K kinetic energy
 * @param W potential energy
 * @return double virial ratio
 */
double calc_virial_ratio(const double K, const double W) {
    return -K / W;
}

/**
 * @brief Calculates the virial ratio of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param v particle velocities, size n.
 * @param eps2 softening parameter squared
 * @return double virial ratio
 * 
 * just a wrapper for calc_virial_ratio(K, W)
 */
double calc_virial_ratio(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2) {
    return calc_virial_ratio(calc_kinetic_energy(n, x, v, eps2), calc_potential_energy(n, x, eps2));
}
