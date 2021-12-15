#include <igraph/igraph.h>
#include <stdbool.h>

typedef struct router
{
    int type;
    double utilisation;
    bool sleeping;
    igraph_vector_t events;
} router;

typedef struct link
{
    int max_bandwidth;
    int remaining_bandwidth;
    double utilisation;
} link;

typedef struct event
{
    int type;
    int time; //Time of when it happens in seconds.
    int latency; //Latency of the event in seconds.
    double data; // How much data should be transfered in MB.
    int bandwidth; //Bandwidth of the event in Mbps.
    int available_bandwidth; //Available bandwidth of the event in Mbps.
    int source_id;
    int destination_id;
    igraph_vector_t path;
    igraph_vector_t path_edges;

} event;

