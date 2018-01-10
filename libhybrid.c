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

/**
 * @brief Hybrid system main loop
 *
 * The main loop of the hybrid system performs the following operations:
 * 1. Check if stop criteria is reached
 * 2. Check if the jump conditions are respected.
 *     * if jump is requsted, it enters a loop of update until no jump should be performed
 *     * else the execution continues
 * 3. The
 * @param opts pointer to an option structure
 * @param xp next evaluated
 * @param tau time as it arrives from the MATLAB engine
 * @param
 */


hyb_errno hyb_main_loop(hyb_opts *opts, hyb_float *y, hyb_float *xp, hyb_float tau, hyb_float *x, hyb_float *u, hyb_float **p) {
  double xs = x + 2;

  hyb_bool must_jump = hyb_true;
  while (must_jump) {
    hyb_bool d = opts->D(x[0], x[1], x + 2, u, p);
    hyb_bool c = opts->C(x[0], x[1], x + 2, u, p);

    must_jump = jump_logic(d, c);
    if (must_jump) {

    }
  }

  if (j >= opts->J_horizon)
    return HYB_JLIMIT;
  if (t >= opts->T_horizon)
    return HYB_TLIMIT;

}
