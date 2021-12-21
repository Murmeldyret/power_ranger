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

## Software Test
The testing part of the software can be run with this command. However, keep in mind that it is specified for Linux. 

    ./test_ranger
    
Output of the test software should return something along the lines of.
       
    2 tests, 2 assertions, 0 failures
    Finished in 0.00009168 seconds (real) 0.00008856 seconds (proc)


## Dependencies
- CMake - https://cmake.org/
- IGraph - https://igraph.org/c/

## License
<a href="https://github.com/Murmeldyret/power_ranger/blob/main/LICENSE">Repository license</a>
