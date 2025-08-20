/******************************************************************************
 leapfrog.cpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#include "type.hpp"

void leap_frog_kick(double3 v[restrict], const double3 a[restrict], const int n, const double dt) {
#pragma acc data present(x[0:n], v[0:n])
    {   
        /* update v with a */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            v[i].x += a[i].x * dt;
            v[i].y += a[i].y * dt;
            v[i].z += a[i].z * dt;
        }
    }
}

void leap_frog_drift(double4 x[restrict], const double3 v[restrict], const int n, const double dt) {
#pragma acc data present(x[0:n], v[0:n])
    {   
        /* update x with v */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            x[i].x += v[i].x * dt;
            x[i].y += v[i].y * dt;
            x[i].z += v[i].z * dt;
        }
    }
}
