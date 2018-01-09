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

#ifndef LIBHYBRID_H_
#define LIBHYBRID_H_

#if defined(_MSC_VER)
#define inline __inline
#endif

#ifndef HYB_FLOAT_TYPE
#define HYB_FLOAT_TYPE double /**< Precision defaut is double */
#endif
typedef HYB_FLOAT_TYPE hyb_float; /**< Precision typedef, used in library */

#define RK4_FLOAT_TYPE HYB_FLOAT_TYPE /**< Forcing consistent types */
#include "librk4.h"

/**
 * @brief libhybrid boolean type is actually an enum
 *
 * This enum is the returned value for the Jump Set function and the
 * Flow Set function.
 */
typedef enum hyb_bool {
  hyb_false = 0; /**< False value */
  hyb_true = 1; /**< True value */
} hyb_bool;

/**
 * @brief Callback definition for the Flow Map
 *
 * The callback stores the output in the first pointer (xdot).
 * The function does not need to allocate nor free the output vector,
 * but may overflow if exceedes the dimension that is declared in the
 * hyb_opts structure. Please notice that this function will be discretized
 * in a Runge Kutta 4 integrator: librk4.h.
 * When accessing an element of the parameter, please remember that the
 * implementation keeps in mind a MATLAB-like interface. If the parameters
 * (in MATLAB) are passed like:
 * @code
 * f(t, x, u, p1, [p2, p3])
 * @endcode
 * then, in the C code, p1, p2 and p3 may be accessed in p as:
 * @code
 * rk4_float p1, p2, p3;
 * p1 = p[0][0];
 * p2 = p[1][0];
 * p3 = p[1][1];
 * @endcode
 * @param xdot an array in which the result will be stored
 * @param t is the current time
 * @param j is the  discrete time
 * @param x a constant array with the current state
 * @param u a constant array with current input
 * @param p a pointer to arrays of parameters. This allows the compatibility with the
 *          MATLAB System Identification Toolbox
 */
typedef void (*hyb_flow_map)(hyb_float *xdot, hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p);

/**
 * @brief Callback definition for the Jump Map
 *
 * The callback stores the output in the first pointer (xp).
 * The function does not need to allocate nor free the output vector,
 * but may overflow if exceedes the dimension that is declared in the
 * hyb_opts structure.
 * When accessing an element of the parameter, please remember that the
 * implementation keeps in mind a MATLAB-like interface. If the parameters
 * (in MATLAB) are passed like:
 * @code
 * f(t, x, u, p1, [p2, p3])
 * @endcode
 * then, in the C code, p1, p2 and p3 may be accessed in p as:
 * @code
 * rk4_float p1, p2, p3;
 * p1 = p[0][0];
 * p2 = p[1][0];
 * p3 = p[1][1];
 * @endcode
 * @param xp an array in which the result will be stored
 * @param t is the current time
 * @param j is the  discrete time
 * @param x a constant array with the current state
 * @param u a constant array with current input
 * @param p a pointer to arrays of parameters. This allows the compatibility with the
 *          MATLAB System Identification Toolbox
 */
typedef void (*hyb_jump_map)(hyb_float *xp, hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p);

/**
 * @brief Callback definition for the Output Map
 *
 * The callback stores the output in the first pointer (y).
 * The function does not need to allocate nor free the input vector,
 * but may overflow if exceedes the dimension that is declared in the
 * hyb_opts structure.
 * When accessing an element of the parameter, please remember that the
 * implementation keeps in mind a MATLAB-like interface. If the parameters
 * (in MATLAB) are passed like:
 * @code
 * f(t, x, u, p1, [p2, p3])
 * @endcode
 * then, in the C code, p1, p2 and p3 may be accessed in p as:
 * @code
 * rk4_float p1, p2, p3;
 * p1 = p[0][0];
 * p2 = p[1][0];
 * p3 = p[1][1];
 * @endcode
 * @param xp an array in which the result will be stored
 * @param t is the current time
 * @param j is the  discrete time
 * @param x a constant array with the current state
 * @param u a constant array with current input
 * @param p a pointer to arrays of parameters. This allows the compatibility with the
 *          MATLAB System Identification Toolbox
 */
typedef void (*hyb_jump_map)(hyb_float *xp, hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p);


/**
 * @brief Callback definition for the Jump Set
 *
 * The callback should return hyb_true if the combination of state, input
 * and parameters enters the jump set.
 * When accessing an element of the parameter, please remember that the
 * implementation keeps in mind a MATLAB-like interface. If the parameters
 * (in MATLAB) are passed like:
 * @code
 * f(t, x, u, p1, [p2, p3])
 * @endcode
 * then, in the C code, p1, p2 and p3 may be accessed in p as:
 * @code
 * rk4_float p1, p2, p3;
 * p1 = p[0][0];
 * p2 = p[1][0];
 * p3 = p[1][1];
 * @endcode
 * @param t is the current time
 * @param j is the  discrete time
 * @param x a constant array with the current state
 * @param u a constant array with current input
 * @param p a pointer to arrays of parameters. This allows the compatibility with the
 *          MATLAB System Identification Toolbox
 */
typedef hyb_bool (*hyb_jump_set)(hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p);


/**
 * @brief Callback definition for the Flow Set
 *
 * The callback should return hyb_true if the combination of state, input
 * and parameters enters the flow set.
 * When accessing an element of the parameter, please remember that the
 * implementation keeps in mind a MATLAB-like interface. If the parameters
 * (in MATLAB) are passed like:
 * @code
 * f(t, x, u, p1, [p2, p3])
 * @endcode
 * then, in the C code, p1, p2 and p3 may be accessed in p as:
 * @code
 * rk4_float p1, p2, p3;
 * p1 = p[0][0];
 * p2 = p[1][0];
 * p3 = p[1][1];
 * @endcode
 * @param t is the current time
 * @param j is the  discrete time
 * @param x a constant array with the current state
 * @param u a constant array with current input
 * @param p a pointer to arrays of parameters. This allows the compatibility with the
 *          MATLAB System Identification Toolbox
 */
typedef hyb_bool (*hyb_jump_set)(hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p);

#ifndef HYB_JUMP_LOGIC
#define HYB_JUMP_LOGIC 1 /**< Default jump logic cosiders only the jump set */
#endif
#define HYB_JUMP_LOGIC
#if HYB_JUMP_LOGIC == 1
/**
 * There are three possible jump logic:
 *
 *  1. Precedence to jump set only (HYB_JUMP_LOGIC defined 1. It is also the default)
 *  2. Precedence on the flow set (HYB_JUMP_LOGIC defined 2)
 *  3. Precedence is establish via random number if flow and jump are both true (HYB_JUMP_LOGIC defined 3)
 *  4. Always flow
 *
 * @param d jump set result
 * @param c flow set result
 * @return the result of the jump logic
 */
inline hyb_bool jump_logic(hyb_bool d, hyb_bool c) {
  return d;
}
#endif
#if HYB_JUMP_LOGIC == 2
inline hyb_bool jump_logic(hyb_bool d, hyb_bool c) {
  return (!c && d);
}
#endif
#if HYB_JUMP_LOGIC == 3
#include <time.h>
#include <stdlib.h>
extern hyb_bool hyb_seed_init = 0; /**< Flag for seed initialization. Must be done once */
hyb_bool jump_logic(hyb_bool d, hyb_bool c) {
  if (!hyb_seed_init)
    srand(time(NULL));

  return (!c && d) || (c && d && rand());
}
#endif
#if HYB_JUMP_LOGIC == 4
inline hyb_bool jump_logic(hyb_bool d, hyb_bool c) { return hyb_false; }
#endif


/**
 * @brief Options structure for the hybrid system
 */
typedef struct hyb_opts {
  size_t y_size; /**< Output size */
  rk4_opts *rk4_o; /**< Discretization options, using Runge Kutta step */
  hyb_float T_horizon; /**< Maximum continuous time horizon */
  hyb_float J_horizon; /**< Maximum discrete step horizon */
  hyb_flow_map F; /**< Flow map function pointer */
  hyb_jump_map J; /**< Jump map function pointer */
  hyb_out_map Y; /**< Output map function pointer */
  hyb_jump_set D; /**< Jump set function pointer */
  hyb_flow_set C; /**< Flow set function pointer */
} hyb_opts;


typedef enum hyb_errno {
  HYB_SUCCESS = 0, /**< Step seems good! */
  HYB_EMALLOC,     /**< During the step there was an allocation error */
  HYB_NULLPTR,     /**< Reveived a null pointer */
  HYB_GENERIC,     /**< Unknown error generated */
  HYB_TLIMIT,      /**< Reached time limit */
  HYB_JLIMIT,      /**< Reached step limit */
  HYB_NOJUMP       /**< Invalid jump condition (Both c = 0 and d = 0) */
} hyb_errno;

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
hyb_errno hyb_main_loop(hyb_opts *opts, hyb_float *y, hyb_float *xp, hyb_float tau, const hyb_float *x, const hyb_float *u, const hyb_float **p);

void hyb_jump_map_wrapper(hyb_opts *opts, hyb_float *xp, hyb_float tau, const hyb_float *x, const hyb_float *u, const hyb_float **p, void *vopts);
void hyb_flow_map_wrapper(hyb_opts *opts, hyb_float *xp, hyb_float tau, const hyb_float *x, const hyb_float *u, const hyb_float **p, void *vopts);

#endif /* LIBHYBRID_H_ */
