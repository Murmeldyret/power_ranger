#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "output.h"

/*
 * main - Entry point
 */
int main(void)
{
    srand(time(NULL));

    routerType routertypes[NMBR_OF_ROUTERTYPES];
    trafficType traffictypes[NMBR_OF_TRAFFICTYPES];
    simulationData data;
    printf("🗿ඞ\n");

    initialise_data(routertypes, traffictypes);
    //prints structs of relevant type
    //printRouterTypeElements(routertypes[0]);
    //printTrafficTypeElements(traffictypes[0]);

    run_simulation(routertypes, traffictypes, &data);

    display_data(&data);

    return EXIT_SUCCESS; /* Successful termination */
}