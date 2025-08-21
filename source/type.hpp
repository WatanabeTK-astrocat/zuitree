/**
 * @file type.hpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#pragma once

// structs with 2, 3, or 4 floats or doubles to represent 2D/3D/4D vectors

typedef struct {
    float x, y;
} float2;

typedef struct {
    double x, y;
} double2;

typedef struct {
    float x, y, z;
} float3;

typedef struct {
    double x, y, z;
} double3;

typedef struct {
    float x, y, z, w;
} float4;

typedef struct {
    double x, y, z, w;
} double4;
