#include "simulation.h"
#include <time.h>

#define EVENT_COUNT 100
/* In seconds */
#define SIMULATION_TIME 100

/**
 * Description: Run simulation of network
 * Inputs: Validated data
 * Output: struct simulationData
 */
void run_simulation(struct routerType *routers, struct trafficType *traffic)
{
    // Initialize variables for populate network
    int nodes;
    int edges;
    igraph_t graph;

    nodes = 100;
    edges = 3;

    // Initialize array of Router

    populate_network(nodes, edges, &graph);
    run_simulation_loop(&graph, routers, traffic);

    // Free memory
    igraph_destroy(&graph);
}

/**
 * Description: Populate network with nodes and edges
 * Inputs: Validated data
 * Output: struct graph
 */
void populate_network(int nodes, int edges_per_node, igraph_t *graph)
{
    // Initialize variables
    igraph_vector_t edges;
    int i;
    int j;

    // Initialize graph
    igraph_barabasi_game(/* graph=    */ graph,
                         /* n=        */ nodes,
                         /* power=    */ 1.0,
                         /* m=        */ edges_per_node,
                         /* outseq=   */ 0,
                         /* outpref=  */ 0,
                         /* A=        */ 1.0,
                         /* directed= */ IGRAPH_DIRECTED,
                         /* algo=     */ IGRAPH_BARABASI_PSUMTREE,
                         /* start_from= */ 0);

    printf("Graph created\n");
}

/**
 * Description: Run simulation loop
 * Inputs: Validated data, graph
 * Output: struct simulationData
 */
void run_simulation_loop(igraph_t *graph, struct routerType *routers, struct trafficType *traffic)
{
    // Initialize variables
    double *utilisation;
    igraph_vector_t weights;
    
    // Initialise router utilisation array
    utilisation = malloc(sizeof(double) * igraph_vcount(graph));

    // Set utilisation to 0
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        utilisation[i] = 0;
    }

    // Initialise vector
    igraph_vector_init(&weights, igraph_vcount(graph));

    // Create random events


    establish_connections(graph, routers, traffic, utilisation, &weights);
    send_data();

    /* Free memory */
    free(utilisation);
    igraph_vector_destroy(&weights);
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, double *utilisation, igraph_vector_t *weights)
{
    cal_link_weights(graph, routers, traffic, utilisation, weights);

}

/**
 * Description: Send data between nodes and edges
 * Inputs:
 * Output:
 */
void send_data()
{
}