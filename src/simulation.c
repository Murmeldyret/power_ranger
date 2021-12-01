#include "simulation.h"
#include "algorithms.h"

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
    igraph_vector_t dimvector;
    igraph_vector_t edges;
    int i;

    igraph_vector_init(&dimvector, 2);
    VECTOR(dimvector)[0] = 30;

    // test
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