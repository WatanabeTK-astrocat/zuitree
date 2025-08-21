/**
 * @file analyze.hpp
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

#pragma once

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
double calc_kinetic_energy(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2);

/**
 * @brief Calculates the potential energy of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param eps2 softening parameter squared
 * @return double potential energy
 */
double calc_potential_energy(const int n, const double4 x[restrict], const double eps2);

/**
 * @brief Calculates the total energy of a system of particles
 * 
 * @param K kinetic energy
 * @param W potential energy
 * @return double total energy
 */
double calc_total_energy(const double K, const double W);

/**
 * @brief Calculates the total energy of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param v particle velocities, size n.
 * @param eps2 softening parameter squared
 * @return double total energy
 */
double calc_total_energy(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2);

/**
 * @brief Calculates the virial ratio of a system of particles
 * 
 * @param K kinetic energy
 * @param W potential energy
 * @return double virial ratio
 */
double calc_virial_ratio(const double K, const double W);

/**
 * @brief Calculates the virial ratio of a system of particles
 * 
 * @param n number of particles
 * @param x particle positions, size n.
 * @param v particle velocities, size n.
 * @param eps2 softening parameter squared
 * @return double virial ratio
 */
double calc_virial_ratio(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2);
