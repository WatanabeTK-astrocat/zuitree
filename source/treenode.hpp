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
    /**
     * @brief 3D position of the node (left, bottom, down)
     * 
     */
    double3 x;

    /**
     * @brief 4D position of the center of mass (mass, cx, cy, cz)
     * 
     */
    double4 cx;

    /**
     * @brief length of cell
     * 
     */
    double l;

    /**
     * @brief threshold for tree force calculation: l over theta squared
     * 
     */
    double lt2;

    /**
     * @brief -100: no particle within -1: multiple particles within 0~: the only particle within
     * 
     */
    int particle;

    /**
     * @brief pointer to the next sibling node
     * 
     */
    struct _NODE *n;

    /**
     * @brief pointer to the more (first child) node
     * 
     */
    struct _NODE *m;
} NODE;
