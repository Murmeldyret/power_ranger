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
