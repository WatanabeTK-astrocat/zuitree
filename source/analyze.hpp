/******************************************************************************
 analyze.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

double calc_kinetic_energy(const int n, const double m[], const double v[][3], const double eps2);
double calc_potential_energy(const int n, const double m[], const double x[][3], const double eps2);
double calc_total_energy(const int n, const double m[], const double x[][3], const double v[][3], const double eps2);
double calc_virial_ratio(const int n, const double m[], const double x[][3], const double v[][3], const double eps2);
