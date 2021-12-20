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

    for (int i = 0; i < nodes; i++)
    {
        igraph_vector_destroy(&routers_array[i].att_links);
    }
    free(routers_array);
    for (int i = 0; i < igraph_ecount(&graph); i++)
    {
        igraph_vector_destroy(&links_array[i].events);
    }
    free(links_array);
    igraph_destroy(&graph);
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
                         /* outseq=   */ NULL,
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
        igraph_vector_init(&routers[i].att_links, 0);
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

        igraph_vector_init(&links_array[i].events, 0);
        j += 2;
    }

    /* Iterate through all edges and add edges to routers */
    j = 0;
    for (i = 0; i < igraph_ecount(graph); i++)
    {
        igraph_vector_push_back(&routers[(int)igraph_vector_e(&edges, j)].att_links, i);
        igraph_vector_push_back(&routers[(int)igraph_vector_e(&edges, j + 1)].att_links, i);
        j += 2;
    }

    /* Free memory */
    igraph_vector_destroy(&edges);
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
    for (int i = 0; i < EVENT_COUNT; i++)
    {
        igraph_vector_destroy(&events[i].path);
        igraph_vector_destroy(&events[i].path_edges);
    }
    free(events);
}

void create_events(igraph_t *graph, trafficType *traffic, event *events)
{
    int i;

    for (i = 0; i < EVENT_COUNT; i++)
    {
        events[i].time = (rand() % SIMULATION_TIME) + 1;
        events[i].type = (rand() % NMBR_OF_TRAFFICTYPES);
        events[i].is_active = false;
        events[i].source_id = (rand() % igraph_vcount(graph));
        events[i].destination_id = (rand() % igraph_vcount(graph));
        events[i].data = (double)traffic[events[i].type].data_size;
        events[i].bandwidth = (double)traffic[events[i].type].speed;
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

    int ongoing_events = 0;
    int clock = 0; /* In milliseconds */
    double data_transfer = 0;
    double progress = 0;

    // Set utilisation to 0
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        router_array[i].utilisation = 0;
    }

    /* Run simulation and loop while there are ongoing events */
    while (clock < SIMULATION_TIME * 1000 || ongoing_events) // TODO: Change && to || because else ongoing events won't count. However for debugging it is practical to not include ongoing events for now.
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

                /* Set event to active */
                events[i].is_active = true;
                ongoing_events += 1;

                /* Add latency to event */
                for (int j = 0; j < igraph_vector_size(&events[i].path); j++)
                {
                    events[i].latency += routers[router_array[(int)igraph_vector_e(&events[i].path, j)].type].latency;
                }

                bandwidth_balancer(i, &events[i].path_edges, links_array, events);
                cal_utilisation(igraph_vcount(graph), igraph_ecount(graph), router_array, links_array, events);
            }
        }

        /* Handle ongoing events */
        if (ongoing_events)
        {
            for (int i = 0; i < EVENT_COUNT; i++)
            {
                if (events[i].time * 1000 < clock && events[i].data > 0)
                {

                    /* Is latency greater than 0? */
                    if (events[i].latency > 0)
                    {
                        /* Decrease latency */
                        events[i].latency--;
                    }
                    else
                    {
                        // Converting from Kb to KB
                        data_transfer = events[i].available_bandwidth / 1000 / 8;

                        /* Subtract data from event */
                        events[i].data -= data_transfer;

                        if (events[i].data <= 0)
                        {
                            events[i].is_active = false;
                            remove_event_from_links(i, &events[i].path_edges, links_array);
                            /* Release bandwidth on links */
                            release_bandwidth(i, &events[i].path_edges, links_array, events);
                            ongoing_events -= 1;
                        }
                    }
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
    double sum_bandwidth;
    double temp_bandwidth;
    igraph_vector_t link_overload;

    igraph_vector_init(&link_overload, 0);

    /* Check if there is enough bandwidth on each link */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        sum_bandwidth = 0;
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
            sum_bandwidth = 0;
            /* initialise variables */
            double *bandwidth_percentage = (double *)malloc(sizeof(double) * igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events));

            /* Sum bandwidth for each event */
            for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events); j++)
            {
                sum_bandwidth += event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].bandwidth;
            }

            /* Calculate bandwidth percentage */
            for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events); j++)
            {
                bandwidth_percentage[j] = event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].bandwidth / sum_bandwidth;
            }

            /* Apply bandwidth percentage to each event */
            for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events); j++)
            {
                double new_bandwidth = bandwidth_percentage[j] * links_array[(int)igraph_vector_e(&link_overload, i)].max_bandwidth;
                if (event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth > new_bandwidth)
                {
                    /* release bandwidth on link if the event is not event_id */
                    if (igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j) != event_id)
                    {
                        temp_bandwidth = event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth - new_bandwidth;
                        links_array[(int)igraph_vector_e(&link_overload, i)].remaining_bandwidth += temp_bandwidth;
                    }
                    else
                    {
                        links_array[(int)igraph_vector_e(&link_overload, i)].remaining_bandwidth -= new_bandwidth;
                    }

                    event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth = new_bandwidth;
                }
            }

            /* Free memory */
            free(bandwidth_percentage);
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

void cal_utilisation(int router_len, int link_len, router *router_array, link *links_array, event *event)
{
    /* Iterate through all links */
    for (int i = 0; i < link_len; i++)
    {
        /* Calculate utilisation */
        links_array[i].utilisation = (((double)links_array[i].max_bandwidth - links_array[i].remaining_bandwidth) / (double)links_array[i].max_bandwidth) * 100;
    }

    /* Iterate through all routers */
    for (int i = 0; i < router_len; i++)
    {
        /* Calculate router utilisation */
        double avg_utilisation = 0;
        for (int j = 0; j < igraph_vector_size(&router_array[i].att_links); j++)
        {
            avg_utilisation += links_array[(int)igraph_vector_e(&router_array[i].att_links, j)].utilisation;
        }

        avg_utilisation /= (double)igraph_vector_size(&router_array[i].att_links);

        router_array[i].utilisation = avg_utilisation;
    }
}

void remove_event_from_links(int event_id, igraph_vector_t *path_edges, link *links_array)
{
    /* Iterate through all links */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        /* Iterate through all events in link */
        for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(path_edges, i)].events); j++)
        {
            /* Remove event from link */
            if ((int)igraph_vector_e(&links_array[(int)igraph_vector_e(path_edges, i)].events, j) == event_id)
            {
                igraph_vector_remove(&links_array[(int)igraph_vector_e(path_edges, i)].events, j);
            }
        }
    }
}

void release_bandwidth(int event_id, igraph_vector_t *path_edges, link *links_array, event *events)
{
    /* Iterate through links affected by event */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        /* Release bandwidth */
        links_array[(int)igraph_vector_e(path_edges, i)].remaining_bandwidth += events[event_id].available_bandwidth;
    }
}