#include <igraph/igraph.h>

typedef struct router
{
    int type;
    double utilisation;
} router;

typedef struct event
{
    int type;
    int time; //Time of when it happens in seconds.
    double data; // How much data should be transfered in MB.
    int source_id;
    int destination_id;
    igraph_vector_t path;

} event;