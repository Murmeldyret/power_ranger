#include <igraph/igraph.h>


void run_simulation();
void populate_network(int, int);
void run_simulation_loop();
void establish_connections();
void send_data();

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
