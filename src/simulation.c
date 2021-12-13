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

    router *routers_array = malloc(nodes * sizeof(struct routerType));

    // Initialize array of Router
    populate_network(nodes, edges, &graph, routers_array);
    run_simulation_loop(&graph, routers, traffic, routers_array);

    // Free memory
    igraph_destroy(&graph);
    free(routers_array);
}

/**
 * Description: Populate network with nodes and edges
 * Inputs: Validated data
 * Output: struct graph
 */
void populate_network(int nodes, int edges_per_node, igraph_t *graph, router *routers)
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

    /* Create random router types in array */
    for (i = 0; i < nodes; i++)
    {
        routers[i].type = rand() % NMBR_OF_ROUTERTYPES;
    }
}

/**
 * Description: Run simulation loop
 * Inputs: Validated data, graph
 * Output: struct simulationData
 */
void run_simulation_loop(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, router *routers_array)
{
    // Initialize variables

    event *events;

    // Initialise router utilisation array

    events = malloc(sizeof(event) * EVENT_COUNT);

    // Create random events
    create_events(graph, traffic, events);

    // Run simulation
    send_data(graph, routers, traffic, events, routers_array);

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
        igraph_vector_init(&events[i].path, 0);
    }
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, router *router_array, igraph_vector_t *weights, igraph_vector_t *vertices, int from, int to)
{
    igraph_vector_t edges;
    igraph_vector_init(&edges, igraph_ecount(graph));

    cal_link_weights(graph, routers, traffic, router_array, &edges, weights);
    bellman_ford(graph, vertices, &edges, from, to, weights);
}

/**
 * Description: Send data between nodes and edges
 * Inputs:
 * Output:
 */
void send_data(igraph_t *graph, routerType *routers, trafficType *traffic, event *events, router *router_array)
{
    /* Initialize variables */
    igraph_vector_t weights;
    bool ongoing_events = true;
    int clock = 0; /* In milliseconds */

    // Initialise vector
    igraph_vector_init(&weights, igraph_ecount(graph));


    // Set utilisation to 0
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        router_array[i].utilisation = 0;
    }

    /* Run simulation and loop while there are ongoing events */
    while (clock < SIMULATION_TIME * 1000 && ongoing_events)
    {
        /* Check if there are any events */
        for (int i = 0; i < EVENT_COUNT; i++)
        {
            if (events[i].time * 1000 == clock)
            {
                // Establish connections
                establish_connections(graph, routers, traffic, router_array, &weights, &events[i].path, events[i].source_id, events[i].destination_id);
                ongoing_events = true;
            }
        }

        /* Move clock forward */
        clock++;
    }

    /* Free memory */
    igraph_vector_destroy(&weights);
}