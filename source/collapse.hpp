/******************************************************************************
 collapse.hpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#pragma once

#include "type.hpp"
#include "treenode.hpp"

/* calculate gravitational force from j-body to i-body */
void grav_force(double3 a, const double3 dx, const double r2, const double mj);

/* calculate gravitational forces for all particles */
void calc_forces_direct(double3 a[restrict], const int n, const double4 x[restrict], const double eps2);
void calc_forces_tree(double3 a[restrict], const int n, const double4 x[restrict], const double eps2, const double theta);
