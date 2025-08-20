/******************************************************************************
 gnuplot.cpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

#include <iostream>

#include "type.hpp"

const double VMAX = 1.5; /* limit of variables in window */
const int TERMINAL_SIZE = 800;

void open_window(FILE **gp) {
    if ((*gp = popen("gnuplot -persist", "w")) == NULL) {
        printf("gnuplot open error!!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(*gp, "set size square\n");
    // fprintf(*gp, "set terminal qt size %d, %d\n", TERMINAL_SIZE, TERMINAL_SIZE);
    fprintf(*gp, "set terminal gif animate size %d, %d\n", TERMINAL_SIZE, TERMINAL_SIZE);
    fprintf(*gp, "set output \"plot.gif\"\n");
    fprintf(*gp, "set xrange [-%f:%f]\n", VMAX, VMAX);
    fprintf(*gp, "set yrange [-%f:%f]\n", VMAX, VMAX);
}

void close_window(FILE **gp) {
    pclose(*gp);
}

void animated_snapshot(FILE **gp, const int n, const double t, const double4 x[restrict]) {
    fprintf(*gp, "set key title \"%f\"\n", t);
    fprintf(*gp, "plot '-' with points pointtype 0 notitle \n");
    for (int i = 0; i <= n; i++) {
        fprintf(*gp, "%f\t%f\n", x[i].x, x[i].y);
    }
    fprintf(*gp, "e\n");
}
