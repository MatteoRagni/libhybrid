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

 /**
  * @file mex_wrapper.c
  * @author Matteo Ragni, Matteo Cocetti
  * @date 10 Jan 2018
  */

/**
 * @brief To include the mex wrappe in your project you MUST define MATLAB_WRAPPER
 */
#ifdef MATLAB_WRAPPER

#include "libhybrid.h"
#include "mex.h"

#ifndef HYB_MODEL_SOURCE
/**
 * @brief Name of the model to wrap
 *
 * Please provide the name of source of the model to wrap. It will be included
 * in the wrapper for compilation. The model should include **at least**:
 *  * a flow map function
 *  * a jump map function
 *  * a jump set function
 *  * an output function
 *  * an hyb_opts structure with the name "options", and all the information set.
 */
#define HYB_MODEL_SOURCE "model.c"
#endif
#include HYB_MODEL_SOURCE

/**
 * @brief MATLAB System Identification wrapper and common wrapper are different
 *
 * The System Identification Wrapper and the common wrapper are different. The
 * normal wrapper returns for each step, and it does not matter if the step is a
 * either flow step or jump step. In the System Identification wrapper the function
 * returns only in case of flow step. If there is more than one jump step it is
 * not reported to the MATLAB engine. This forces the user to carefully decide
 * which Jump Horizon to use. Reaching an horizon forces the raise of an error.
 * This wrapper separation allows to keep always the flow time state synchronized with
 * the System Identification sim time.
 * This avoid the insertion of a delay \f$ e^{-T_s s} \f$ for each jump in the
 * flow dynamic.
 * For the System identification wrapper, simply define MATLAB_SYSTEM_IDENTIFICATION.
 * If MATLAB_SYSTEM_IDENTIFICATION is not defined, the common wrapper is generated.
 */
// #define MATLAB_SYSTEM_IDENTIFICATION


#if defined(_MSC_VER)
#define inline __inline /**< For compatibility with Microsoft Compiler */
#endif

/**
 * @brief handler for errors from hyb_main_loop function
 *
 * This function handles the error code from the hyb_main_loop function.
 * Any returning value different from HYB_SUCCESS raises a mexErrMsgIdAndTxt
 * with a convenient error message.
 * @param c the error code from the hyb_main_loop function
 */
inline void error_message(hyb_errorcode c) {
  switch (c) {
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


/**
 * @brief Entry point for MATLAB Api
 *
 * The compilation and linking of a mex function generates a shared object that
 * is dynamically loaded in MATLAB. The entry point for such library is the
 * function mexFunction that is always called with the following arguments:
 * @param nlhs number of elements in the pointer for the output (lhs = left hand side)
 * @param plhs pointer for data in the left hand side
 * @param nrhs number of elements in the pointer for the input arguments (rhs = right hand side)
 * @param prhs pointer for input data for the function
 */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]) {
  /* Declaration of input and output arguments. */
  double *x, *u, **p, *xp, *y, *sim_time;
  int     i, np;
  size_t  nu, nx;

  if (nrhs < 3) {
      mexErrMsgIdAndTxt("LIBHYBRID:Model:InvalidSyntax",
      "At least 4 inputs expected (t, u, x, p).");
  }

  np = nrhs - 3;

  /* Determine number of inputs and states. */
  nx = mxGetNumberOfElements(prhs[1]); /* Number of states. */
  nu = mxGetNumberOfElements(prhs[2]); /* Number of inputs. */

  if (nx != options.x_size + 2) {
    mexErrMsgIdAndTxt("LIBHYBRID:Model:InvalidSyntax",
    "The dimension of input 2 should be %d", options.x_size + 2);
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

  #ifdef MATLAB_SYSTEM_IDENTIFICATION
    double *z = mxCalloc(options.x_size + 2, sizeof(double));
    hyb_bool is_step = hyb_true;
    for(size_t i = 0; i < options.x_size + 2; i++)
        z[i] = x[i];

    while (is_step) {
      hyb_errorcode ret = hyb_main_loop(&options, y, xp, sim_time[0], z, u, (const double**) p);
      if (z[1] != xp[1]) {
        for(size_t i = 0; i < options.x_size + 2; i++)
          z[i] = xp[i];
        is_step = hyb_true;
      } else {
        is_step = hyb_false;
      }
      error_message(ret);
    }
    mxFree(z);
  #else
    hyb_errorcode ret = hyb_main_loop(&options, y, xp, sim_time[0], x, u, (const double**) p);
    error_message(ret);
  #endif
  mxFree(p);
}

#endif
