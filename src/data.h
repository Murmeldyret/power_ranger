#include <stdbool.h>

/* Function prototypes */
bool initialise_data();
void readRouterType();
void readTrafficType();

/* Structures */
struct routerType
{
    int id;
    char *type;
    int bandwidth;
    int wakeup_time;
    int latency;
    struct powerConsumption *power;
    int packet_memory;
};

struct powerConsumption
{
    int idle;
    int peak;
    int sleep;
};

struct trafficType 
{
    int id;
    char *type;
    int latency_sensitivity;
    int data_size;
    int packetloss_sensitivity;
};

