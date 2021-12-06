#include "algorithms.h"
#include <igraph.h>
#include <math.h>


/**
 * Function: bellman_ford
 * Description: Run Bellman-Ford algorithm
 * Inputs: graph, source node
 * Output: distance, predecessor
 */
void bellman_ford()
{

}

/**
 * Function: link_weights
 * Description: Calculates the weight of each link
 * Inputs: graph, routerType, trafficType
 * Output: weight in vector
 */
void cal_link_weights(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, igraph_vector_t *weights)
{

    printf("hello\n\n");
    // Print all vertexes
    printf("Vertexes:\n");
    
    

}

double f(double x)
{
    return (1/32)*pow(x,2)-5*x+200;
}

double g(double x)
{
    return (5/8)*pow(x,2)-100*x+4000;
}