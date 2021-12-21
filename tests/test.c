#include "minunit.h"
#include "../src/algorithms.h"

int file_size(FILE *input);

#define ROUTER_PATH "config/routerType.csv"
#define TRAFFIC_PATH "config/trafficType.csv"

// Expected results.
const int router_type_size = 202; // Bytes
const int traffic_type_size = 315; // Bytes

const double weight_below = 0;
const double weight_above = 0;

const double power_consumption = 64;

FILE *fp;

// Checks if the routertype file exists
int can_read_routertype_file()
{
    if((fp = fopen(ROUTER_PATH, "r")) == NULL)
    {
        return 0;
    }
    fclose(fp);

    return 1;
}

// Checks if the traffictype file exists
int can_read_traffictype_file()
{
    if((fp = fopen(TRAFFIC_PATH, "r")) == NULL)
    {
        return 0;
    }
    fclose(fp);

    return 1;
}

// Gets the file size of the routertype
int get_router_file_size()
{
    if((fp = fopen(ROUTER_PATH, "r")) == NULL)
    {
        return 0;
    }
    
    int routerRes = file_size(fp);
    return routerRes;
}

// Gets the file size of the traffictype
int get_traffic_file_size()
{
    if((fp = fopen(TRAFFIC_PATH, "r")) == NULL)
    {
        return 0;
    }

    int trafficRes = file_size(fp);
    return trafficRes;
}

// calculate the weight below 80% utilisation
int can_calculate_weight_below()
{   
    double below = f(80);
    return below;
}

// calculate the weight above 80% utilisation
int can_calculate_weight_above()
{
    double above = g(80);
    return above;
}

// calculate the power consumption of linear
int can_calculate_linear_power_consumption()
{
    double linear = linear_power_con(20, 75, 80, 1);
    return linear;
}

// Returns the size of a file in bytes
int file_size(FILE *input)
{
    long size;

    fseek(input, 0, SEEK_END);
    size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    fclose(input);

    return size;
}

// Test to check if the routertype file exist.
MU_TEST(test_can_read_router_file)
{
    int res = can_read_routertype_file();
    mu_check(res == 1);
}

// Test to check if the traffictype file exist.
MU_TEST(test_can_read_traffic_file)
{
    int res = can_read_traffictype_file();
    mu_check(res == 1);
}

// Test to check if the size of routertype file is equal to the expected size.
MU_TEST(test_router_file_size)
{
    int res = get_router_file_size();
    mu_check(res == router_type_size);
}

// Test to check if the size of traffictype file is equal to the expected size.
MU_TEST(test_traffic_file_size)
{
    int res = get_traffic_file_size();
    mu_check(res == traffic_type_size);
}

// Test to check if the calculation of weights below 80% is equal to the expected.
MU_TEST(test_calculate_weight_below)
{
    int res = can_calculate_weight_below();
    mu_check(res == weight_below);
}

// Test to check if the calculation of weights above 80% is equal to the expected.
MU_TEST(test_calculate_weight_above)
{
    int res = can_calculate_weight_above();
    mu_check(res == weight_above);
}

// Test to check if the calculation of linear power consumption is equal to the expected.
MU_TEST(test_calculate_linear_power_consumption)
{
    int res = can_calculate_linear_power_consumption();
    mu_check(res == power_consumption);
}

MU_TEST_SUITE(test_suite) 
{
    MU_RUN_TEST(test_can_read_router_file);
    MU_RUN_TEST(test_can_read_traffic_file);

    MU_RUN_TEST(test_router_file_size);
    MU_RUN_TEST(test_traffic_file_size);

    MU_RUN_TEST(test_calculate_weight_below);
    MU_RUN_TEST(test_calculate_weight_above);

    MU_RUN_TEST(test_calculate_linear_power_consumption);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}
