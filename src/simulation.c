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

    event *events;

    // Initialise router utilisation array

    events = malloc(sizeof(event) * EVENT_COUNT);

    

    

    // Create random events
    create_events(graph, traffic, events);

    // Run simulation
    send_data(graph, routers, traffic, events);


    /* Free memory */
    free(events);
}

void create_events(igraph_t *graph, trafficType *traffic, event *events)
{
    int i;

    for (i = 0; i < EVENT_COUNT; i++)
    {
        events[i].time = (rand() % SIMULATION_TIME) + 1;
        events[i].type = (rand() % NMBR_OF_TRAFFICTYPES);
        events[i].source_id = (rand() % igraph_vcount(graph));
        events[i].destination_id = (rand() % igraph_vcount(graph));
        events[i].data = traffic[events[i].type].data_size;
    }
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, double *utilisation, igraph_vector_t *weights, igraph_vector_t *vertices, int from, int to)
{
    cal_link_weights(graph, routers, traffic, utilisation, weights);
    bellman_ford(graph, vertices, from, to, weights);
    
}

/**
 * Description: Send data between nodes and edges
 * Inputs:
 * Output:
 */
void send_data(igraph_t *graph, routerType *routers, trafficType *traffic, event *events)
{
    double *utilisation;
    igraph_vector_t weights;
    igraph_vector_t vertices;
    utilisation = malloc(sizeof(double) * igraph_vcount(graph));

    // Initialise vector
    igraph_vector_init(&weights, igraph_vcount(graph));

    // Set utilisation to 0
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        utilisation[i] = 0;
    }

    /* loop while there are events */


    // ! Used when a new event is happening
    establish_connections(graph, routers, traffic, utilisation, &weights, &vertices, events[0].source_id, events[0].destination_id);

    /* Free memory */
    free(utilisation);
    igraph_vector_destroy(&weights);
}