#include "minunit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int routerTypeSize = 135;
const int trafficTypeSize = 134;

int getRouterFileSize()
{
    FILE *fp = fopen("config/routerType.csv", "r");

    if(!fp)
        return 0;
    
    fseek(fp, 0, SEEK_END);

    int routerRes = ftell(fp);

    fclose(fp);

    return routerRes;
}

int getTrafficFileSize()
{
    FILE *fp = fopen("config/trafficType.csv", "r");

    if(!fp)
        return 0;
    
    fseek(fp, 0, SEEK_END);

    int trafficRes = ftell(fp);

    fclose(fp);

    return trafficRes;
}

MU_TEST(correctRouterFileSize)
{
    int res = getRouterFileSize();
    mu_check(res == routerTypeSize);
}

MU_TEST(correctTrafficFileSize)
{
    int res = getTrafficFileSize();
    mu_check(res == trafficTypeSize);
}

MU_TEST_SUITE(test_suite) 
{
    MU_RUN_TEST(correctRouterFileSize);
    MU_RUN_TEST(correctTrafficFileSize);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
    
    /*
    int res = getTrafficFileSize();
    if (res != -1)
        printf("Size of the file is %d bytes \n", res);
    */

	return MU_EXIT_CODE;
}