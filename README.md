<p align="center">
  <img src="https://cdn.flickeringmyth.com/wp-content/uploads/2018/01/Mighty_Morphin_Power_Rangers_Season_1_to_3_logo.png" alt="Smiley face" width="40%" height="40%">
</p>

# Power ranger - Energy optimization
Computer network simulation. Calculation the networks power consumption on static, dynamical, and dynamical with sleep mode. 

## Build
When building the project it will generate two programs. Power_ranger and Test_ranger. Where power_ranger is the main part of the software, and test_ranger is the testing part of the software.

    cmake CMakeLists.txt
    make

## Software Main
The main part of the software can be run with this command. However, keep in mind that it is specified for Linux. 

    ./power_ranger

The user will be prompted to write input parameters. 
When the user has written the input. The software will run and write an output.csv file that contian the output.


    Network simulation
    Simulation setup:
    Event spawn time:,3600
    Number of routers:,10000
    Number of links:,59979
    Number of events:,10000
    Total amount of data:,213844.047546,GB

    Simulation results:,Static,Dynamic,Dynamic with sleep states
    Power Consumption,7021.028445 kWh,1477.040533 kWh,950.069879 kWh
    Time (s),40836.000000 s,4987.000000 s,4987.000000 s



## Software Test
The testing part of the software can be run with this command. However, keep in mind that it is specified for Linux. 

    ./test_ranger
    
Output of the test software should return something along the lines of.
       
    2 tests, 2 assertions, 0 failures
    Finished in 0.00009168 seconds (real) 0.00008856 seconds (proc)

## Configuration files
Routertype.csv

| id  | type(int) | bandwidth(int) | wakeupTime (int) | latency(int) | powerIdle(int) | powerPeak (int) | powerSleep | packetMemory (int) |
| --- | --------- | -------------- | ---------------- | ------------ | -------------- | --------------- | ---------- | ------------------ |
| 1   | fafb      | 10000000Kbps   | 50ms             | 1ms          | 100W           | 250W            | 20W        | 16000B             |

Traffictype.csv
| id  | type(string)[16] | latencySensitivity(int) | dataSize (int) | packetlossSensitivity(int) |     |
| --- | ------------ | ----------------------- | -------------- | -------------------------- | --- |
| 1   | Movie        | 1000ms                  | 50000KB        | 20000Kbps                  | low |

## Dependencies
- CMake - https://cmake.org/
- IGraph - https://igraph.org/c/

## License
<a href="https://github.com/Murmeldyret/power_ranger/blob/main/LICENSE">Repository license</a>
