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

#include "libhybrid.h"

hyb_errorcode hyb_main_loop(hyb_opts *opts, hyb_float *y, hyb_float *xp, hyb_float tau, const hyb_float *x, const hyb_float *u, const hyb_float **p) {

  opts->Y(y, x[0], x[1], x + 2, u, p);

  if (x[0] >= opts->T_horizon)
    return HYB_TLIMIT;
  if (x[1] >= opts->J_horizon)
    return HYB_JLIMIT;

  hyb_bool d = opts->D(x[0], x[1], x + 2, u, p);
  #if HYB_JUMP_LOGIC == 2
    hyb_bool c = opts->C(x[0], x[1], x + 2, u, p);
  #endif

  hyb_bool it_jumps;
  #if HYB_JUMP_LOGIC == 1
    it_jumps = d;
  #elif HYB_JUMP_LOGIC == 2
    it_jumps = d && !c;
  #endif

  if (it_jumps) {
    xp[0] = x[0];
    xp[1] = x[1] + 1.0;
    opts->J(xp + 2, x[0], x[1], x + 2, u, p);
  } else {
    rk4_opts rk4_o = { opts->Ts, (opts->x_size + 2), hyb_flow_map_wrapper };
    rk4_errorcode rk4_ret = rk4(&rk4_o, xp, tau, x, u, p, HYB_SEND_OPTS(opts));
    if (rk4_ret) {
      return (hyb_errorcode)rk4_ret;
    }
  }
  return HYB_SUCCESS;
}

void hyb_flow_map_wrapper(hyb_float *dx, hyb_float tau, const hyb_float *x, const hyb_float *u, const hyb_float **p, void *vopts) {
  dx[0] = 1.0;
  dx[1] = 0.0;
  HYB_GET_OPTS(vopts)->F(dx + 2, x[0], x[1], x + 2, u, p);
}
