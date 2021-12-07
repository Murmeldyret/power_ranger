
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_ROUTERTYPE_MAX_LEN 100
#define CSV_ROUTERTYPE_NMBR_OF_PROPERTIES 9
#define CSV_ROUTERTYPE_CHAR_LEN 10 //Max length of an element in routerType.csv excluding first line

/**
 * Description: Initializes the data structure
 * Inputs: return parameters - pointer to the data structure
 * Output: struct *routertype, *traffictype
 */

bool initialise_data(routerType *routertypearray, trafficType *traffictypearray)
{
    bool routerType;
    bool trafficType;
    routerType = readRouterType(routertypearray);
    //printRouterTypeElements(routertypearray[0]);
    if (routerType == false)
    {
        printf("Error in readRouterType \n");
        return false;
    }
    
    trafficType = readTrafficType(traffictypearray);
    if (trafficType == false)
    {
        printf("Error in readTrafficType \n");
        return false;
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
    if(frtp == NULL)
    {
        printf("Error in opening routerType.csv \n");
        return false;
    }

    // TODO ryd op i variabler som ikke bliver brugt.
    
    
    char buffer[100]; 
    // TODO routerTypes array skal i main 
    //routerType routerTypes[NMBR_OF_ROUTERTYPES];
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
                routertypesarr[i].id = atoi(token);
                break;
            case 2:
                //routertypesarr[i].type = atoi(token);
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
        }  while (token != NULL);
        //printf("switch slut\n\n\n\n");
        i++;
    }
    

    fclose(frtp);
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
 * 
 * 
 *  
 *  
 * 
 */
bool readTrafficType(trafficType *traffictypearr)
{
    /*
    should output to struct in following order:
    id
    type
    latencySensitivity
    dataSize
    packetlossSensitivity
    id,type,latencySensitivity,dataSize,packetLossSensitivity
    All int
    


    Traffictypes:
    Movie = 1
    VoIP = 2
    */
    char buffer[100];
    char *token;
    int i = 0;
    int j = 1;
    //Opens trafficType.csv file
    FILE *fttp = fopen("config/trafficType.csv","r");
    //Checks that the file has been opened with success
    if(fttp == NULL)
    {
        printf("Error in opening trafficType.csv \n");
        return false;
    }

    while (feof(fttp) != true)
    {
        fgets(buffer,100,fttp);
        j = 1;
        token = strtok(buffer,",");
        do
        {
            printf("token: %s\n",token);
            switch (j)
            {
            case 1:
                traffictypearr[i].id = atoi(token);
                break;
            case 2:
                //traffictypearr[i].type = atoi(token);
                strcpy(traffictypearr[i].type, token);
                //printf("\n%s\n",traffictypearr[i].type);
                break;
            case 3:
                traffictypearr[i].latency_sensitivity = atoi(token);
                break;
            case 4:
                traffictypearr[i].data_size = atoi(token);
                break;
            case 5:
                //traffictypearr[i].packetloss_sensitivity = atoi(token);
                strcpy(traffictypearr[i].packetloss_sensitivity,token);
                break;
            default:
                printf("Error in readTrafficType Switch: Expected number from 1-5 but got %d\n",j);
                return false;
                break;
            }
            j++;
            token = strtok(NULL,",");
        } while (token != NULL && j <=5);
        
    i++;
    }

    fclose(fttp);
    return true;
}

// prints all elements of a single trafficType
void printTrafficTypeElements(trafficType trafficType)
{
    printf("\n%d\n%s\n%d\n%d\n%s\n",
                            trafficType.id,
                            trafficType.type,
                            trafficType.latency_sensitivity,
                            trafficType.data_size,
                            trafficType.packetloss_sensitivity);
    return;
}