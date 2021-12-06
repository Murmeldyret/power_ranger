#include <stdbool.h>

#define ROUTER_TYPE_TYPE_LEN 6 // type length +2
/* Structures */
typedef struct routerType
{
    // .csv elements appear in same order
    int id;
    int type; // 1 = fafb
    int bandwidth; //Mbps
    int wakeup_time; //ms
    int latency; //ms
    struct power
    {
        int idle;
        int peak;
        int sleep;
    } power; //in watt
    int packet_memory; //in bytes
} routerType;

/*struct powerConsumption
{
    int idle;
    int peak;
    int sleep;
}; */

struct trafficType 
{
    int id;
    char *type;
    int latency_sensitivity;
    int data_size;
    int packetloss_sensitivity;
};
/* Function prototypes */
bool initialise_data();
bool readRouterType();
bool readTrafficType();
void printRouterTypeElements(routerType routerType);

