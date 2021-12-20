#include <igraph/igraph.h>
#include <stdbool.h>

typedef struct router
{
    int type;
    double utilisation;
    int sleeping;              // 0 = awake, 1 = sleeping, 2 = waking up
    int inactivity_counter;    // counts how many times the router has been inactive
    igraph_vector_t att_links; /* Attached links */
} router;

typedef struct link_e
{
    double max_bandwidth;
    double remaining_bandwidth;
    double utilisation;
    igraph_vector_t events; // Indexes of events
} link_e;

typedef struct event
{
    int type;
    bool is_active;
    int time;                   //Time of when it happens in seconds.
    int latency;                //Latency of the event in seconds.
    double data;                // How much data should be transfered in KB.
    double bandwidth;           //Bandwidth of the event in Kbps.
    double available_bandwidth; //Available bandwidth of the event in Kbps.
    int source_id;
    int destination_id;
    igraph_vector_t path;       // Indexes of routers in the path.
    igraph_vector_t path_edges; // Indexes of links in the path.

} event;
