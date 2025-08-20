/******************************************************************************
 initialize.hpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#pragma once

#include "type.hpp"

/**
 * @brief Initializes particle positions and velocities for a spherical distribution.
 *
 * @param n Number of particles.
 * @param x Array of particle positions, size n. Return value.
 * @param v Array of particle velocities, size n. Return value.
 * @param r_v Virial ratio.
 * @param eps2 Softening parameter squared.
 */
void make_spherical_df(const int n, double4 x[restrict], double3 v[restrict], const double r_v, const double eps2);
