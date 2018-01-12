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

void flow_map(double *dx, double t, double k, const double *x, const double *u, const double **p) {
  dx[0] = x[1];
  dx[1] = u[0] - p[0][0] * x[0] - p[1][0] * x[1];
}

void jump_map(double *xp, double t, double k, const double *x, const double *u, const double **p) {
  xp[0] = 0.0;
  xp[1] = 0.0;
}

hyb_bool jump_set(double t, double k, const double *x, const double *u, const double **p) {
  return x[0] >= 1.0 ? hyb_true : hyb_false;
}

void out_map(double *y, double t, double k, const double *x, const double *u, const double **p) {
  y[0] = t;
  y[1] = x[0];
  y[2] = x[1];
}

hyb_opts opts = {
  3, /**< Output size */
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

  double t_k_x[4] = { 0.0, 0.0, 0.0, 0.0 };
  double tp_kp_xp[4] = { 0.0, 0.0, 0.0, 0.0 };
  double y[3] = { 0.0, 0.0, 0.0 };
  double u[1] = { 5.0 };
  double **p = (double **)malloc(2 * sizeof(double*));
  p[0] = (double *)calloc(1, sizeof(double));
  p[1] = (double *)calloc(1, sizeof(double));
  p[0][0] = 1.0;
  p[0][1] = 0.5;

  double step = 0;
  while (step * 1e-3 <= 8.0) {
    //      step  y0      u0      y1      y2
    printf("% 5.0f,% 3.5f,% 3.5f,% 3.5f,% 3.5f\n", step, y[0], u[0], y[1], y[2]);
    step += 1;
    hyb_main_loop(&opts, y, tp_kp_xp, step, t_k_x, u, (const double**) p);

    t_k_x[0] = tp_kp_xp[0];
    t_k_x[1] = tp_kp_xp[1];
    t_k_x[2] = tp_kp_xp[2];
    t_k_x[3] = tp_kp_xp[3];
  }

  return 0;
}
