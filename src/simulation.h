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

typedef struct event
{
    int type;
    int time; //Time of when it happens in seconds.
    double data; // How much data should be transfered in MB.
    int source_id;
    int destination_id;

} event;


void run_simulation(struct routerType *, struct trafficType *);
void populate_network(int, int, igraph_t *);
void create_events(igraph_t *, trafficType *, event *);
void run_simulation_loop(igraph_t *, struct routerType *, struct trafficType *);
void establish_connections(igraph_t *, struct routerType *, struct trafficType *, double *, igraph_vector_t *, igraph_vector_t *, int, int);
void send_data(igraph_t *, struct routerType *, struct trafficType *, event *);

