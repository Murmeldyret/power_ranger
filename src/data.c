
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
                                                type(string), 
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
    char type[5];
    char line[512];
    char buffer[100];
    char linebuffer[100]; 
    char *sp;
    char router_type_temp[9][CSV_ROUTERTYPE_CHAR_LEN];
    //const char delimiters[] = "; ,";
    // TODO routerTypes array skal i main 
    routerType routerTypes[NMBR_OF_ROUTERTPES];
    int fscanned_elements; 

    // Scans one line from routerType.csv
    // TODO få fscanned_elements til at give 9 ikke 2 (muligvis problem med .type)
    fscanned_elements = fscanf(frtp, "%d,%s,%d,%d,%d,%d,%d,%d,%d;",
                            &routerTypes[0].id,
                            &routerTypes[0].type,
                            &routerTypes[0].bandwidth,
                            &routerTypes[0].wakeup_time,
                            &routerTypes[0].latency,
                            &routerTypes[0].power->idle,
                            &routerTypes[0].power->peak,
                            &routerTypes[0].power->sleep,
                            &routerTypes[0].packet_memory);
   //segmentation fault
   //printf("%s",routerTypes[0].type);
    if (fscanned_elements != CSV_ROUTERTYPE_NMBR_OF_PROPERTIES)
    {
        printf("Expected 9 elements read, only %d read with success\n",fscanned_elements);
        return false;
    }
    printRouterTypeElements(routerTypes[0]);
    /*while(fgets(line, 512, frtp))
    {
        int i = 0;
        for (i = 0; i < CSV_ROUTERTYPE_NMBR_OF_PROPERTIES; i++)
        {
        sp = strtok(line, delimiters);
        }
        
        
        sp = strtok(NULL, delimiters);
        printf("%s",sp);
        
        i = 0;
        do
        {
            printf("%d\n",i);
            //copies routerType properties to string array
            sp = strtok(NULL,delimiters);
            printf("%s test\n",sp);
            strcpy(router_type_temp[i], sp = strtok(NULL,delimiters));
            i++;
            printf("%d\n",i);
        } while (strtok(NULL, delimiters)!=NULL);
        */

        
        /*
        // Reads entire line and stops at semicolon delimiter (end of line)
        linebuffer[100] = strtok(line, ";");
        strtok(linebuffer,",");
        
        // Reading the line and stops at the comma delimter.
        sp = strtok(line, ",");
        
        // Converts the string into an integer. 
        routerId = atoi(sp);

        // Picks up where it left over at the comman and continue on.
        sp = strtok(NULL, ",");
        strcpy(type, sp);
        
        sp = strtok(NULL, ",");
        bandwidth = atoi(sp);
        printf("\n%d %s", routerId, type);
        */
    fclose(frtp);
    return true;
    }

void printRouterTypeElements(routerType routerType) 
{
    printf("%d\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d",
                            routerType.id,
                            routerType.type,
                            routerType.bandwidth,
                            routerType.wakeup_time,
                            routerType.latency,
                            routerType.power->idle,
                            routerType.power->peak,
                            routerType.power->sleep,
                            routerType.packet_memory);
}

bool readTrafficType()
{
}