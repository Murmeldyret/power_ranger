#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_LINE_LEN 100

#define ROUTER_PATH "config/routerType.csv"
#define TRAFFIC_PATH "config/trafficType.csv"

FILE *fp;

/**
 * Description: Reads .csv files and outputs to data structures
 * Inputs: return parameters - pointer to the data structure
 * Output: struct *routertypearray, *traffictypearray
 */
bool initialise_data(routerType *routertypearray, trafficType *traffictypearray)
{
    bool routerType;
    bool trafficType;

    if(routerType = readRouterType(routertypearray) == false)
    {
        printf("Error in readRouterType \n");
    }

    if(trafficType = readTrafficType(traffictypearray) == false)
    {
        printf("Error in readTrafficType \n");
    }
    
    return true;
}

/**
 * Description: Read router type from routertype.csv
 * Inputs: routerType.csv
 * Output: routerType array of router types
 */
bool readRouterType(routerType *routertypesarr) 
{
    // TODO variabler herop
    // TODO ryd op i variabler som ikke bliver brugt.
    char buffer[CSV_LINE_LEN]; 
    char *token;
    
    int i = 0;
    int j = 1;

    if((fp = fopen(ROUTER_PATH, "r")) == NULL)
    {
        printf("Error in opening routerType.csv \n");
        return 0;
    }

    // loops until eof is reached
    while (feof(fp) == false)
    {
        // Outputs current line to buffer char array
        fgets(buffer, CSV_LINE_LEN, fp);
        j = 1;

        // Splits buffer into smaller parts delimited by ","
        token = strtok(buffer, ",");
        printf("Token: %s\n", token);
        do
        {     
            //Switches on which struct elements should get token value
            switch (j)
            {
            case 1:
                routertypesarr[i].id = atoi(token);
                printf("Token: %s\n", token);
                break;
            case 2:
                strcpy(routertypesarr[i].type,token);
                break;
            case 3:
                routertypesarr[i].bandwidth = atoi(token);
                break;
            case 4:
                routertypesarr[i].wakeup_time = atoi(token);
                break;
            case 5:
                routertypesarr[i].latency = atoi(token);
                break;
            case 6:
                routertypesarr[i].power.idle = atoi(token);
                break;
            case 7:
                routertypesarr[i].power.peak = atoi(token);
                break;
            case 8:
                routertypesarr[i].power.sleep = atoi(token);
                break;
            case 9:
                routertypesarr[i].packet_memory = atoi(token);
                break;
            default:
                printf("Error in readRouterType Switch: Expected number from 1-9 but got %d\n", j);
                return false;
                break;
            }
            token = strtok(NULL,",");
            j++;
        }  while (token != NULL && j <= 9);
        i++;
    }

    // Close file, no longer needed
    fclose(fp);

    return true;
}

//Prints all elements in routertype routertype
void printRouterTypeElements(routerType routerType) 
{
    printf("\n%d\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
                            routerType.id,
                            routerType.type,
                            routerType.bandwidth,
                            routerType.wakeup_time,
                            routerType.latency,
                            routerType.power.idle,
                            routerType.power.peak,
                            routerType.power.sleep,
                            routerType.packet_memory);
    return;
}

/**
 * Description: Reads trafficType.csv and outputs to trafficType struct
 * Inputs: traffictypearray
 * Output: trafficType *traffictypearray
 */
bool readTrafficType(trafficType *traffictypearr)
{
    char buffer[CSV_LINE_LEN];
    char *token;
    int i = 0;
    int j;

    // Opens trafficType.csv file
    if((fp = fopen(TRAFFIC_PATH, "r")) == NULL)
    {
        // Checks that the file has been opened with success
        printf("Error in opening trafficType.csv \n");
        return false;
    }

    // loops until eof is reached
    while (feof(fp) == false)
    {
        fgets(buffer, CSV_LINE_LEN, fp);
        j = 1;
        // Splits buffer into smaller parts delimited by ","
        token = strtok(buffer, ",");
        do
        {
            //printf("token: %s\n",token);
            //Switches on which struct element should get token value
            switch (j)
            {
            case 1:
                traffictypearr[i].id = atoi(token);
                break;
            case 2:
                strcpy(traffictypearr[i].type, token);
                break;
            case 3:
                traffictypearr[i].latency_sensitivity = atoi(token);
                break;
            case 4:
                traffictypearr[i].data_size = atoi(token);
                break;
            case 5:
                traffictypearr[i].speed = atoi(token);
                break;
            case 6:
                //traffictypearr[i].packetloss_sensitivity = atoi(token);
                strcpy(traffictypearr[i].packetloss_sensitivity,token);
                break;
            default:
                printf("Error in readTrafficType Switch: Expected number from 1-6 but got %d\n",j);
                return false;
                break;
            }
            j++;
            token = strtok(NULL,",");


        } while (token != NULL && j <=6);
        
    i++;
    }

    fclose(fp);
    return true;
}

// prints all elements of a single trafficType
void printTrafficTypeElements(trafficType trafficType)
{
    printf("\n%d\n%s\n%d\n%d\n%d\n%s\n",
                            trafficType.id,
                            trafficType.type,
                            trafficType.latency_sensitivity,
                            trafficType.data_size,
                            trafficType.speed,
                            trafficType.packetloss_sensitivity);
    return;
}