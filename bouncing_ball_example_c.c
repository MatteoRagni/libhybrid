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
  * @file bouncing_ball_example.c
  * @author Matteo Ragni, Matteo Cocetti
  * @date 10 Jan 2018
  */

/**
 * @brief This file contains an example of a model for the System Identification Toolbox
 *
 * The model here presented is in the form:
 *
 * \f{eqnarray*}
 *  \dot{x_1} &= x_2 & \\
 *  \dot{x_2} &= -g & \text{for } x \in C \\
 * \f}
 * \f{eqnarray*}
 *  x_1^+ &= 0 & \\
 *  x_2^+ &= -k x_2 & \text{for } x \in D \\
 * \f}
 * \f{eqnarray*}
 *  C = \{ x \geq 0 \} & D = \{ x \leq 0 \}
 * \f}
 *
 * To have it working we must define all the callbacks and setup all
 * the options in the hyb_opts struct.
 */

#define MODEL_Y_SIZE     1          /**< Output size */
#define MODEL_X_SIZE     2          /**< State size */
#define MODEL_TS         1e-3       /**< Integration step */
#define MODEL_T_HORIZON  30.0       /**< Maximum time horizon */
#define MODEL_J_HORIZON  30.0       /**< Maximum jump horizon */

/**
 * @brief Flow map for the bouncing ball
 */
void BouncingBall_flow_map(hyb_float *xdot, hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p) {
  xdot[0] = x[1];
  xdot[1] = -p[0][0];
}

/**
 * @brief Jump map for the bouncing ball
 */
void BouncingBall_jump_map(hyb_float *xp, hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p) {
   xp[0] = 0;
   xp[1] = -p[1][0] * x[1];
 }

/**
 * @brief Jump set for the bouncing ball
 */
hyb_bool BouncingBall_jump_set(hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p) {
  return x[0] <= 0 ? hyb_true : hyb_false;
}

/**
 * @brief Flow set for the bouncing ball
 */
hyb_bool BouncingBall_flow_set(hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p) {
  return x[0] >= 0 ? hyb_true : hyb_false;
}

/**
 * @brief Output map for the bouncing ball
 */
void BouncingBall_out_map(hyb_float *y, hyb_float t, hyb_float j, const hyb_float *x, const hyb_float *u, const hyb_float **p) {
  y[0] = x[0];
}


/**
 * @brief Runtime options for the hybrid system
 */
hyb_opts options = {
  MODEL_Y_SIZE,           /**< Output size */
  MODEL_X_SIZE,           /**< State size */
  MODEL_TS,               /**< Integration step */
  MODEL_T_HORIZON,        /**< Maximum time horizon */
  MODEL_J_HORIZON,        /**< Maximum jump horizon */
  BouncingBall_flow_map,  /**< Flow Map */
  BouncingBall_jump_map,  /**< Jump Map */
  BouncingBall_out_map,   /**< Output Map */
  BouncingBall_jump_set,  /**< Jump Set */
  BouncingBall_flow_set   /**< Flow Set */
};
