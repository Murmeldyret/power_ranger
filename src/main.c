#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "output.h"

/*
 * main - Entry point
 */
int main(void)
{
    routerType routertypes[NMBR_OF_ROUTERTYPES];
    trafficType traffictypes[NMBR_OF_TRAFFICTYPES];
    printf("🗿ඞ\n");
    
    initialise_data(routertypes, traffictypes); //TODO: Make this function
    //prints structs of relevant type
    //printRouterTypeElements(routertypes[0]);
    //printTrafficTypeElements(traffictypes[1]);

    
    run_simulation(routertypes, traffictypes);  //TODO: Make this function

    
    simulationData data;
    display_data(data);    //TODO: Make this function

    // Test

    
    return 0;
}