/******************************************************************************
 leapfrog.hpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#pragma once

#include "type.hpp"

void leap_frog_kick(double3 v[restrict], const double3 a[restrict], const int n, const double dt);
void leap_frog_drift(double4 x[restrict], const double3 v[restrict], const int n, const double dt);
