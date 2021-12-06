
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_ROUTERTYPE_MAX_LEN 100
#define CSV_ROUTERTYPE_NMBR_OF_PROPERTIES 9
#define CSV_ROUTERTYPE_CHAR_LEN 10 //Max length of an element in routerType.csv excluding first line
#define NMBR_OF_ROUTERTPES 1
/**
 * Description: Initializes the data structure
 * Inputs: return parameters - pointer to the data structure
 * Output: struct routertype[], traffictype[]
 */
bool initialise_data()
{
    readRouterType();
    if (readRouterType == false)
    {
        printf("Error in readRouterType \n");
        return false;
    }
    
    // readTrafficType();

    return true;
}

/**
 * Description: Read router type from routertype.csv
 * Inputs: 
 * Output: 
 */
bool readRouterType()
{
    FILE *frtp = fopen("config/routerType.csv", "r");
    /* RouterType.csv values are ordered in (seperated by commas) :   id(int), 
                                                type(int), 
                                                bandwidth(int), 
                                                wakeupTime(int), 
                                                latency(int), 
                                                powerIdle(int), 
                                                powerPeak(int), 
                                                powerSleep(int), 
                                                packetMemory(int)
    id,type,bandwidth,wakeupTime,latency,powerIdle,powerPeak,powerSleep,packetMemory;
    */
    if(!frtp)
    {
        printf("Problem reading routerType.csv \n");
        return false;
    }

    // TODO ryd op i variabler som ikke bliver brugt.
    
    
    char buffer[100]; 
    // TODO routerTypes array skal i main 
    routerType routerTypes[NMBR_OF_ROUTERTPES];
    char *token;
    int i = 0;
    int j = 1;
    
    // loops till eof is reached
    while (feof(frtp) != true)
    {
        // Outputs current line to buffer char array
        fgets(buffer,100,frtp);

        // Splits buffer into smaller parts delimited by ","
        token = strtok(buffer,",");
        do
        {     
            //printf("%s\n",token);
            //Switches on which struct should get token value
            switch (j)
            {
            case 1:
                routerTypes[i].id = atoi(token);
                break;
            case 2:
                routerTypes[i].type = atoi(token);
                break;
            case 3:
                routerTypes[i].bandwidth = atoi(token);
                break;
            case 4:
                routerTypes[i].wakeup_time = atoi(token);
                break;
            case 5:
                routerTypes[i].latency = atoi(token);
                break;
            case 6:
                routerTypes[i].power.idle = atoi(token);
                break;
            case 7:
                routerTypes[i].power.peak = atoi(token);
                break;
            case 8:
                routerTypes[i].power.sleep = atoi(token);
                break;
            case 9:
                routerTypes[i].packet_memory = atoi(token);
                break;
            default:
                printf("Expected number from 1-9 but got %d\n", j);
                return false;
                break;
            }
            token = strtok(NULL,",");
           j++;
        }  while (token != NULL);
        //printf("switch slut\n\n\n\n");
        i++;
    }
    
    printRouterTypeElements(routerTypes[0]);
    fclose(frtp);
    return true;
    }

//Prints all elements in struct routertype
void printRouterTypeElements(routerType routerType) 
{
    printf("\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
                            routerType.id,
                            routerType.type,
                            routerType.bandwidth,
                            routerType.wakeup_time,
                            routerType.latency,
                            routerType.power.idle,
                            routerType.power.peak,
                            routerType.power.sleep,
                            routerType.packet_memory);
}

bool readTrafficType()
{
}