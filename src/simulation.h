#include "algorithms.h"

typedef struct chain
{
    int data;
    struct chain *next;
} chain;

typedef struct simulationData
{
    int total_power_consumption[3]; //Power consumed by network
    chain current_path;             // * router needs individual id!
    int total_nodes;                //Total number of nodes in network
    int total_links;                //Total number of links in network
    double total_amount_of_data;    //Total amount of data in network (in GB)
} simulationData;

void run_simulation(struct routerType *, struct trafficType *, struct simulationData *);
void populate_network(int, int, igraph_t *, router *, link *, routerType *);
void create_events(igraph_t *, trafficType *, event *);
double cal_total_data(const event *, int);
void run_simulation_loop(igraph_t *, struct routerType *, struct trafficType *, router *, link *, simulationData *);
void establish_connections(igraph_t *, struct routerType *, struct trafficType *, link *, igraph_vector_t *, igraph_vector_t *, int, int);
void send_data(igraph_t *, struct routerType *, struct trafficType *, event *, router *, link *, simulationData *);
void add_event_to_links(int, igraph_vector_t *, link *);
void bandwidth_balancer(int, igraph_vector_t *, link *, event *);
void sort_links(link *, igraph_vector_t *);
void cal_utilisation(int, int, router *, link *, event *);
void remove_event_from_links(int, igraph_vector_t *, link *);
void release_bandwidth(int, igraph_vector_t *, link *, event *);
void cal_power_consumption(int, router *, struct routerType *, double *);