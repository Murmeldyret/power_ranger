
#include <stdio.h>
#include <stdlib.h>
#include "output.h"
#include "simulation.h"
/**
 * Function: display_data
 * Description: Display data
 * Inputs: struct simulationData
 * Output: None
 */


void display_data()
{
    //! example simulation data only. Should not be present in final product
    simulationData data;
    data.total_power_consumption = 1337;
    data.total_links = 2;
    data.total_amount_of_data = 123456789;
    FILE *fp = fopen("outputs/Output.csv", "w");

    fprintf(fp, "This really be a bruh moment\n");
    fprintf(fp, "Total power consumption: \n%d\n\n", data.total_power_consumption);
    fprintf(fp, "Amount of links: \n%d\n\n", data.total_links);
    fprintf(fp, "Total amount of data: \n%d\n\n", data.total_amount_of_data);


}