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
    printf("Bellman-Ford algorithm\n");
    
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
void cal_link_weights(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, double *utilisation, igraph_vector_t *edges, igraph_vector_t *weights)
{
    int i;
    int j;
    
    igraph_vector_t router_weights;


    igraph_vector_init(&router_weights, igraph_vcount(graph));
    igraph_get_edgelist(graph, edges, false);

    /* Calculate the weight of each router */
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        if (utilisation[i] < 80)
        {
            igraph_vector_set(&router_weights, i, f(utilisation[i]));
        }
        else if (utilisation[i] >= 80)
        {
            igraph_vector_set(&router_weights, i, g(utilisation[i]));
        }
        else
        {
            utilisation[i] = 0;
        }
    }

    printf("%d\n", VECTOR(*edges)[0]);

    j = 0;

    /* Calculate the weight of each link */
    for (i = 0; i < igraph_ecount(graph); i++)
    {
        /* Add weight of each router to the total weight */
        igraph_vector_set(weights, i, igraph_vector_e(&router_weights, igraph_vector_e(edges, j)) + igraph_vector_e(&router_weights, igraph_vector_e(edges, j + 1)));
        j += 2;
    }

    /* Print all weights */
    for (i = 0; i < igraph_ecount(graph); i++)
    {
        printf("%f\n", VECTOR(*weights)[i]);
    }
    /* Print lenght of weights vector */
    printf("%d\n", igraph_vector_size(weights));
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