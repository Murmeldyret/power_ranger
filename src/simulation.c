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
    igraph_t graph;
    igraph_vector_t v;
    igraph_vector_t result;
    igraph_vector_t edges;
    int i;

    igraph_vector_init(&v, 2);
    VECTOR(v)[0] = 100;
    VECTOR(v)[1] = 100;
    // Create graph
    igraph_lattice(&graph, &v, 0, IGRAPH_UNDIRECTED, false, true);
    printf("Hello");
    igraph_rng_seed(igraph_rng_default(), time(NULL));
    igraph_vector_init(&edges, 50);
    for (i = 0; i < igraph_vector_size(&edges); i++)
    {
        VECTOR(edges)
        [i] = RNG_INTEGER(0, igraph_vcount(&graph) - 1);
    }

    igraph_add_edges(&graph, &edges, 0);

    

    printf("graph count %d", igraph_vcount(&graph));

    // test 2
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