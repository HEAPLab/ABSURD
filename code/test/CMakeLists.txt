#******************************************************************************
#   Copyright 2021 Politecnico di Milano
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#******************************************************************************/

project(HEAP-WCET-Benchmarks)
cmake_minimum_required(VERSION 3.00)

option(MULTI_THREAD "Compile multi-threaded benchmarks" ON)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
endif(CMAKE_COMPILER_IS_GNUCC)


file(GLOB BASIC_SRC ../basic/*.c)
foreach(PATH ${BASIC_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()

target_link_libraries(array_stats m)
target_link_libraries(exp_int m)

file(GLOB KERNELS_SRC ../kernels/*.c)
foreach(PATH ${KERNELS_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()

target_link_libraries(biquad m)
target_link_libraries(eq_root m)
target_link_libraries(fft m)
target_link_libraries(fir_avg m)
target_link_libraries(dct m)
target_link_libraries(gauss_filter m)

if(MULTI_THREAD)
    find_package (Threads)
    SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

    file(GLOB KERNELS_SRC_MT ../kernels/*.cpp)
    foreach(PATH ${KERNELS_SRC_MT})
        get_filename_component(BENCH_NAME ${PATH} NAME_WE)
        message("Configuring ${BENCH_NAME}...")
        configure_file( main.c.in mains/main_${BENCH_NAME}.c )
        add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH})
        target_include_directories(${BENCH_NAME} PRIVATE ../include)
        target_link_libraries(${BENCH_NAME} Threads::Threads)
    endforeach()

    target_link_libraries(gauss_filter_RGB m)
endif()

file(GLOB KERNELS_SRC ../apps/*.c)
foreach(PATH ${KERNELS_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()

target_link_libraries(tcas m)

