#include "simulation.h"
#include "algorithms.h"
#include "data.h"
#include <time.h>

/**
 * Description: Run simulation of network
 * Inputs: Validated data
 * Output: struct simulationData
 */
void run_simulation()
{
    populate_network();
    run_simulation_loop();
}

/**
 * Description: Populate network with nodes and edges
 * Inputs: Validated data
 * Output: struct graph
 */
void populate_network()
{
    // Initialize variables
    igraph_t graph;
    igraph_vector_t dimvec;
    igraph_vector_t edges;
    int i;

    // Initialize graph
    // TODO: Add dynamic graph size
    igraph_barabasi_game(/* graph=    */ &graph,
                         /* n=        */ 100,
                         /* power=    */ 1.0,
                         /* m=        */ 3,
                         /* outseq=   */ 0,
                         /* outpref=  */ 0,
                         /* A=        */ 1.0,
                         /* directed= */ IGRAPH_DIRECTED,
                         /* algo=     */ IGRAPH_BARABASI_PSUMTREE,
                         /* start_from= */ 0);

    printf("Graph created\n");
    // Print edge count in graph
    printf("Edge count: %d\n", igraph_ecount(&graph));
    // Print vertex count in graph
    printf("Vertex count: %d\n", igraph_vcount(&graph));

    
}

/**
 * Description: Run simulation loop
 * Inputs: Validated data
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