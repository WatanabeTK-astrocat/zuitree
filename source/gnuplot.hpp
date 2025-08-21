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

void open_window(FILE **gp);
void close_window(FILE **gp);
void animated_snapshot(FILE **gp, const int n, const double t, const double4 x[restrict]);
