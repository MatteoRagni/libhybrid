/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2018 - Matteo Ragni, Matteo Cocetti - University of Trento
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include "libhybrid.h"
/**
 * Flow map for a bouncing ball:
 *
 *  \f[
 *    \dot{x}_1(t) = x_2(t), \quad
 *    \dot{x}_2(t) = -g
 *  \f]
 * where g is the gravity (p[0][0])
 */
void flow_map(double *dx, double t, double j, const double *x, const double *u, const double **p) {
  dx[0] = x[1];
  dx[1] = -p[0][0];
}

/**
 * Jump Map for a bouncing ball:
 *
 *  \f[
 *    x^+_1(t, j) = x_1(t, j), \quad
 *    x^+_2(t, j) = -k x_2(t, j)
 *  \f]
 * where k is damping for elastic impact (p[0][1])
 */
void jump_map(double *xp, double t, double j, const double *x, const double *u, const double **p) {
  xp[0] = 0;
  xp[1] = -p[1][0] * x[1];
}

/**
 * Jumping condition: the ball reaches the zero
 */
hyb_bool jump_set(double t, double j, const double *x, const double *u, const double **p) {
  return x[0] <= -1e-3 ? hyb_true : hyb_false;
}

void out_map(double *y, double t, double j, const double *x, const double *u, const double **p) {
  y[0] = t;
  y[1] = j;
  y[2] = x[0];
  y[3] = x[1];
}

hyb_opts opts = {
  4, /**< Output size */
  2, /**< State size */
  1e-3, /**< Time step */
  10.0, /**< T horizon */
  1000.0, /**< J horizon */
  flow_map,
  jump_map,
  out_map,
  jump_set,
  NULL
};


int main() {

  double z[4] = { 0.0, 0.0, 2.0, 0.2 }; /**< States MUST contain two additional placeholder for time and jumps */
  double zp[4] = { 0.0, 0.0, 2.0, 0.2 };
  double y[4] = { 0.0, 0.0, 2.0, 0.2 };
  double u[1] = { 5.0 };
  double **p = (double **)malloc(2 * sizeof(double*));
  p[0] = (double *)calloc(1, sizeof(double));
  p[1] = (double *)calloc(1, sizeof(double));
  p[0][0] = 9.81;
  p[1][0] = 0.75;

  double step = 0;
  while (y[1] < 50) {
    //      step  y0      u0      y1      y2
    printf("% 3.5f,% 3.5f,% 3.5f,% 3.5f\n", y[0], y[1], y[2], y[3]);
    hyb_main_loop(&opts, y, zp, step, z, u, (const double**) p);

    for(size_t l = 0; l < 4; l++)
      z[l] = zp[l]; // Updating the states for next loop
  }

  return 0;
}
