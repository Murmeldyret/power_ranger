#include "algorithms.h"

typedef struct chain
{
    int data;
    struct chain *next;
} chain;

typedef struct simulationData
{
    int total_power_consumption; //Power consumed by network
    chain current_path;          // * router needs individual id!
    int total_links;             //Total number of links in network
    int total_amount_of_data;
} simulationData;

void run_simulation(struct routerType *, struct trafficType *);
void populate_network(int, int, igraph_t *, router *, link *, routerType *);
void create_events(igraph_t *, trafficType *, event *);
void run_simulation_loop(igraph_t *, struct routerType *, struct trafficType *, router *, link *);
void establish_connections(igraph_t *, struct routerType *, struct trafficType *, link *, igraph_vector_t *, igraph_vector_t *, int, int);
void send_data(igraph_t *, struct routerType *, struct trafficType *, event *, router *, link *);
void add_event_to_links(int, igraph_vector_t *, link *);
void bandwidth_balancer(igraph_vector_t *, link *, event *);
void sort_links(link *, igraph_vector_t *);