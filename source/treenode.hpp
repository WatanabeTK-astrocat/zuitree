/******************************************************************************
 treenode.hpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

#pragma once

#include "type.hpp"

typedef struct _NODE {
    double3 x;   // left bottom down poisition
    double4 cx;  // center of mass position
    double l;      // length of cell
    double lt2;    // threshold for tree force calculation: l over theta squared
    int particle;  // -100: no particle within -1: multiple particles within 0~: the only particle within
    struct _NODE *n;
    struct _NODE *m;
} NODE;
