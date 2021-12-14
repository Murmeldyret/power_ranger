#include <igraph/igraph.h>
#include <stdbool.h>

typedef struct router
{
    int type;
    double utilisation;
    bool sleeping;
} router;

typedef struct event
{
    int type;
    int time; //Time of when it happens in seconds.
    int latency; //Latency of the event in seconds.
    double data; // How much data should be transfered in MB.
    int source_id;
    int destination_id;
    igraph_vector_t path;

} event;