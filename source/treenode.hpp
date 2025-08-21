/**
 * @file treenode.hpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#pragma once

#include "type.hpp"

/**
 * @brief tree structure node for the tree method
 * 
 */
typedef struct _NODE {
    double3 x;          // left bottom down position
    double4 cx;         // center of mass position
    double l;           // length of cell
    double lt2;         // threshold for tree force calculation: l over theta squared
    int particle;       // -100: no particle within -1: multiple particles within 0~: the only particle within
    struct _NODE *n;    // next sibling node
    struct _NODE *m;    // more (first child) node
} NODE;
