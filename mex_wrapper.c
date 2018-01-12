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

#ifdef MATLAB_WRAPPER

#include "libhybrid.h"
#include "mex.h"
#include "model.c" /**< Please provide your model in this C file. Also include the struct with the name "options" */


#if defined(_MSC_VER)
#define inline __inline
#endif
inline void error_message(hyb_errorcode c) {
  switch(ret) {
  case HYB_SUCCESS:
    return;
    break;
  case HYB_EMALLOC:
    mexErrMsgIdAndTxt("LIBHYBRID:Model:MallocError",
     "Allocation error in the discretization");
    break;
  case HYB_NULLPTR:
    mexErrMsgIdAndTxt("LIBHYBRID:Model:NullPointer",
     "Null pointer passed to the integrator");
    break;
  case HYB_TLIMIT:
    mexErrMsgIdAndTxt("LIBHYBRID:Model:tLimit",
     "Reached time horizon for hybrid model");
    break;
  case HYB_JLIMIT:
    mexErrMsgIdAndTxt("LIBHYBRID:Model:jLimit",
     "Reached jump horizon for hybrid model");
    break;
  case HYB_NOJUMP:
    mexErrMsgIdAndTxt("LIBHYBRID:Model:InvalidSet",
     "Invalid set conditions provided");
    break;
  default:
    mexErrMsgIdAndTxt("LIBHYBRID:Model:GenericError",
     "An unknown error was raised");
    break;
  }
}



void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]) {
  /* Declaration of input and output arguments. */
  double *x, *u, **p, *xp, *y, *sim_time;
  int     i, np;
  size_t  nu, nx;

  if (nrhs < 3) {
      mexErrMsgIdAndTxt("LIBHYBRID:Model:InvalidSyntax",
      "At least 3 inputs expected (t, u, x).");
  }

  np = nrhs - 3;

  /* Determine number of inputs and states. */
  nx = mxGetNumberOfElements(prhs[1]); /* Number of states. */
  nu = mxGetNumberOfElements(prhs[2]); /* Number of inputs. */

  if (nx != options.x_size) {
    mexErrMsgIdAndTxt("LIBHYBRID:Model:InvalidSyntax",
    "The dimension of input 2 should be %d", options.x_size);
  }

  /* Obtain double data pointers from mxArrays. */
  sim_time = mxGetPr(prhs[0]);  /* States at time t. */
  x = mxGetPr(prhs[1]);  /* States at time t. */
  u = mxGetPr(prhs[2]);  /* Inputs at time t. */

  p = mxCalloc(np, sizeof(double*));
  for (i = 0; i < np; i++)
    p[i] = mxGetPr(prhs[3+i]); /* Parameter arrays. */

  /* Create matrix for the return arguments. */
  plhs[0] = mxCreateDoubleMatrix(nx, 1, mxREAL);
  plhs[1] = mxCreateDoubleMatrix(options.y_size, 1, mxREAL);
  xp      = mxGetPr(plhs[0]); /* State derivative values. */
  y       = mxGetPr(plhs[1]); /* Output values. */

  #ifdef SYSTEM_IDENTIFICATION
  double repeat = x[1];
  xp[1] = x[1];
  while (x[1] == xp[1]) {
    hyb_errorcode ret = hyb_main_loop(&options, y, xp, sim_time[0], x, u, (const double**) p);
    error_message(ret);
  }
  #else
    hyb_errorcode ret = hyb_main_loop(&options, y, xp, sim_time[0], x, u, (const double**) p);
    error_message(ret);
  #endif
  mxFree(p);
}

#endif