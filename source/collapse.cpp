/******************************************************************************
 collapse.cpp: practice 1
 Taichi K. Watanabe
******************************************************************************/

// #include <omp.h>

#include <cmath>

#include "treenode.hpp"

void calc_force(const int n, const double m[restrict], const double x[restrict][3], double a[restrict][3], const double eps2, const double theta) {
#pragma acc data present(m[0:n], x[0:n][0:3], a[0:n][0:3])
    {
        /* calc force */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            a[i][0] = 0.0;
            a[i][1] = 0.0;
            a[i][2] = 0.0;

            for (int j = 0; j < n; j++) {
                double r0 = x[j][0] - x[i][0];
                double r1 = x[j][1] - x[i][1];
                double r2 = x[j][2] - x[i][2];

                double d2 = r0 * r0 + r1 * r1 + r2 * r2 + eps2;
                double rinv = 1.0 / std::sqrt(d2);
                double r3inv = rinv * rinv * rinv;

                a[i][0] += m[j] * r0 * r3inv;
                a[i][1] += m[j] * r1 * r3inv;
                a[i][2] += m[j] * r2 * r3inv;
            }
        }
    }
}

void set_node_param(NODE *node, const double x, const double y, const double z, const double l, const double theta) {
    node->x[0] = x;
    node->x[1] = y;
    node->x[2] = z;
    node->l = l;
    node->lt2 = (l * l) / (theta * theta);
    for (int k = 0; k < 3; k++) {
        node->cx[k] = 0.0;
    }
    node->particle = -100;
    node->cm = 0.0;
}

void create_root_node(NODE *root_node, const int n, const double x[restrict][3], const double theta) {
#pragma acc data present(x[0:n][0:3])
    {
        double minx[3], maxx[3];
        for (int k = 0; k < 3; k++) {
            minx[k] = x[0][k];
            maxx[k] = x[0][k];
        }

        for (int i = 1; i < n; i++) {
            for (int k = 0; k < 3; k++) {
                minx[k] = fmin(minx[k], x[i][k]);
                maxx[k] = fmax(maxx[k], x[i][k]);
            }
        }

        set_node_param(root_node, minx[0], minx[1], minx[2], fmax(fmax(maxx[0] - minx[0], maxx[1] - minx[1]), maxx[2] - minx[2]), theta);
    }
}

void add_particle_to_node(NODE *node, const int particle, const int n, const double x[restrict][3], const double m[restrict], const double theta) {
#pragma acc data present(m[0:n], x[0:n][0:3])
    {
        /* Calculate new centroid and total mass */
        for (int k = 0; k < 3; k++) {
            node->cx[k] += m[particle] * x[particle][k];
        }
        node->cm += m[particle];
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
            set_node_param(node->m, node->x[0], node->x[1], node->x[2], newl, theta);
            set_node_param(node->m->n, node->x[0], node->x[1], node->x[2] + newl, newl, theta);
            set_node_param(node->m->n->n, node->x[0], node->x[1] + newl, node->x[2], newl, theta);
            set_node_param(node->m->n->n->n, node->x[0], node->x[1] + newl, node->x[2] + newl, newl, theta);
            set_node_param(node->m->n->n->n->n, node->x[0] + newl, node->x[1], node->x[2], newl, theta);
            set_node_param(node->m->n->n->n->n->n, node->x[0] + newl, node->x[1], node->x[2] + newl, newl, theta);
            set_node_param(node->m->n->n->n->n->n->n, node->x[0] + newl, node->x[1] + newl, node->x[2], newl, theta);
            set_node_param(node->m->n->n->n->n->n->n->n, node->x[0] + newl, node->x[1] + newl, node->x[2] + newl, newl, theta);

            // add the particle that was originally in the cell into children
            if (x[node->particle][0] < node->x[0] + newl) {
                if (x[node->particle][1] < node->x[1] + newl) {
                    if (x[node->particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m, node->particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n, node->particle, n, x, m, theta);
                    }
                } else {
                    if (x[node->particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m->n->n, node->particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n, node->particle, n, x, m, theta);
                    }
                }
            } else {
                if (x[node->particle][1] < node->x[1] + newl) {
                    if (x[node->particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m->n->n->n->n, node->particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n, node->particle, n, x, m, theta);
                    }
                } else {
                    if (x[node->particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m->n->n->n->n->n->n, node->particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n->n->n, node->particle, n, x, m, theta);
                    }
                }
            }

            node->particle = -1;
        }

        if (node->particle == -1) {
            // multiple particles within
            if (x[particle][0] < node->x[0] + newl) {
                if (x[particle][1] < node->x[1] + newl) {
                    if (x[particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m, particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n, particle, n, x, m, theta);
                    }
                } else {
                    if (x[particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m->n->n, particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n, particle, n, x, m, theta);
                    }
                }
            } else {
                if (x[particle][1] < node->x[1] + newl) {
                    if (x[particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m->n->n->n->n, particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n, particle, n, x, m, theta);
                    }
                } else {
                    if (x[particle][2] < node->x[2] + newl) {
                        add_particle_to_node(node->m->n->n->n->n->n->n, particle, n, x, m, theta);
                    } else {
                        add_particle_to_node(node->m->n->n->n->n->n->n->n, particle, n, x, m, theta);
                    }
                }
            }
        }
    }
}

void normalize_nodes_recursive(NODE *node) {
    if (node->particle > -100) {
        for (int k = 0; k < 3; k++) {
            node->cx[k] /= node->cm;
        }

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

void calc_force_iterative(NODE *node, const int i_particle, const int n, const double m[restrict], const double x[restrict][3], double a[restrict][3], const double eps2) {
#pragma acc data present(m[0:n], x[0:n][0:3], a[0:n][0:3])
    {
        while (node != nullptr) {
            if (node->particle > -100) {
                double r0 = node->cx[0] - x[i_particle][0];
                double r1 = node->cx[1] - x[i_particle][1];
                double r2 = node->cx[2] - x[i_particle][2];

                double d2 = r0 * r0 + r1 * r1 + r2 * r2 + eps2;

                if (node->particle > -1 || node->lt2 < d2) {
                    /* the cell is far from the i_particle */
                    double rinv = 1.0 / std::sqrt(d2);
                    double r3inv = rinv * rinv * rinv;
                    a[i_particle][0] += node->cm * r0 * r3inv;
                    a[i_particle][1] += node->cm * r1 * r3inv;
                    a[i_particle][2] += node->cm * r2 * r3inv;

                    node = node->n;
                } else {
                    /* the cell is near the particle */
                    node = node->m;
                }
            } else {
                node = node->n;
            }
        }
    }
}

/*
各力の計算 NlogN:
木構造の作成
    各ノードには、そのノードが相当する領域のxyz一辺の長さのdouble、そこに含まれる質点のid(なければ-1)がメンバとなる
    各質点に関して、(N)
        親ノードに質点を追加しようとする
            できなければそのノードを分割、そのうち二つに元あった質点と今入れようとした質点を入れる(リンクの追加と新しい重心・総質量の計算) (log N)
    各質点に関して、(N)
        全ての領域の葉に関してDFS/BFS的に、(下限はシータクラテリオン)セル(もしくは質点)との相互作用を計算(logN?)
*/
void calc_force_tree(const int n, const double m[restrict], const double x[restrict][3], double a[restrict][3], const double eps2, const double theta) {
#pragma acc data present(m[0:n], x[0:n][0:3], a[0:n][0:3])
    {
        /* init a array */
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < 3; k++) {
                a[i][k] = 0.0;
            }
        }

        /* build tree */
        NODE root_node{};
        create_root_node(&root_node, n, x, theta);

        for (int i = 0; i < n; i++) {
            add_particle_to_node(&root_node, i, n, x, m, theta);
        }

        normalize_nodes_recursive(&root_node);

        /* calculate force from tree */
#pragma acc kernels
#pragma acc loop independent gang
        for (int i = 0; i < n; i++) {
            calc_force_iterative(&root_node, i, n, m, x, a, eps2);
        }
    }
}
