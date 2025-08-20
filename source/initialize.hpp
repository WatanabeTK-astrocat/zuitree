/******************************************************************************
 initialize.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

/* create spherical initial state from n, r_v, and eps2 */
void make_spherical_df(const int n, double m[], double x[][3], double v[][3], const double r_v, const double eps2);
