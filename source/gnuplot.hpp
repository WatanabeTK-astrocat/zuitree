/**
 * @file gnuplot.hpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#pragma once

#include <iostream>

#include "type.hpp"

/**
 * @brief Opens a new gnuplot window
 * 
 * @param gp gnuplot window
 */
void open_window(FILE **gp);

/**
 * @brief Closes a gnuplot window
 * 
 * @param gp gnuplot window
 */
void close_window(FILE **gp);

/**
 * @brief Creates an animated snapshot in a gnuplot window
 * 
 * @param gp gnuplot window
 * @param n number of particles
 * @param t current time
 * @param x particle positions
 * 
 */
void animated_snapshot(FILE **gp, const int n, const double t, const double4 x[restrict]);
