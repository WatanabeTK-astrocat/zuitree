/******************************************************************************
 leapfrog.hpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#pragma once

#include "type.hpp"

/**
 * @brief Updates the velocity of particles using the kick phase of the leapfrog integration.
 *
 * @param v Array of particle velocities, size n. Return value.
 * @param a Array of particle accelerations, size n.
 * @param n Number of particles.
 * @param dt Time step.
 */
void leap_frog_kick(double3 v[restrict], const double3 a[restrict], const int n, const double dt);

/**
 * @brief Updates the position of particles using the drift phase of the leapfrog integration.
 *
 * @param x Array of particle positions, size n. Return value.
 * @param v Array of particle velocities, size n.
 * @param n Number of particles.
 * @param dt Time step.
 */
void leap_frog_drift(double4 x[restrict], const double3 v[restrict], const int n, const double dt);
