/******************************************************************************
 main.cpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "type.hpp"
#include "analyze.hpp"
#include "leapfrog.hpp"
#include "collapse.hpp"
#include "gnuplot.hpp"
#include "initialize.hpp"

void export_simple_snapshot(std::ofstream &outputfile_realtime, const double t, const int n, const double4 x[restrict], const double3 v[restrict], const double eps2) {
    double K = calc_kinetic_energy(n, x, v, eps2);
    double W = calc_potential_energy(n, x, eps2);
    outputfile_realtime << t << ", " << K << ", " << W << ", " << calc_total_energy(K, W) << ", " << calc_virial_ratio(K, W) << std::endl;
}

int main() {
    /* Parameter Settings */
    int animation_bool; /* bool for realtime visualizing 0=False, 1=True */
    int tree_bool;      /* bool for using tree method. 0=Direct, 1=Tree */
    int n;              /* Number of particles */
    double r_v;         /* Virial ratio = Kinetic Energy (K) / Absolute Potential Energy (|W|) */
    double eps;         /* Softening Param */
    double eps2;        /* Softening Param Squared */
    double theta;       /* Theta param */
    double dt;          /* Time step */
    double T_end;       /* Duration of simulation */
    double T_out;       /* Time step of data output */

    std::cout << "Bool for visualizing (1 for true, 0 for false) = " << std::endl;
    std::cin >> animation_bool;
    std::cout << "Bool for using tree method (1 for tree method, 0 for direct method) = " << std::endl;
    std::cin >> tree_bool;
    std::cout << "Number of particles = " << std::endl;
    std::cin >> n;
    std::cout << "Virial ratio = " << std::endl;
    std::cin >> r_v;
    std::cout << "Softening Param = " << std::endl;
    std::cin >> eps;
    eps2 = eps * eps;
    std::cout << "Theta Param = " << std::endl;
    std::cin >> theta;
    std::cout << "Time step = " << std::endl;
    std::cin >> dt;
    std::cout << "Duration of simulation = " << std::endl;
    std::cin >> T_end;
    std::cout << "Time step of data output = " << std::endl;
    std::cin >> T_out;

    std::cout << "Initializing..." << std::endl;

    /* Variables needed for simulation */
    double4 x[n];
    double3 v[n], a[n];                    /* Particle Data : mass, position, velocity, acceleration */
    /* init arrays of struct */
    for (int i = 0; i < n; i++) {
        x[i] = {0.0, 0.0, 0.0, 0.0};
        v[i] = {0.0, 0.0, 0.0};
        a[i] = {0.0, 0.0, 0.0};
    }
    
    double t = 0.0;                                            /* current time */
    double next_t_out = T_out;                                 /* next time for exporting */

    make_spherical_df(n, x, v, r_v, eps2);

    const double e_start = calc_total_energy(n, x, v, eps2); /* Start system energy */

    FILE *gp; /* animation window and realtime analyze file */
    if (animation_bool) {
        open_window(&gp);
    }
    std::ofstream outputfile_realtime("./output/output-realtime.csv");
    outputfile_realtime << "t, K, W, E, v_r" << std::endl;

    const std::chrono::system_clock::time_point time_start = std::chrono::system_clock::now();  // start time measurement

    /* simulation start */
    std::cout << "Simulation started..." << std::endl;

#pragma acc enter data create(x[0:n], v[0:n], a[0:n])
#pragma acc update device(x[0:n], v[0:n], a[0:n])

    /* kick start */
    if (tree_bool) {
        calc_forces_tree(a, n, x, eps2, theta);
    } else {
        calc_forces_direct(a, n, x, eps2);
    }
    leap_frog_kick(v, a, n, 0.5 * dt);

    export_simple_snapshot(outputfile_realtime, t, n, x, v, eps2);

    while (t < T_end) {
        /* advance time by one timestep */
        leap_frog_drift(x, v, n, dt);

        if (tree_bool) {
            calc_forces_tree(a, n, x, eps2, theta);
        } else {
            calc_forces_direct(a, n, x, eps2);
        }
        leap_frog_kick(v, a, n, dt);

        if (t >= next_t_out) {
            /* visualize */
#pragma acc update host(x[0:n], v[0:n])

            export_simple_snapshot(outputfile_realtime, t, n, x, v, eps2);

            next_t_out = t + T_out;
            if (animation_bool) {
                animated_snapshot(&gp, n, t, x);
                // usleep(10000);
            }
        }

        t += dt;
    }

    leap_frog_drift(x, v, n, 0.5 * dt);

#pragma acc exit data delete (x[0:n], v[0:n], a[0:n])

    /* simulation finish */
    std::cout << "Simulation finished!" << std::endl;
    const std::chrono::system_clock::time_point time_end = std::chrono::system_clock::now();  // end time measurement

    if (animation_bool) {
        close_window(&gp);
    }
    outputfile_realtime.close();

    const double e_end = calc_total_energy(n, x, v, eps2);                                                       /* End system energy */
    const double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000.0; /* elapsed time*/

    /*<< "T_start = " << std::chrono::duration_cast<std::chrono::milliseconds>(t_start).count() / 1000.0 << std::endl
    << "T_end   = " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end).count() / 1000.0 << std::endl*/
    std::cout << std::endl
              << "Elapsed time (s) = " << elapsed << std::endl
              << "E_start = " << e_start << std::endl
              << "E_end   = " << e_end << std::endl
              << std::scientific
              << "E_error = " << abs((e_end - e_start) / e_start) << std::endl;

    return 0;
}
