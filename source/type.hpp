/**
 * @file type.hpp
 * @author Taichi K. Watanabe (taichi.astrocat@gmail.com)
 * @brief Defines structs with 2, 3, or 4 floats or doubles to represent 2D/3D/4D vectors
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Taichi K. Watanabe 2025
 * 
 */

#pragma once

/**
 * @brief 2D vector of floats
 * 
 */
typedef struct {
    float x, y;
} float2;

/**
 * @brief 2D vector of doubles
 * 
 */
typedef struct {
    double x, y;
} double2;

/**
 * @brief 3D vector of floats
 * 
 */
typedef struct {
    float x, y, z;
} float3;

/**
 * @brief 3D vector of doubles
 * 
 */
typedef struct {
    double x, y, z;
} double3;

/**
 * @brief 4D vector of floats
 * 
 */
typedef struct {
    float x, y, z, w;
} float4;

/**
 * @brief 4D vector of doubles
 * 
 */
typedef struct {
    double x, y, z, w;
} double4;
