#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "output.h"


void recieve_setup(sim_setup *);

/*
 * main - Entry point
 */
int main(void)
{
    srand(time(NULL));

    routerType routertypes[NMBR_OF_ROUTERTYPES];
    trafficType traffictypes[NMBR_OF_TRAFFICTYPES];
    simulationData data;
    sim_setup setup;
    printf("🗿ඞ\n");

    recieve_setup(&setup);

    initialise_data(routertypes, traffictypes);
    //prints structs of relevant type
    //printRouterTypeElements(routertypes[0]);
    //printTrafficTypeElements(traffictypes[0]);

    run_simulation(routertypes, traffictypes, &data, &setup);

    display_data(&data);

    return EXIT_SUCCESS; /* Successful termination */
}

void recieve_setup(sim_setup *setup)
{
    printf("-------------------------------------------------------------------------------\n");
    printf("Enter the number of routers: ");
    scanf("%d", &setup->nodes);
    printf("-------------------------------------------------------------------------------\n");
    printf("Enter the number of links per node: ");
    scanf("%d", &setup->edges_per_node);
    printf("-------------------------------------------------------------------------------\n");
    printf("NOTICE: The simulation will continue to run if there is still data to be sent.\n");
    printf("Enter the amount of time to simulate in seconds: ");
    scanf("%d", &setup->simulation_time);
    printf("-------------------------------------------------------------------------------\n");
    printf("Enter the number of events to be simulated: ");
    scanf("%d", &setup->event_count);
    printf("-------------------------------------------------------------------------------\n");
}