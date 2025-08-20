/******************************************************************************
 leapfrog.cpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#include "collapse.hpp"

void leap_frog_start(const int n, const double m[restrict], double x[restrict][3], double v[restrict][3], double a[restrict][3], const double dt, const double eps2, const double theta, const int tree_bool) {
#pragma acc data present(m[0:n], x[0:n][0:3], v[0:n][0:3], a[0:n][0:3])
    {
        /* update a */
        if (tree_bool == 0) {
            calc_force(n, m, x, a, eps2, theta);
        } else {
            calc_force_tree(n, m, x, a, eps2, theta);
        }
        
        
        /* update v by half step */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < 3; k++) {
                v[i][k] += a[i][k] * dt * 0.5;
            }
        }
    }
}

void leap_frog_onestep(const int n, const double m[restrict], double x[restrict][3], double v[restrict][3], double a[restrict][3], const double dt, const double eps2, const double theta, const int tree_bool) {
#pragma acc data present(m[0:n], x[0:n][0:3], v[0:n][0:3], a[0:n][0:3])
    {
        /* update x */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < 3; k++) {
                x[i][k] += v[i][k] * dt;
            }
        }

        /* update a */
        if (tree_bool == 0) {
            calc_force(n, m, x, a, eps2, theta);
        } else {
            calc_force_tree(n, m, x, a, eps2, theta);
        }
        
        /* update v */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < 3; k++) {
                v[i][k] += a[i][k] * dt;
            }
        }
    }
}

void leap_frog_finish(const int n, const double m[restrict], double x[restrict][3], double v[restrict][3], double a[restrict][3], const double dt, const double eps2, const double theta, const int tree_bool) {
#pragma acc data present(m[0:n], x[0:n][0:3], v[0:n][0:3], a[0:n][0:3])
    {
        /* update x by half step */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < 3; k++) {
                x[i][k] += v[i][k] * dt * 0.5;
            }
        }
    }
}
