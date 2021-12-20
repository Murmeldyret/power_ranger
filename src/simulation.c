#include "simulation.h"
#include <time.h>

/**
 * Description: Run simulation of network
 * Inputs: Validated data
 * Output: struct simulationData
 */
void run_simulation(struct routerType *routers, struct trafficType *traffic, simulationData *out_data, const sim_setup *setup)
{
    // Initialize variables for populate network
    int nodes;
    int edges;
    igraph_t graph;

    nodes = setup->nodes;
    edges = setup->edges_per_node;

    router *routers_array = (router *)malloc(nodes * sizeof(struct router));
    link_e *links_array = (link_e *)malloc(nodes * edges * sizeof(link_e));

    // Initialize array of Router
    populate_network(nodes, edges, &graph, routers_array, links_array, routers);

    /* Add setup data to out_data */
    out_data->total_nodes = nodes;
    out_data->total_links = (int)igraph_ecount(&graph);

    run_simulation_loop(&graph, routers, traffic, routers_array, links_array, setup, out_data);

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
void populate_network(int nodes, int edges_per_node, igraph_t *graph, router *routers, link_e *links_array, routerType *routers_types)
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
        routers[i].sleeping = 0;
        routers[i].inactivity_counter = 0;
        igraph_vector_init(&routers[i].att_links, 0);
    }

    igraph_vector_init(&edges, 0);
    igraph_get_edgelist(graph, &edges, false);

    /* Populate links array */
    links_array = (link_e *)realloc(links_array, igraph_ecount(graph) * sizeof(struct link_e));

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
void run_simulation_loop(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, router *routers_array, link_e *links_array, const sim_setup *setup, simulationData *out_data)
{
    // Initialize variables

    event *events;

    // Initialise router utilisation array

    events = (event *)malloc(sizeof(event) * setup->event_count);

    // Create random events
    create_events(graph, traffic, setup, events);

    out_data->total_amount_of_data = cal_total_data(events, setup->event_count);

    /* Run simulation. Runs the simulation 3 times. i = 0 is for static, i = 1 is for dynamic, i = 2 is for dynamic with sleep */
    for (int i = 0; i < 3; i++)
    {
        /* Copy events, routers and links to temporary variables in function */
        event *events_temp = (event *)malloc(sizeof(event) * setup->event_count);
        router *routers_temp = (router *)malloc(out_data->total_nodes * sizeof(struct router));
        link_e *links_temp = (link_e *)malloc(out_data->total_links * sizeof(struct link_e));
        copy_sim_data(graph, events, routers_array, links_array, setup, events_temp, routers_temp, links_temp);

        send_data(graph, routers, traffic, events_temp, routers_temp, links_temp, setup, i, out_data);

        /* Free memory */
        free(events_temp);
        free(routers_temp);
        free(links_temp);
    }

    /* Free memory */
    for (int i = 0; i < setup->event_count; i++)
    {
        igraph_vector_destroy(&events[i].path);
        igraph_vector_destroy(&events[i].path_edges);
    }
    free(events);
}

void create_events(igraph_t *graph, trafficType *traffic, const sim_setup *setup, event *events)
{
    int i;

    for (i = 0; i < setup->event_count; i++)
    {
        events[i].time = (rand() % setup->simulation_time) + 1;
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

double cal_total_data(const event *events, int event_count)
{
    double total_data = 0;
    int i;
    for (i = 0; i < event_count; i++)
    {
        total_data += events[i].data / 1024 / 1024;
    }

    return total_data;
}

void copy_sim_data(igraph_t *graph, event *events, router *routers, link_e *links, const sim_setup *setup, event *events_temp, router *routers_temp, link_e *links_temp)
{
    int i;
    /* Copy events */
    for (i = 0; i < setup->event_count; i++)
    {
        events_temp[i].time = events[i].time;
        events_temp[i].type = events[i].type;
        events_temp[i].is_active = events[i].is_active;
        events_temp[i].source_id = events[i].source_id;
        events_temp[i].destination_id = events[i].destination_id;
        events_temp[i].data = events[i].data;
        events_temp[i].bandwidth = events[i].bandwidth;
        events_temp[i].available_bandwidth = events[i].available_bandwidth;
        igraph_vector_init(&events_temp[i].path, 0);
        igraph_vector_init(&events_temp[i].path_edges, 0);
        events_temp[i].latency = events[i].latency;
    }

    /* Copy routers */
    for (i = 0; i < igraph_vcount(graph); i++)
    {
        routers_temp[i].type = routers[i].type;
        routers_temp[i].utilisation = routers[i].utilisation;
        routers_temp[i].sleeping = routers[i].sleeping;
        routers_temp[i].inactivity_counter = routers[i].inactivity_counter;
        igraph_vector_copy(&routers_temp[i].att_links, &routers[i].att_links);
    }

    /* Copy links */
    for (i = 0; i < igraph_ecount(graph); i++)
    {
        links_temp[i].max_bandwidth = links[i].max_bandwidth;
        links_temp[i].remaining_bandwidth = links[i].remaining_bandwidth;
        links_temp[i].utilisation = links[i].utilisation;
        igraph_vector_init(&links_temp[i].events, 0);
    }
}

/**
 * Description: Connection between nodes and edges
 * Inputs:
 * Output:
 */
void establish_connections(igraph_t *graph, struct routerType *routers, struct trafficType *traffic, link_e *links_array, igraph_vector_t *edges, igraph_vector_t *vertices, int from, int to, bool *first_run, int state)
{
    igraph_vector_t weights; // TODO: Move into establish connections so that it doesn't have to be a parameter.

    igraph_vector_init(&weights, igraph_ecount(graph));

    /* Run only first time if static */
    if (*first_run == true || state != 0)
    {
        *first_run = false;
        cal_link_weights(graph, routers, traffic, links_array, edges, &weights);
    }

    bellman_ford(graph, vertices, edges, from, to, &weights);

    /* Free memory */
    igraph_vector_destroy(&weights);
}

/**
 * Description: Send data between nodes and edges
 * Inputs: Validated data, graph, router properties, event properties, test state, total power consumption
 * Output: total power consumption
 */
void send_data(igraph_t *graph, routerType *routers, trafficType *traffic, event *events, router *router_array, link_e *links_array, const sim_setup *setup, int test_state, simulationData *out_data)
{
    /* Initialize variables */
    double temp_power_consumption = 0;
    double temp_power_MW = 0;
    int ongoing_events = 0;
    int clock = 0; /* In milliseconds */
    double data_transfer = 0;
    double progress = 0;
    bool first_static_run = true;

    // Set utilisation to 0
    for (int i = 0; i < igraph_vcount(graph); i++)
    {
        router_array[i].utilisation = 0;
    }

    /* Run simulation and loop while there are ongoing events */
    while (clock < setup->simulation_time * 1000 || ongoing_events) // TODO: Change && to || because else ongoing events won't count. However for debugging it is practical to not include ongoing events for now.
    {
        /* Check if there are any events */
        for (int i = 0; i < setup->event_count; i++)
        {
            if (events[i].time * 1000 == clock)
            {
                // Establish connections
                establish_connections(graph, routers, traffic, links_array, &events[i].path_edges, &events[i].path, events[i].source_id, events[i].destination_id, &first_static_run, test_state);

                /* Add event to links event list */
                add_event_to_links(i, &events[i].path_edges, links_array);

                /* Awake sleeping routers */
                wake_up_routers(graph, routers, router_array, &events[i].path, &events[i].latency);

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
            for (int i = 0; i < setup->event_count; i++)
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

        /* Calculate power consumption every second */
        if (clock % 1000 == 0)
        {
            cal_power_consumption(igraph_vcount(graph), router_array, routers, &temp_power_consumption);

            /* Check if power consumption is greater than 1 kW */
            if (temp_power_consumption > 1000)
            {
                temp_power_MW += temp_power_consumption / 1000;
                temp_power_consumption -= 1000;
            }
        }

        /* Check if routers should go to sleep */
        if (test_state == 2 || clock % 1000 == 0)
        {
            check_router_activity(igraph_vcount(graph), router_array, routers, links_array);
        }

        /* Move clock forward */
        clock++;
    }

    /* Add remaining temp power consumption to total power consumption */
    temp_power_MW += temp_power_consumption / 1000000;

    /* Return total power consumption */
    out_data->total_power_consumption[test_state] = temp_power_MW;
    out_data->simulation_time[test_state] = clock / 1000; // In seconds
}

void add_event_to_links(int event_id, igraph_vector_t *path_edges, link_e *links_array)
{
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        igraph_vector_push_back(&links_array[(int)igraph_vector_e(path_edges, i)].events, event_id);
    }
}

void wake_up_routers(igraph_t *graph, struct routerType *routers, router *router_array, igraph_vector_t *path, int *latency)
{
    int longest_latency = 0;
    /* Check if routers in the path are sleeping */
    for (int i = 0; i < igraph_vector_size(path); i++)
    {
        if (router_array[(int)igraph_vector_e(path, i)].sleeping)
        {
            /* Wake up router */
            router_array[(int)igraph_vector_e(path, i)].sleeping = 2;

            /* Set latency if it is greater than the current latency */
            if (routers[router_array[(int)igraph_vector_e(path, i)].type].wakeup_time > longest_latency)
            {
                longest_latency = routers[router_array[(int)igraph_vector_e(path, i)].type].wakeup_time;
            }
        }
        router_array[(int)igraph_vector_e(path, i)].inactivity_counter = 0;
    }

    latency += longest_latency;
}

void bandwidth_balancer(int event_id, igraph_vector_t *path_edges, link_e *links_array, event *event)
{
    double sum_bandwidth;
    double temp_bandwidth;
    igraph_vector_t link_overload;

    igraph_vector_init(&link_overload, 0);

    /* Subtract bandwidth from links */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        links_array[(int)igraph_vector_e(path_edges, i)].remaining_bandwidth -= event[event_id].available_bandwidth;
    }

    /* Check if there are links that are overloaded */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {

        /* Check if there is enough bandwidth */
        if (links_array[(int)igraph_vector_e(path_edges, i)].remaining_bandwidth < 0)
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
                    event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth = new_bandwidth;
                }
            }

            sum_bandwidth = 0;
            /* Calculate remaining bandwidth */
            for (int j = 0; j < igraph_vector_size(&links_array[(int)igraph_vector_e(&link_overload, i)].events); j++)
            {
                sum_bandwidth += event[(int)igraph_vector_e(&links_array[(int)igraph_vector_e(&link_overload, i)].events, j)].available_bandwidth;
            }

            links_array[(int)igraph_vector_e(&link_overload, i)].remaining_bandwidth = links_array[(int)igraph_vector_e(&link_overload, i)].max_bandwidth - sum_bandwidth;

            /* Free memory */
            free(bandwidth_percentage);
        }
    }

    /* Free memory */
    igraph_vector_destroy(&link_overload);
}

void sort_links(link_e *links_array, igraph_vector_t *link_overload)
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

void cal_utilisation(int router_len, int link_len, router *router_array, link_e *links_array, event *event)
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

void remove_event_from_links(int event_id, igraph_vector_t *path_edges, link_e *links_array)
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

void release_bandwidth(int event_id, igraph_vector_t *path_edges, link_e *links_array, event *events)
{
    /* Iterate through links affected by event */
    for (int i = 0; i < igraph_vector_size(path_edges); i++)
    {
        /* Release bandwidth */
        links_array[(int)igraph_vector_e(path_edges, i)].remaining_bandwidth += events[event_id].available_bandwidth;
    }
}

void cal_power_consumption(int router_len, router *router_array, struct routerType *t_routers, double *power_con)
{
    double *temp_power_con = (double *)malloc(sizeof(double) * router_len); /* Temporary power consumption in milliWatts */
    double power_sum = 0;                                                   /* Sum of power consumption in milliwatts */
    double offset;                                                          /* Offset of milliseconds to calculate linearly (Used to not calculate for the power used by a router that is starting up) */

    /* Set temp power consumption to 0 */
    for (int i = 0; i < router_len; i++)
    {
        temp_power_con[i] = 0;
    }

    /* Iterate through all routers */
    for (int i = 0; i < router_len; i++)
    {
        offset = 1;
        switch (router_array[i].sleeping)
        {
        case 2:
            offset = (1000 - t_routers[router_array[i].type].wakeup_time) / 1000;
            temp_power_con[i] = (t_routers[router_array[i].type].power.sleep / 3.6) * (1 - offset);
            router_array[i].sleeping = 0; /* Set sleeping to 0 since the power consumption is calculated */

        case 0:
            temp_power_con[i] += linear_power_con(t_routers[router_array[i].type].power.idle, t_routers[router_array[i].type].power.peak, router_array[i].utilisation, offset);
            break;

        case 1:
            temp_power_con[i] = t_routers[router_array[i].type].power.sleep;
            break;

        default:
            break;
        }
    }

    /* Calculate sum of power consumption */
    for (int i = 0; i < router_len; i++)
    {
        power_sum += temp_power_con[i];
    }

    /* Add sum to power consumption and convert back to Watts */
    *power_con += power_sum / 1000;

    /* Free memory */
    free(temp_power_con);
}

void check_router_activity(int router_len, router *router_array, struct routerType *t_routers, link_e *links_array)
{

    /* Iterate through all routers that are not sleeping */

    for (int i = 0; i < router_len; i++)
    {
        if (router_array[i].sleeping == 0)
        {
            bool has_events = false;
            /* This is actually a stupid way of doing this, but there's no time to do it properly */

            /* Check all links connected to router */
            for (int j = 0; j < igraph_vector_size(&router_array[i].att_links); j++)
            {
                /* Check if link has events */
                if (igraph_vector_size(&links_array[(int)igraph_vector_e(&router_array[i].att_links, j)].events) > 0)
                {
                    has_events = true;
                    break; /* No need to check the rest of the links */
                }
            }

            /* If no events are found */
            if (!has_events)
            {
                /* Set inactivity counter 1 up */
                router_array[i].inactivity_counter++;

                /* If inactivity counter is equal to the inactivity time */
                if (router_array[i].inactivity_counter == 300)
                {
                    /* Set sleeping to 1 */
                    router_array[i].sleeping = 1;
                    router_array[i].inactivity_counter = 0;
                    router_array[i].utilisation = 0;
                }
            }
        }
    }
}