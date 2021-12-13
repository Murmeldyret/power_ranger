#include <stdbool.h>


#define NMBR_OF_ROUTERTYPES 3 //number of routertypes from routerType.csv
#define NMBR_OF_TRAFFICTYPES 4 // number of routertypes from trafficTypes.csv
#define CSV_MAX_CHAR_LEN 16 // MAX length of struct element strings
// Types, prototypes and functions (in order :))

/* Structures */
typedef struct routerType
{
    // .csv elements appear in same order
    int id;
    char type[CSV_MAX_CHAR_LEN]; 
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

//Diferent types of traffic.
typedef struct trafficType 
{
    int id; //int
    char type[CSV_MAX_CHAR_LEN];
    int latency_sensitivity; //ms
    int data_size; //MB
    int speed;
    char packetloss_sensitivity[CSV_MAX_CHAR_LEN]; 
} trafficType;
/* Function prototypes */
bool initialise_data(routerType *routertypearray, trafficType *traffictypearray); //output parameters: routertypearray
bool readRouterType(routerType *routertypesarr);
bool readTrafficType(trafficType *traffictypearr);
void printRouterTypeElements(routerType routerType);
void printTrafficTypeElements(trafficType trafficType);
void printTrafficTypeElements(trafficType traffictype);