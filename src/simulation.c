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
    populate_network(100,3);
    run_simulation_loop();
}

/**
 * Description: Populate network with nodes and edges
 * Inputs: Validated data
 * Output: struct graph
 */
void populate_network(int nodes, int edges_per_node)
{
    // Initialize variables
    igraph_t graph;
    igraph_vector_t edges;
    int i;
    int j;

    // Initialize graph
    // TODO: Add dynamic graph size
    igraph_barabasi_game(/* graph=    */ &graph,
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


    // Print edge count in graph
    printf("Edge count: %d\n", igraph_ecount(&graph));
    // Print vertex count in graph
    printf("Vertex count: %d\n", igraph_vcount(&graph));

    // Print first edge in graph
    igraph_edge(&graph, 15, &i, &j);
    printf("First edge: %d, %d\n", i, j);

    // Print all edges in graph
    igraph_vector_init(&edges, 0);
    igraph_get_edgelist(&graph, &edges, 0);
    for (i = 0; i < igraph_vector_size(&edges); i++)
    {
        printf("Edge %d: %d, %d\n", i, (int)VECTOR(edges)[i], (int)VECTOR(edges)[i+1]);
    }
    
    
    
    // Free memory
    igraph_destroy(&graph);

    
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