
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
    /* Initialize variables */
    simulationData results;
    results = *data;
    results.path_ptr = &results.current_path;

    FILE *fp = fopen("outputs/Output.csv", "w");

    fprintf(fp, "This really be a bruh moment\n");
    fprintf(fp, "Total power consumption: \n%d Watt(s)\n\n", results.total_power_consumption);
    fprintf(fp, "Amount of links: \n%d\n\n", results.total_links);
    fprintf(fp, "Total amount of data: \n%d Mb\n\n", results.total_amount_of_data);

    fprintf(fp, "Path followed:\n");

    // while loop discontinued until function returns actual data, as it is the cause of a segmentation fault
   /* while (results.path_ptr.n != NULL)
    {
    }*/
    fprintf(fp, "%d", results.path_ptr->data);
    if (results.path_ptr->next != NULL)
    {
        fprintf(fp, " -> ");
    }
    results.path_ptr = results.path_ptr->next;

    fprintf(fp, "\n");

    fclose(fp);
}