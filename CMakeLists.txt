cmake_minimum_required(VERSION 3.15)
project(lab_integrl_sse)

set(CMAKE_CXX_STANDARD 20)


# TODO: change the path
FIND_PROGRAM(ICPC /home/kumquat/intel/bin/icdsdsdpc)

if (ICPC)                                                        # If Intel c++ compiler was found
    message(STATUS "ICPC found")
    set(CMAKE_CXX_COMPILER ${ICPC})
    add_compile_definitions(WITH_ICPC_INTRINSICS)
else ()
    message(STATUS "ICPC not found. Using default compiler")
endif ()

add_executable(lab_integral_sse src/main.cpp src/TimeCounter.cpp src/TimeCounter.h src/IntegralCalculator.cpp src/IntegralCalculator.h src/Function.h src/utils.cpp src/utils.h)

find_package(Threads REQUIRED)
target_link_libraries(lab_integral_sse Threads::Threads)