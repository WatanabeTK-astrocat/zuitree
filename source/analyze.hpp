/******************************************************************************
 analyze.hpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#pragma once

#include "type.hpp"

double calc_kinetic_energy(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2);
double calc_potential_energy(const int n, const double4 x[restrict], const double eps2);
double calc_total_energy(const double K, const double W);
double calc_total_energy(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2);
double calc_virial_ratio(const double K, const double W);
double calc_virial_ratio(const int n, const double4 x[restrict], const double3 v[restrict], const double eps2);
