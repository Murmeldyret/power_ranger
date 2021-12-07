
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
    chain *path1, *path2, *path3, *path4, *temp;

    path1 = (chain *)malloc(sizeof(chain));
    path2 = (chain *)malloc(sizeof(chain));
    path3 = (chain *)malloc(sizeof(chain));
    path4 = (chain *)malloc(sizeof(chain));

    path1->data = 0;
    path2->data = 1;
    path3->data = 2;
    path4->data = 3;
    path1->next = path2;
    path2->next = path3;
    path3->next = path4;


    FILE *fp = fopen("outputs/Output.csv", "w");

    fprintf(fp, "This really be a bruh moment\n");
    fprintf(fp, "Total power consumption: \n%d\n\n", data.total_power_consumption);
    fprintf(fp, "Amount of links: \n%d\n\n", data.total_links);
    fprintf(fp, "Total amount of data: \n%d\n\n", data.total_amount_of_data);

    temp = path1;
    fprintf(fp, "Path followed:\n");
    do
    {
        fprintf(fp, "%d", temp->data);
        if (temp->next != NULL)
        {
            fprintf(fp, " -> ");
        }
        temp = temp->next;
    } while (temp != NULL);
    fprintf(fp, "\n");
}