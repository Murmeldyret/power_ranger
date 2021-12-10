#include <igraph/igraph.h>
#include "algorithms.h"

void run_simulation(struct routerType *, struct trafficType *);
void populate_network(int, int, igraph_t *);
void run_simulation_loop(igraph_t *, struct routerType *, struct trafficType *);
void establish_connections(igraph_t *, struct routerType *, struct trafficType *);
void send_data();

typedef struct chain
{
    int data;
    struct chain *next;
} chain;

typedef struct simulationData
{
    int total_power_consumption; //Power consumed by network
    chain current_path, *path_ptr;          // * router needs individual id!
    int total_links;             //Total number of links in network
    int total_amount_of_data;
} simulationData;

