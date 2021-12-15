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
        routers[i].utilisation = 0;
        routers[i].sleeping = false;
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
        links_array[i].utilisation = 0.0;
        igraph_vector_init(&links_array[i].events, 0);
        j += 2;
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

    events = (event *)malloc(sizeof(event) * EVENT_COUNT);

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
        events[i].available_bandwidth = events[i].bandwidth;
        igraph_vector_init(&events[i].path, 0);
        igraph_vector_init(&events[i].path_edges, 0);
        events[i].latency = 0;
    }
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, link *links_array, igraph_vector_t *edges, igraph_vector_t *vertices, int from, int to)
{
    igraph_vector_t weights; // TODO: Move into establish connections so that it doesn't have to be a parameter.

    igraph_vector_init(&weights, igraph_ecount(graph));

    cal_link_weights(graph, routers, traffic, links_array, edges, &weights);
    bellman_ford(graph, vertices, edges, from, to, &weights);

    /* Free memory */
    igraph_vector_destroy(&weights);
}

/**
 * Description: Send data between nodes and edges
 * Inputs:
 * Output:
 */
void send_data(igraph_t *graph, routerType *routers, trafficType *traffic, event *events, router *router_array, link *links_array)
{
    /* Initialize variables */

    int ongoing_events = 1;
    int clock = 0; /* In milliseconds */

    // Set utilisation to 0
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        router_array[i].utilisation = 0;
    }

    /* Run simulation and loop while there are ongoing events */
    while (clock < SIMULATION_TIME * 1000 && ongoing_events) // TODO: Change && to || because else ongoing events won't count. However for debugging it is practical to not include ongoing events for now.
    {
        /* Check if there are any events */
        for (int i = 0; i < EVENT_COUNT; i++)
        {
            if (events[i].time * 1000 == clock)
            {
                // Establish connections
                establish_connections(graph, routers, traffic, links_array, &events[i].path_edges, &events[i].path, events[i].source_id, events[i].destination_id);

                /* Add event to links event list */
                add_event_to_links(i, &events[i].path_edges, links_array);

                ongoing_events += 1;

                /* Add latency to event */
                for (int j = 0; j < igraph_vector_size(&events[i].path); j++)
                {
                    events[i].latency += routers[router_array[(int)igraph_vector_e(&events[j].path, j)].type].latency;
                }

                // ! Calculate available bandwidth for the links affected by the new connection.

                bandwidth_balancer(i, &events[i].path_edges, links_array, events);
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
}

void add_event_to_links(int event_id, igraph_vector_t *path_edges, link *links_array)
{
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        igraph_vector_push_back(&links_array[(int)igraph_vector_e(path_edges, i)].events, event_id);
    }
}

void bandwidth_balancer(int event_id, igraph_vector_t *path_edges, link *links_array, event *event)
{
    int sum_bandwidth = 0;
    igraph_vector_t link_overload;

    igraph_vector_init(&link_overload, 0);

    /* Check if there is enough bandwidth on each link */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        /* Sum available bandwidth in every event */
        for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(path_edges, i)].events); j++)
        {
            sum_bandwidth += event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(path_edges, i)].events, j)].available_bandwidth;
        }

        /* Check if there is enough bandwidth */
        if (sum_bandwidth > links_array[(int)igraph_vector_e(path_edges, i)].max_bandwidth)
        {
            igraph_vector_push_back(&link_overload, (int)igraph_vector_e(path_edges, i));
        }
    }

    /* Run if links are overloaded */
    if (igraph_vector_size(&link_overload) > 0)
    {
        /* Sort overloaded links by least remaining bandwidth */
        sort_links(links_array, &link_overload);

        /* Balance bandwidth for each link */
        for (int i = 0; i < igraph_vector_size(&link_overload); i++)
        {
            /* initialise variables */
            double *bandwidth_percentage = (double *)malloc(sizeof(double) * igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events));

            /* Calculate bandwidth percentage */
            for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events); j++)
            {
                bandwidth_percentage[j] = (double)event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth / (double)links_array[(int)igraph_vector_e(&link_overload, i)].max_bandwidth;
            }

            /* Apply bandwidth percentage to each event */
            for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events); j++)
            {
                event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth = (int)(bandwidth_percentage[j] * event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth);
            }
        }
    }

    /* Subtract bandwidth from links that are not overloaded */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        if (igraph_vector_contains(&link_overload, (int)igraph_vector_e(path_edges, i)))
        {
            continue;
        }
        else
        {
            links_array[(int)igraph_vector_e(path_edges, i)].remaining_bandwidth -= event[event_id].available_bandwidth;
        }
    }

    /* Free memory */
    igraph_vector_destroy(&link_overload);
}

void sort_links(link *links_array, igraph_vector_t *link_overload)
{
    int temp;
    for (int i = 0; i < igraph_vector_size(link_overload); i++)
    {
        for (int j = 0; j < igraph_vector_size(link_overload) - 1; j++)
        {
            if (links_array[(int)igraph_vector_e(link_overload, j)].remaining_bandwidth > links_array[(int)igraph_vector_e(link_overload, j + 1)].remaining_bandwidth)
            {
                temp = (int)igraph_vector_e(link_overload, j);
                VECTOR(*link_overload)
                [j] = (int)igraph_vector_e(link_overload, j + 1);
                VECTOR(*link_overload)
                [j + 1] = temp;
            }
        }
    }
}