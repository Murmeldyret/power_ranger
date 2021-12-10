
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
    chain *temp;
    data->path_ptr = data->current_path;

    FILE *fp = fopen("outputs/Output.csv", "w");

    if (data != NULL)
    {
        printf("initializing data\n");
        temp = data->path_ptr;
    }
    else
    {
        printf("Unable to get data\n");
        return;
    }

    fprintf(fp, "This really be a bruh moment\n");
    fprintf(fp, "Total power consumption: \n%d Watt(s)\n\n", data->total_power_consumption);
    fprintf(fp, "Amount of links: \n%d\n\n", data->total_links);
    fprintf(fp, "Total amount of data: \n%d Mb\n\n", data->total_amount_of_data);

    fprintf(fp, "Path followed:\n");
    while (temp)
    {
        fprintf(fp, "%d", temp->data);
        if (temp->next != NULL)
        {
            fprintf(fp, " -> ");
        }
        temp = temp->next;
    }

    fprintf(fp, "\n");
}