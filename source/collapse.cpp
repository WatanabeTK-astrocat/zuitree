/******************************************************************************
 collapse.cpp
 Taichi K. Watanabe
******************************************************************************/

#ifndef restrict
#define restrict
#endif

// #include <omp.h>

#include <cmath>
//#include <iostream>

#include "type.hpp"
#include "treenode.hpp"

/**
 * @brief Calculates the gravitational force exerted on i from j
 * note that a (return value) needs to be referenced
 *
 * @param a     Gravitational force vector, return value.
 * @param dx    Displacement vector x_j - x_i
 * @param r2    Squared distance |x_j - x_i|^2
 * @param mj    Mass of the j body m_j
 *
 */
void grav_force(double3 &a, const double3 dx, const double r2, const double mj) {
    double r1i = 1.0 / std::sqrt(r2); // Is optimization working?
    double r2i = r1i * r1i; // Optimization to multiply 2 variables at once, interchanging this
    double r1im = mj * r1i;
    double r3im = r1im * r2i;

    a.x += dx.x * r3im;
    a.y += dx.y * r3im;
    a.z += dx.z * r3im;
}

/**
 * @brief Calculates the gravitational forces for all particles using the direct method
 * 
 * @param a     Acceleration vectors, size n. Return value.
 * @param n     Number of particles.
 * @param x     Position vectors, size n.
 * @param eps2  Softening parameter squared.
 */
void calc_forces_direct(double3 a[restrict], const int n, const double4 x[restrict], const double eps2) {
#pragma acc data present(a[0:n], x[0:n])
    {
        /* calc force */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            a[i].x = 0.0;
            a[i].y = 0.0;
            a[i].z = 0.0;
            double3 dx;

            for (int j = 0; j < n; j++) {
                dx.x = x[j].x - x[i].x;
                dx.y = x[j].y - x[i].y;
                dx.z = x[j].z - x[i].z;

                double d2 = eps2 + dx.x * dx.x + dx.y * dx.y + dx.z * dx.z;

                grav_force(a[i], dx, d2, x[j].w);
            }
        }
    }

    //std::cout << a[0].x << ", " << a[0].y << ", " << a[0].z << std::endl;
}

void set_node_param(NODE *node, const double x, const double y, const double z, const double l, const double theta) {
    node->x.x = x;
    node->x.y = y;
    node->x.z = z;
    node->l = l;
    node->lt2 = (l * l) / (theta * theta);
    node->cx.x = 0.0;
    node->cx.y = 0.0;
    node->cx.z = 0.0;
    node->cx.w = 0.0;
    node->particle = -100;
}

void create_root_node(NODE *root_node, const int n, const double4 x[restrict], const double theta) {
#pragma acc data present(x[0:n])
    {
        double3 minx{}, maxx{};
        minx.x = x[0].x;
        maxx.x = x[0].x;
        minx.y = x[0].y;
        maxx.y = x[0].y;
        minx.z = x[0].z;
        maxx.z = x[0].z;

        for (int i = 1; i < n; i++) {
            minx.x = fmin(minx.x, x[i].x);
            maxx.x = fmax(maxx.x, x[i].x);
            minx.y = fmin(minx.y, x[i].y);
            maxx.y = fmax(maxx.y, x[i].y);
            minx.z = fmin(minx.z, x[i].z);
            maxx.z = fmax(maxx.z, x[i].z);
        }

        set_node_param(root_node, minx.x, minx.y, minx.z, fmax(fmax(maxx.x - minx.x, maxx.y - minx.y), maxx.z - minx.z), theta);
    }
}

void add_particle_to_node(NODE *node, const int particle, const int n, const double4 x[restrict], const double theta) {
#pragma acc data present(x[0:n])
    {
        /* Calculate new centroid and total mass */
        node->cx.x += x[particle].w * x[particle].x;
        node->cx.y += x[particle].w * x[particle].y;
        node->cx.z += x[particle].w * x[particle].z;
        node->cx.w += x[particle].w;
        double newl = node->l * 0.5;

        /* Add link to particle */
        if (node->particle < -1) {
            // no particle within
            node->particle = particle;
        } else if (node->particle >= 0) {
            // there was one particle within

            // create children
            node->m = new NODE;                      /* lll */
            node->m->n = new NODE;                   /* llr */
            node->m->n->n = new NODE;                /* lrl */
            node->m->n->n->n = new NODE;             /* lrr */
            node->m->n->n->n->n = new NODE;          /* rll */
            node->m->n->n->n->n->n = new NODE;       /* rlr */
            node->m->n->n->n->n->n->n = new NODE;    /* rrl */
            node->m->n->n->n->n->n->n->n = new NODE; /* rrr */
            node->m->n->n->n->n->n->n->n->n = node->n;
            set_node_param(node->m, node->x.x, node->x.y, node->x.z, newl, theta);
            set_node_param(node->m->n, node->x.x, node->x.y, node->x.z + newl, newl, theta);
            set_node_param(node->m->n->n, node->x.x, node->x.y + newl, node->x.z, newl, theta);
            set_node_param(node->m->n->n->n, node->x.x, node->x.y + newl, node->x.z + newl, newl, theta);
            set_node_param(node->m->n->n->n->n, node->x.x + newl, node->x.y, node->x.z, newl, theta);
            set_node_param(node->m->n->n->n->n->n, node->x.x + newl, node->x.y, node->x.z + newl, newl, theta);
            set_node_param(node->m->n->n->n->n->n->n, node->x.x + newl, node->x.y + newl, node->x.z, newl, theta);
            set_node_param(node->m->n->n->n->n->n->n->n, node->x.x + newl, node->x.y + newl, node->x.z + newl, newl, theta);

            // add the particle that was originally in the cell into children
            if (x[node->particle].x < node->x.x + newl) {
                if (x[node->particle].y < node->x.y + newl) {
                    if (x[node->particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m, node->particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n, node->particle, n, x, theta);
                    }
                } else {
                    if (x[node->particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m->n->n, node->particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n, node->particle, n, x, theta);
                    }
                }
            } else {
                if (x[node->particle].y < node->x.y + newl) {
                    if (x[node->particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m->n->n->n->n, node->particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n, node->particle, n, x, theta);
                    }
                } else {
                    if (x[node->particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m->n->n->n->n->n->n, node->particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n->n->n, node->particle, n, x, theta);
                    }
                }
            }

            node->particle = -1;
        }

        if (node->particle == -1) {
            // multiple particles within
            if (x[particle].x < node->x.x + newl) {
                if (x[particle].y < node->x.y + newl) {
                    if (x[particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m, particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n, particle, n, x, theta);
                    }
                } else {
                    if (x[particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m->n->n, particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n, particle, n, x, theta);
                    }
                }
            } else {
                if (x[particle].y < node->x.y + newl) {
                    if (x[particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m->n->n->n->n, particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n, particle, n, x, theta);
                    }
                } else {
                    if (x[particle].z < node->x.z + newl) {
                        add_particle_to_node(node->m->n->n->n->n->n->n, particle, n, x, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n->n->n, particle, n, x, theta);
                    }
                }
            }
        }
    }
}

void normalize_nodes_recursive(NODE *node) {
    if (node->particle > -100) {
        node->cx.x /= node->cx.w;
        node->cx.y /= node->cx.w;
        node->cx.z /= node->cx.w;

        if (node->particle == -1) {
            normalize_nodes_recursive(node->m);
            normalize_nodes_recursive(node->m->n);
            normalize_nodes_recursive(node->m->n->n);
            normalize_nodes_recursive(node->m->n->n->n);
            normalize_nodes_recursive(node->m->n->n->n->n);
            normalize_nodes_recursive(node->m->n->n->n->n->n);
            normalize_nodes_recursive(node->m->n->n->n->n->n->n);
            normalize_nodes_recursive(node->m->n->n->n->n->n->n->n);
        }
    }
}

void calc_force_iterative(NODE *node, double3 a[restrict], const int i_particle, const int n, const double4 x[restrict], const double eps2) {
#pragma acc data present(a[0:n], x[0:n])
    {
        while (node != nullptr) {
            if (node->particle > -100) {
                double3 dx;
                dx.x = node->cx.x - x[i_particle].x;
                dx.y = node->cx.y - x[i_particle].y;
                dx.z = node->cx.z - x[i_particle].z;

                double d2 = eps2 + dx.x * dx.x + dx.y * dx.y + dx.z * dx.z;

                if (node->particle > -1 || node->lt2 < d2) {
                    /* the cell is sufficiently far from the i_particle, so calculate the force */
                    grav_force(a[i_particle], dx, d2, node->cx.w);

                    node = node->n;
                } else {
                    /* the cell is too big and close to the particle */
                    node = node->m;
                }
            } else {
                node = node->n;
            }
        }
    }
}

/**
 * @brief Calculates the gravitational forces for all particles using the tree method
 * 
 * @param a     Acceleration vectors, size n. Return value.
 * @param n     Number of particles.
 * @param x     Position vectors, size n.
 * @param eps2  Softening parameter squared.
 * 
 * 各力の計算 NlogN:
 * 木構造の作成
 * 各ノードには、そのノードが相当する領域のxyz一辺の長さのdouble、そこに含まれる質点のid(なければ-1)がメンバとなる
 * 各質点に関して、(N)
 *     親ノードに質点を追加しようとする
 *     できなければそのノードを分割、そのうち二つに元あった質点と今入れようとした質点を入れる(リンクの追加と新しい重心・総質量の計算) (log N)
 * 各質点に関して、(N)
 *     全ての領域の葉に関してDFS/BFS的に、(下限はシータクラテリオン)セル(もしくは質点)との相互作用を計算 (logN)
 * 
 */
void calc_forces_tree(double3 a[restrict], const int n, const double4 x[restrict], const double eps2, const double theta) {
#pragma acc data present(a[0:n], x[0:n])
    {
        /* init a array */
        for (int i = 0; i < n; i++) {
            a[i].x = 0.0;
            a[i].y = 0.0;
            a[i].z = 0.0;
        }

        /* build tree */
        NODE root_node{};
        create_root_node(&root_node, n, x, theta);

        for (int i = 0; i < n; i++) {
            add_particle_to_node(&root_node, i, n, x, theta);
        }

        normalize_nodes_recursive(&root_node);

        /* calculate force from tree */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            calc_force_iterative(&root_node, a, i, n, x, eps2);
        }
    }
}
