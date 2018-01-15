# libhybrid - A library for discretized Hybrid Dynamical Systems

This library implements an hybrid system in the form:


<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{array}{rlr}&space;\dot{t}(\tau)&space;=&space;&&space;1&space;&&space;\text{for&space;}&space;(t,&space;j,&space;x,&space;u)&space;\in&space;C&space;\\&space;\dot{j}(\tau)&space;=&space;&&space;0&space;&&space;\\&space;\dot{x}(\tau)&space;=&space;&&space;f(t,&space;j,&space;x,&space;u,&space;p)&space;&&space;\\&space;\end{array}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{array}{rlr}&space;\dot{t}(\tau)&space;=&space;&&space;1&space;&&space;\text{for&space;}&space;(t,&space;j,&space;x,&space;u)&space;\in&space;C&space;\\&space;\dot{j}(\tau)&space;=&space;&&space;0&space;&&space;\\&space;\dot{x}(\tau)&space;=&space;&&space;f(t,&space;j,&space;x,&space;u,&space;p)&space;&&space;\\&space;\end{array}" title="\begin{array}{rlr} \dot{t}(\tau) = & 1 & \text{for } (t, j, x, u) \in C \\ \dot{j}(\tau) = & 0 & \\ \dot{x}(\tau) = & f(t, j, x, u, p) & \\ \end{array}" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{array}{rlr}&space;t^&plus;(\tau)&space;=&space;&&space;t&space;&&space;\text{for&space;}&space;(t,&space;j,&space;x,&space;u)&space;\in&space;D&space;\\&space;j^&plus;(\tau)&space;=&space;&&space;j&space;&plus;&space;1&space;&&space;\\&space;x^&plus;(\tau)&space;=&space;&&space;g(t,&space;j,&space;x,&space;u,&space;p)&space;&&space;\\&space;\end{array}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{array}{rlr}&space;t^&plus;(\tau)&space;=&space;&&space;t&space;&&space;\text{for&space;}&space;(t,&space;j,&space;x,&space;u)&space;\in&space;D&space;\\&space;j^&plus;(\tau)&space;=&space;&&space;j&space;&plus;&space;1&space;&&space;\\&space;x^&plus;(\tau)&space;=&space;&&space;g(t,&space;j,&space;x,&space;u,&space;p)&space;&&space;\\&space;\end{array}" title="\begin{array}{rlr} t^+(\tau) = & t & \text{for } (t, j, x, u) \in D \\ j^+(\tau) = & j + 1 & \\ x^+(\tau) = & g(t, j, x, u, p) & \\ \end{array}" /></a>

<a href="https://www.codecogs.com/eqnedit.php?latex=y&space;=&space;h(t,&space;j,&space;x,&space;u&space;,&space;p)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?y&space;=&space;h(t,&space;j,&space;x,&space;u&space;,&space;p)" title="y = h(t, j, x, u , p)" /></a>

where:

 * _f_ is the flow map;
 * _g_ is the jump map;
 * _h_ is the output map;
 * _C_ is the flow set;
 * _D_ is the jump set.
 * _p_ are parameters.
 * is an engine time for the integration of _t_ and _j_.

The flow map is discretized with a Runge Kutta 4 step. For the evolution of
the system, both _t_ and _j_ are limited by horizons.
