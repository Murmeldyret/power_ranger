#include <igraph/igraph.h>
#include "commons.h"
#include "data.h"

void bellman_ford(igraph_t *, igraph_vector_t *, igraph_vector_t *, igraph_integer_t, igraph_integer_t, igraph_vector_t *);
void cal_link_weights(igraph_t *graph, struct routerType *, struct trafficType *, link *, igraph_vector_t *, igraph_vector_t *);
double f(double);
double g(double);
double linear_power_con(int, int, double, double);