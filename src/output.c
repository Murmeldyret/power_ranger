
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "output.h"

/**
 * Function: display_data
 * Description: Display data
 * Inputs: struct simulationData
 * Output: None
 */

void display_data(simulationData *data)
{
    FILE *fp = fopen("outputs/Output.csv", "w");

    /* String of simulation state names */
    char *simulationStateNames[] = {
        "Static",
        "Dynamic",
        "Dynamic with sleep states",
    };

    fprintf(fp, "Network simulation\n");
    fprintf(fp, "Simulation setup:\n");
    fprintf(fp, "Number of routers:,%d\n", data->total_nodes);
    fprintf(fp, "Number of links:,%d\n", data->total_links);
    fprintf(fp, "Total amount of data:,%lf,GB\n\n", data->total_amount_of_data / 1024 / 1024);

    /* Print simulation state and power consumption */
    fprintf(fp, "Simulation results:\n");
    for (int i = 0; i < 3; i++)
    {
        fprintf(fp, "%s:,%lf,MWh\n", simulationStateNames[i], data->total_power_consumption[i]);
    }

    fprintf(fp, "\n");

    fclose(fp);
}