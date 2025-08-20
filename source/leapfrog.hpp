/******************************************************************************
 leapfrog.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

void leap_frog_start(const int n, const double m[restrict], double x[restrict][3], double v[restrict][3], double a[restrict][3], const double dt, const double eps2, const double theta, const int tree_bool);

void leap_frog_onestep(const int n, const double m[restrict], double x[restrict][3], double v[restrict][3], double a[restrict][3], const double dt, const double eps2, const double theta, const int tree_bool);

void leap_frog_finish(const int n, const double m[restrict], double x[restrict][3], double v[restrict][3], double a[restrict][3], const double dt, const double eps2, const double theta, const int tree_bool);
