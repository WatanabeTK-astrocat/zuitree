/******************************************************************************
 treenode.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

typedef struct _NODE {
    double x[3];   // left bottom down poisition
    double cx[3];  // center of mass position
    double l;      // length of cell
    double lt2;    // threshold for tree force calculation: l over theta squared
    int particle;  // -100: no particle within -1: multiple particles within 0~: the only particle within
    double cm;     // total mass
    struct _NODE *n;
    struct _NODE *m;
} NODE;
