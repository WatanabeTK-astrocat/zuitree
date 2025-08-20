/******************************************************************************
 collapse.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

#include "treenode.hpp"

/* calculate interaction (gravity) force */
void calc_force(const int n, const double m[restrict], const double x[restrict][3], double a[restrict][3], const double eps2, const double theta);

void set_node_param(NODE *node, const double x, const double y, const double z, const double l, const double theta);

void create_root_node(NODE *root_node, const int n, const double x[restrict][3], const double theta);

void add_particle_to_node(NODE *node, const int particle, const int n, const double x[restrict][3], const double m[restrict], const double theta);

void calc_force_iterative(NODE *node, const int i_particle, const int n, const double m[restrict], const double x[restrict][3], double a[restrict][3], const double eps2);

void calc_force_tree(const int n, const double m[restrict], const double x[restrict][3], double a[restrict][3], const double eps2, const double theta);
