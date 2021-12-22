
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
    printf("-------------------------------------------------------------------------------\n");
    printf("Saving data...\n");
    FILE *fp = fopen("outputs/Output.csv", "w");

    /* String of simulation state names */
    char *simulationStateNames[] = {
        "Static",
        "Dynamic",
        "Dynamic with sleep states",
    };

    /* String of simulationData types */
    char *simulationDataTypes[] = {
        "Power Consumption",
        "Time (s)",
    };

    fprintf(fp, "Network simulation\n");
    fprintf(fp, "Simulation setup:\n");
    fprintf(fp, "Number of routers:,%d\n", data->total_nodes);
    fprintf(fp, "Number of links:,%d\n", data->total_links);
    fprintf(fp, "Number of events:,%d\n", data->total_events);
    fprintf(fp, "Total amount of data:,%lf,GB\n\n", data->total_amount_of_data);

    /* Print simulation state and power consumption */
    fprintf(fp, "Simulation results:,%s,%s,%s\n", simulationStateNames[0], simulationStateNames[1], simulationStateNames[2]);
    fprintf(fp, "%s,%lf kW,%lf kW,%lf kW\n", simulationDataTypes[0], data->total_power_consumption[0], data->total_power_consumption[1], data->total_power_consumption[2]);
    fprintf(fp, "%s,%lf s,%lf s,%lf s\n", simulationDataTypes[1], data->simulation_time[0], data->simulation_time[1], data->simulation_time[2]);

    fprintf(fp, "\n");

    fclose(fp);

    printf("Data saved to outputs/Output.csv\n");
    printf("-------------------------------------------------------------------------------\n");
}