#include <igraph.h>
#include "data.h"

void bellman_ford(igraph_t *, igraph_vector_t *, igraph_integer_t, igraph_integer_t, igraph_vector_t *);
void cal_link_weights(igraph_t *, struct routerType *, struct trafficType *, double *, igraph_vector_t *);
double f(double);
double g(double);

