/**
 * @file collapse.hpp
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
#include "treenode.hpp"

/**
 * @brief Calculates the gravitational force exerted on i from j
 * note that a (return value) needs to be referenced
 * 
 * @param a     Gravitational force vector. Return value
 * @param dx    Displacement vector x_j - x_i
 * @param r2    Squared distance |x_j - x_i|^2
 * @param mj    Mass of the j body m_j
 * 
 */
void grav_force(double3 &a, const double3 dx, const double r2, const double mj);

/**
 * @brief Calculates the gravitational forces for all particles using the direct method
 *
 * @param a     Acceleration vectors, size n. Return value.
 * @param n     Number of particles.
 * @param x     Position vectors, size n.
 * @param eps2  Softening parameter squared.
 */
void calc_forces_direct(double3 a[restrict], const int n, const double4 x[restrict], const double eps2);

/**
 * @brief Calculates the gravitational forces for all particles using the tree method
 *
 * @param a     Acceleration vectors, size n. Return value.
 * @param n     Number of particles.
 * @param x     Position vectors, size n.
 * @param eps2  Softening parameter squared.
 * @param theta Opening angle for the tree method.
 */
void calc_forces_tree(double3 a[restrict], const int n, const double4 x[restrict], const double eps2, const double theta);
