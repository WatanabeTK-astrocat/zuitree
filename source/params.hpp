/**
 * @file params.hpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief Defines Simulation Parameters
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#pragma once

#include <iostream>

/**
 * @brief Structure to hold simulation parameters.
 * 
 */
struct SimulationParams {
    int animation_bool; // bool for realtime visualizing 0=False, 1=True
    int tree_bool;      // bool for using tree method. 0=Direct, 1=Tree
    int n;              // Number of particles
    double r_v;         // Virial ratio = Kinetic Energy (K) / Absolute Potential Energy (|W|)
    double eps;         // Softening Param
    double eps2;        // Softening Param Squared
    double theta;       // Theta param
    double dt;          // Time step
    double T_end;       // Duration of simulation
    double T_out;       // Time step of data output

    void input_from_cin() {
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
    }
};
