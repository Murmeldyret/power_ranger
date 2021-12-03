//#include <json-c/json.h> // TODO: Include this header file
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Description: Initializes the data structure
 * Inputs: return parameters - pointer to the data structure
 * Output: struct routertype[], traffictype[]
 */
bool initialise_data()
{
    readRouterType();
    // readTrafficType();

    return true;
}

/**
 * Description: Read router type from routertype.csv
 * Inputs: 
 * Output: 
 */
void readRouterType()
{
    FILE *fp = fopen("config/routerType.csv", "r");

    if(!fp)
    {
        printf("Problem reading routertype.csv \n");
        exit(0);
    }

    int routerId;
    char type[512];

    char line[512];

    char *sp;

    while(fgets(line, 5, fp))
    {
        // Reading the line and stops at the comma delimter.
        sp = strtok(line, ",");
        
        // Converts the string into an integer. 
        routerId = atoi(sp);

        // Picks up where it left over at the comman and continue on.
        sp = strtok(NULL, ",");
        strcpy(type, sp);

        printf("\n%d %s", routerId, type);
    }

    fclose(fp);
}

void readTrafficType()
{
}