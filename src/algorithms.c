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
void cal_link_weights(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, double *utilisation, igraph_vector_t *weights)
{

    for(int i = 0; i < igraph_vcount(graph); i++)
    {
        if (utilisation[i] < 80)
        {
            igraph_vector_set(weights,i,f(utilisation[i]));
        }
        else if (utilisation[i] >= 80)
        {
            igraph_vector_set(weights,i,g(utilisation[i]));
        }
        else
        {
            utilisation[i] = 01;
        }
    }

    //print weights
    for(int i = 0; i < igraph_vcount(graph); i++)
    {
        printf("%f\n", VECTOR(*weights)[i]);
    }


}

double f(double x)
{
    return (1/32)*pow(x,2)-5*x+200;
}

double g(double x)
{
    return (5/8)*pow(x,2)-100*x+4000;
}