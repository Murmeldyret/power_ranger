#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "output.h"

/*
 * main - Entry point
 */
int main(void)
{
    printf("🗿ඞ\n");  //! DO NOT REMOVE!!!
    
    /* Initialize variables */
    struct routerType *routers;
    struct trafficType *traffic;


    initialise_data(routers, traffic); //TODO: Make this function
    
    run_simulation(routers, traffic);  //TODO: Make this function

    display_data();    //TODO: Make this function

    // Test

    
    return 0;
}