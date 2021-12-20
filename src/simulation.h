#include "algorithms.h"

typedef struct simulationData
{
    double total_power_consumption[3]; //Power consumed by network
    double simulation_time[3];         //Time taken to complete simulation in seconds
    int total_nodes;                   //Total number of nodes in network
    int total_links;                   //Total number of links in network
    double total_amount_of_data;       //Total amount of data in network (in GB)
} simulationData;

void run_simulation(struct routerType *, struct trafficType *, struct simulationData *, const sim_setup *);
void populate_network(int, int, igraph_t *, router *, link_e *, routerType *);
void create_events(igraph_t *, trafficType *, const sim_setup *, event *);
double cal_total_data(const event *, int);
void copy_sim_data(igraph_t *, event *, router *, link_e *, const sim_setup *, event *, router *, link_e *);
void run_simulation_loop(igraph_t *, struct routerType *, struct trafficType *, router *, link_e *, const sim_setup *, simulationData *);
void establish_connections(igraph_t *, struct routerType *, struct trafficType *, link_e *, igraph_vector_t *, igraph_vector_t *, int, int, bool *, int);
void send_data(igraph_t *, routerType *, trafficType *, event *, router *, link_e *, const sim_setup *, int, simulationData *);
void add_event_to_links(int, igraph_vector_t *, link_e *);
void wake_up_routers(igraph_t *, struct routerType *, router *, igraph_vector_t *, int *);
void bandwidth_balancer(int, igraph_vector_t *, link_e *, event *);
void sort_links(link_e *, igraph_vector_t *);
void cal_utilisation(int, int, router *, link_e *, event *);
void remove_event_from_links(int, igraph_vector_t *, link_e *);
void release_bandwidth(int, igraph_vector_t *, link_e *, event *);
void cal_power_consumption(int, router *, struct routerType *, double *);