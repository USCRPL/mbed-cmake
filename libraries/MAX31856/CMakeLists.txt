set(MAX31856_SOURCES
        MAX31856.h
        MAX31856.cpp)

set(MAX31856_TEST_SOURCES
        TestMAX31856.cpp)

# qwiic relay library
add_library(max31856 STATIC ${MAX31856_SOURCES})
target_include_directories(max31856 PUBLIC .)
target_link_libraries(max31856 mbed-os)

# test program
add_mbed_executable(test_max31856 ${MAX31856_TEST_SOURCES})
target_link_libraries(test_max31856 max31856)