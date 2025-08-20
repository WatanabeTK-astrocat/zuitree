/******************************************************************************
 initialize.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#pragma once

#include "type.hpp"

/* create spherical initial state from n, r_v, and eps2 */
void make_spherical_df(const int n, double4 x[restrict], double3 v[restrict], const double r_v, const double eps2);
