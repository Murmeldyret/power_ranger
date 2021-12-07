#include <stdbool.h>


#define NMBR_OF_ROUTERTYPES 1 //number of routertypes from routerType.csv
#define NMBR_OF_TRAFFICTYPES 2 // number of routertypes from trafficTypes.csv
#define ROUTER_TYPE_TYPE_LEN 6 // type length +2
// Types, prototypes and functions (in order :))

/* Structures */
typedef struct routerType
{
    // .csv elements appear in same order
    int id;
    char type[16]; // 1 = fafb
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

typedef struct trafficType 
{
    int id;
    char type[16];
    int latency_sensitivity;
    int data_size;
    char packetloss_sensitivity[16];
} trafficType;
/* Function prototypes */
bool initialise_data(routerType *routertypearray, trafficType *traffictypearray); //output parameters: routertypearray
bool readRouterType(routerType *routertypesarr);
bool readTrafficType(trafficType *traffictypearr);
void printRouterTypeElements(routerType routerType);
void printTrafficTypeElements(trafficType trafficType);
void printTrafficTypeElements(trafficType traffictype);