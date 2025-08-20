/******************************************************************************
 gnuplot.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

#include <iostream>

void open_window(FILE **gp);
void close_window(FILE **gp);
void animated_snapshot(FILE **gp, const int n, const double t, const double x[][3]);
