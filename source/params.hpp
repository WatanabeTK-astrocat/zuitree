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
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Structure to hold simulation parameters.
 * 
 */
class SimulationParams {
private:
    bool animation_bool; // bool for realtime visualizing 0=False, 1=True
    bool tree_bool;      // bool for using tree method. 0=Direct, 1=Tree
    int n;              // Number of particles
    double r_v;         // Virial ratio = Kinetic Energy (K) / Absolute Potential Energy (|W|)
    double eps;         // Softening Param
    double eps2;        // Softening Param Squared
    double theta;       // Theta param
    double dt;          // Time step
    double T_end;       // Duration of simulation
    double T_out;       // Time step of data output

public:
    SimulationParams(bool animation_bool_, bool tree_bool_, int n_, double r_v_, double eps_, double theta_, double dt_, double T_end_, double T_out_)
        : animation_bool(animation_bool_), tree_bool(tree_bool_), n(n_), r_v(r_v_), eps(eps_), eps2(eps_ * eps_), theta(theta_), dt(dt_), T_end(T_end_), T_out(T_out_) {}
    
    SimulationParams(json& j)
        : animation_bool(j.contains("animation_bool") ? j["animation_bool"].template get<bool>() : false),
          tree_bool(j.contains("tree_bool") ? j["tree_bool"].template get<bool>() : true),
          n(j.contains("n") ? j["n"].template get<int>() : 1024),
          r_v(j.contains("r_v") ? j["r_v"].template get<double>() : 0.2),
          eps(j.contains("eps") ? j["eps"].template get<double>() : 1.5625e-2),
          eps2(eps * eps),
          theta(j.contains("theta") ? j["theta"].template get<double>() : 0.2),
          dt(j.contains("dt") ? j["dt"].template get<double>() : 0.125),
          T_end(j.contains("T_end") ? j["T_end"].template get<double>() : 10.0),
          T_out(j.contains("T_out") ? j["T_out"].template get<double>() : 0.125) {}

    // Getters
    bool get_animation_bool() const { return animation_bool; }
    bool get_tree_bool() const { return tree_bool; }
    int get_n() const { return n; }
    double get_r_v() const { return r_v; }
    double get_eps2() const { return eps2; }
    double get_theta() const { return theta; }
    double get_dt() const { return dt; }
    double get_T_end() const { return T_end; }
    double get_T_out() const { return T_out; }

};
