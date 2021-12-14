#include "algorithms.h"
#include <math.h>

/**
 * Function: bellman_ford
 * Description: Run Bellman-Ford algorithm
 * Inputs: graph, source node
 * Output: distance, predecessor
 */
void bellman_ford(igraph_t *graph, igraph_vector_t *vertices, igraph_vector_t *edges, igraph_integer_t from, igraph_integer_t to, igraph_vector_t *weights)
{    
    igraph_get_shortest_path_bellman_ford(graph,
                                          vertices,
                                          edges,
                                          from,
                                          to,
                                          weights,
                                          IGRAPH_ALL);

    
    
}

/**
 * Function: link_weights
 * Description: Calculates the weight of each link
 * Inputs: graph, routerType, trafficType
 * Output: weight in vector
 */
void cal_link_weights(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, link *links_array, igraph_vector_t *edges, igraph_vector_t *weights)
{
    int i;
    int j;
    
    igraph_vector_t router_weights;


    igraph_vector_init(&router_weights, igraph_vcount(graph));
    igraph_get_edgelist(graph, edges, false);

    /* Calculate the weight of each router */
    for (int i = 0; i < igraph_ecount(graph); i++)
    {
        if (links_array[i].utilisation < 80)
        {
            igraph_vector_set(weights, i, f(links_array[i].utilisation));
        }
        else if (links_array[i].utilisation >= 80)
        {
            igraph_vector_set(weights, i, g(links_array[i].utilisation));
        }
        else
        {
            links_array[i].utilisation = 0;
        }
    }
    
}

/**
 * Function: f
 * Description: Calculates the weight of each link below 80% utilisation
 * Inputs: utilisation
 * Output: weight
 */
double f(double x)
{
    return ((1.0 / 32.0) * pow(x, 2)) - (5.0 * x) + 200.0;
}

/**
 * Function: g
 * Description: Calculates the weight of each link above 80% utilisation
 * Inputs: utilisation
 * Output: weight
 */
double g(double x)
{
    return (5.0 / 8.0) * pow(x, 2) - 100.0 * x + 4000.0;
}