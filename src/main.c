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


    initialise_data(routers, traffic); //TODO: Make this function
    
    run_simulation(routers, traffic);  //TODO: Make this function

    display_data();    //TODO: Make this function

    // Test

    
    return 0;
}