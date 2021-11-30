//#include <json-c/json.h> // TODO: Include this header file
#include "data.h"


/**
 * Function: data_init
 * Description: Initializes the data structure
 * Inputs: return parameters - pointer to the data structure
 * Output: struct routertype[], traffictype[]
 */
bool initialise_data()
{
    FILE = *fp;
    init_routertype();
    init_traffictype();

    return true; // Return true if successful
}

