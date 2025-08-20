/******************************************************************************
 gnuplot.hpp
 Taichi K. Watanabe
******************************************************************************/

#pragma once

#include <iostream>

#include "type.hpp"

void open_window(FILE **gp);
void close_window(FILE **gp);
void animated_snapshot(FILE **gp, const int n, const double t, const double4 x[restrict]);
