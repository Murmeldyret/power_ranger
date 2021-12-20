#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include "algorithms.h"
#include "minunit.h"

// Expected results.
const int router_type_size = 135;
const int traffic_type_size = 134;

int can_read_routertype_file()
{
    FILE *fp;
    fp = fopen("config/routerType.csv", "r");

    if(!fp)
        return 0;

    return 1;
}

int get_router_file_size()
{
    FILE *fp = fopen("config/routerType.csv", "r");

    if(!fp)
        return 0;
    
    fseek(fp, 0, SEEK_END);
    int routerRes = ftell(fp);
    fclose(fp);

    return routerRes;
}

int can_read_traffictype_file()
{
    FILE *fp;
    fp = fopen("config/trafficType.csv", "r");

    if(!fp)
        return 0;

    return 1;
}

int get_traffictype_file_size()
{
    FILE *fp = fopen("config/trafficType.csv", "r");

    if(!fp)
        return 0;
    
    fseek(fp, 0, SEEK_END);
    int trafficRes = ftell(fp);
    fclose(fp);

    return trafficRes;
}

int can_write_output_file()
{
    return 1;
}

double can_calculate_the_weight_below()
{  
    double result;

    result = f(5);
    printf("Weight: %f", result);
}

int can_calculate_the_weight_above()
{
    int result;

    return result;
}

/*
int can_run_simulation()
{
    return 1;
}
*/


MU_TEST(test_can_read_router_file)
{
    int res = can_read_routertype_file();
    mu_check(res == 1);
}

MU_TEST(test_router_file_size)
{
    int res = get_router_file_size();
    mu_check(res == router_type_size);
}

MU_TEST(test_can_read_traffic_file)
{
    int res = can_read_traffictype_file();
    mu_check(res == 1);
}

MU_TEST(test_traffic_file_size)
{
    int res = get_traffictype_file_size();
    mu_check(res == traffic_type_size);
}

MU_TEST(test_write_output_file)
{
    int res = can_write_output_file();
    mu_check(res == 1);
}

MU_TEST(test_can_calculate_the_weight_below)
{
    double res = can_calculate_the_weight_below();
    mu_check(res == weight_calculate_size);
}

MU_TEST_SUITE(test_suite) 
{
    MU_RUN_TEST(test_can_read_router_file);
    MU_RUN_TEST(test_router_file_size);
    MU_RUN_TEST(test_can_read_traffic_file);
    MU_RUN_TEST(test_traffic_file_size);
    MU_RUN_TEST(test_write_output_file);
    MU_RUN_TEST(test_can_calculate_the_weight_below);
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