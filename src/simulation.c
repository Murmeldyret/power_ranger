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

    router *routers_array = (router *)malloc(nodes * sizeof(struct router));
    link *links_array = (link *)malloc(nodes * edges * sizeof(link));

    // Initialize array of Router
    populate_network(nodes, edges, &graph, routers_array, links_array, routers);

    run_simulation_loop(&graph, routers, traffic, routers_array, links_array);

    // Free memory
    igraph_destroy(&graph);
    free(routers_array);
    free(links_array);
}

/**
 * Description: Populate network with nodes and edges
 * Inputs: Validated data
 * Output: struct graph
 */
void populate_network(int nodes, int edges_per_node, igraph_t *graph, router *routers, link *links_array, routerType *routers_types)
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

    igraph_vector_init(&edges, 0);
    igraph_get_edgelist(graph, &edges, false);

    /* Populate links array */
    links_array = (link *)realloc(links_array, igraph_ecount(graph) * sizeof(struct link));

    /* Initialize links array */
    j = 0;
    for (i = 0; i < igraph_ecount(graph); i++)
    {
        if (routers_types[routers[(int)igraph_vector_e(&edges, j)].type].bandwidth < routers_types[routers[(int)igraph_vector_e(&edges, j + 1)].type].bandwidth)
        {
            links_array[i].max_bandwidth = routers_types[routers[(int)igraph_vector_e(&edges, j)].type].bandwidth;
        }
        else
        {
            links_array[i].max_bandwidth = routers_types[routers[(int)igraph_vector_e(&edges, j + 1)].type].bandwidth;
        }
        links_array[i].remaining_bandwidth = links_array[i].max_bandwidth;
        links_array[i].utilisation = 0;
        j += 2;
    }

    /* Print all link utilisation */
    for (int i = 0; i < igraph_ecount(graph); i++)
    {
        printf("Link %d: %d\n", i, links_array[i].utilisation);
    }
}

/**
 * Description: Run simulation loop
 * Inputs: Validated data, graph
 * Output: struct simulationData
 */
void run_simulation_loop(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, router *routers_array, link *links_array)
{
    // Initialize variables

    event *events;

    // Initialise router utilisation array

    events = malloc(sizeof(event) * EVENT_COUNT);

    // Create random events
    create_events(graph, traffic, events);

    // Run simulation
    send_data(graph, routers, traffic, events, routers_array, links_array);

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
        events[i].bandwidth = traffic[events[i].type].speed;
        igraph_vector_init(&events[i].path, 0);
        events[i].latency = 0;
    }
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, link *links_array, igraph_vector_t *weights, igraph_vector_t *vertices, int from, int to)
{
    igraph_vector_t edges;
    igraph_vector_init(&edges, igraph_ecount(graph));

    cal_link_weights(graph, routers, traffic, links_array, &edges, weights);
    bellman_ford(graph, vertices, &edges, from, to, weights);
}

/**
 * Description: Send data between nodes and edges
 * Inputs:
 * Output:
 */
void send_data(igraph_t *graph, routerType *routers, trafficType *traffic, event *events, router *router_array, link *links_array)
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
                establish_connections(graph, routers, traffic, links_array, &weights, &events[i].path, events[i].source_id, events[i].destination_id);
                ongoing_events = true;

                /* Add latency to event */
                for (int j = 0; j < igraph_vector_size(&events[i].path); j++)
                {
                    events[i].latency += routers[router_array[(int)igraph_vector_e(&events[j].path, j)].type].latency;
                }
            }
        }

        /* Handle ongoing events */
        for (int i = 0; i < EVENT_COUNT; i++)
        {
            if (events[i].time * 1000 < clock && events[i].data > 0)
            {
                ongoing_events = true;

                /* Is latency greater than 0? */
                if (events[i].latency > 0)
                {
                    /* Decrease latency */
                    events[i].latency--;
                }
                else
                {
                    /* Send data */
                    send_data_to_router(graph, router_array, links_array, &events[i], &weights);
                }
            }
        }

        for (int i = 0; i < igraph_vector_size(&events->path); i++)
        {
            if (routers[router_array[i].type].power.peak)
            {
                router_array[i].utilisation += 0;
            }
        }

        /* Move clock forward */
        clock++;
    }

    /* Free memory */
    igraph_vector_destroy(&weights);
}