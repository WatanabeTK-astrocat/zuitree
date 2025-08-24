/**
 * @file main.cpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#ifndef restrict
#define restrict
#endif

#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "params.hpp"
#include "type.hpp"
#include "analyze.hpp"
#include "leapfrog.hpp"
#include "collapse.hpp"
#include "gnuplot.hpp"
#include "initialize.hpp"

/**
 * @brief Exports a simple snapshot of the current simulation state.
 *
 * @param outputfile_realtime The output file stream for real-time data.
 * @param t The current time.
 * @param n The number of particles.
 * @param x The positions of the particles, size n.
 * @param v The velocities of the particles, size n.
 * @param eps2 The softening parameter squared.
 */
void export_simple_snapshot(std::ofstream &outputfile_realtime, const double t, const int n, const double4 x[restrict], const double3 v[restrict], const double eps2) {
    double K = calc_kinetic_energy(n, x, v, eps2);
    double W = calc_potential_energy(n, x, eps2);
    outputfile_realtime << t << ", " << K << ", " << W << ", " << calc_total_energy(K, W) << ", " << calc_virial_ratio(K, W) << std::endl;
}

void simulate(SimulationParams& params) {
    // Simulation code goes here
    std::cout << "Initializing..." << std::endl;

    /* Variables needed for simulation */
    double4 x[params.get_n()];
    double3 v[params.get_n()], a[params.get_n()];                    /* Particle Data : mass, position, velocity, acceleration */
    /* init arrays of struct */
    for (int i = 0; i < params.get_n(); i++) {
        x[i] = {0.0, 0.0, 0.0, 0.0};
        v[i] = {0.0, 0.0, 0.0};
        a[i] = {0.0, 0.0, 0.0};
    }
    
    double t = 0.0;                                            /* current time */
    double next_t_out = params.get_T_out();                                 /* next time for exporting */

    make_spherical_df(params.get_n(), x, v, params.get_r_v(), params.get_eps2());

    const double e_start = calc_total_energy(params.get_n(), x, v, params.get_eps2()); /* Start system energy */

    FILE *gp; /* animation window and realtime analyze file */
    if (params.get_animation_bool()) {
        open_window(&gp);
    }
    std::ofstream outputfile_realtime("./output/output-realtime.csv");
    outputfile_realtime << "t, K, W, E, v_r" << std::endl;

    const std::chrono::system_clock::time_point time_start = std::chrono::system_clock::now();  // start time measurement

    /* simulation start */
    std::cout << "Simulation started..." << std::endl;

#pragma acc enter data create(x[0:params.get_n()], v[0:params.get_n()], a[0:params.get_n()])
#pragma acc update device(x[0:params.get_n()], v[0:params.get_n()], a[0:params.get_n()])

    /* kick start */
    if (params.get_tree_bool()) {
        calc_forces_tree(a, params.get_n(), x, params.get_eps2(), params.get_theta());
    } else {
        calc_forces_direct(a, params.get_n(), x, params.get_eps2());
    }
    leap_frog_kick(v, a, params.get_n(), 0.5 * params.get_dt());

    export_simple_snapshot(outputfile_realtime, t, params.get_n(), x, v, params.get_eps2());

    while (t < params.get_T_end()) {
        /* advance time by one timestep */
        leap_frog_drift(x, v, params.get_n(), params.get_dt());

        if (params.get_tree_bool()) {
            calc_forces_tree(a, params.get_n(), x, params.get_eps2(), params.get_theta());
        } else {
            calc_forces_direct(a, params.get_n(), x, params.get_eps2());
        }
        leap_frog_kick(v, a, params.get_n(), params.get_dt());

        if (t >= next_t_out) {
            /* visualize */
#pragma acc update host(x[0:params.get_n()], v[0:params.get_n()])

            export_simple_snapshot(outputfile_realtime, t, params.get_n(), x, v, params.get_eps2());

            next_t_out = t + params.get_T_out(); // it is like this because we want to output data every T_out time
            if (params.get_animation_bool()) {
                animated_snapshot(&gp, params.get_n(), t, x);
                // usleep(10000);
            }
        }

        t += params.get_dt();
    }

    leap_frog_drift(x, v, params.get_n(), 0.5 * params.get_dt());

#pragma acc exit data delete (x[0:params.get_n()], v[0:params.get_n()], a[0:params.get_n()])

    /* simulation finish */
    std::cout << "Simulation finished!" << std::endl;
    const std::chrono::system_clock::time_point time_end = std::chrono::system_clock::now();  // end time measurement

    if (params.get_animation_bool()) {
        close_window(&gp);
    }
    outputfile_realtime.close();

    const double e_end = calc_total_energy(params.get_n(), x, v, params.get_eps2());                                                       /* End system energy */
    const double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count() / 1000.0; /* elapsed time*/

    /*<< "T_start = " << std::chrono::duration_cast<std::chrono::milliseconds>(t_start).count() / 1000.0 << std::endl
    << "T_end   = " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end).count() / 1000.0 << std::endl*/
    std::cout << std::endl
              << "Elapsed time (s) = " << elapsed << std::endl
              << "E_start = " << e_start << std::endl
              << "E_end   = " << e_end << std::endl
              << std::scientific
              << "E_error = " << abs((e_end - e_start) / e_start) << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "ERROR: Incorrect usage of the software. Usage: ./collapse parameterfile" << std::endl;
        return -1;
    }
    std::string paramfile = argv[1];
    std::ifstream fs(paramfile);
    if (!fs) {
        std::cerr << "ERROR: Cannot open the simulation settings json file." << std::endl;
        return -1;
    }

    json j = json::parse(fs); // read JSON
    SimulationParams params(j); // parse JSON to params class

    simulate(params); // run simulation

    return 0;
}
