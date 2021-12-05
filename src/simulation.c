#include "simulation.h"
#include "algorithms.h"
#include <time.h>

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
    run_simulation_loop();

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
    // TODO: Add dynamic graph size
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
void run_simulation_loop()
{
    establish_connections();
    send_data();
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections()
{
}

/**
 * Description: Send data between nodes and edges
 * Inputs:
 * Output:
 */
void send_data()
{
}