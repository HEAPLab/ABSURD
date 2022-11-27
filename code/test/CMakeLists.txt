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
option(WITH_ANSI "Use ANSI C standard" ON)
option(PLATFORM_NUCLEO_F746ZG "Compile benchmarks for NUCLEO-F746ZG board" OFF)
option(PLATFORM_LINUX "Compile benchmarks for Linux systems" OFF)

if(PLATFORM_NUCLEO_F746ZG)
	add_definitions(-DPLATFORM_NUCLEO_F746ZG)
elseif(PLATFORM_LINUX)
	add_definitions(-DPLATFORM_LINUX)
else()
	message(WARNING "You haven't defined a platform, I'm compiling a plain version, be sure to check user.h")
endif()

option(CLASS_A "CLASS A" OFF)
option(CLASS_B "CLASS B" OFF)
option(CLASS_C "CLASS C" OFF)
option(CLASS_D "CLASS D" OFF)
option(CLASS_E "CLASS E" OFF)

if(CLASS_A)
    add_definitions(-DCLASS_A)
elseif(CLASS_B)
    add_definitions(-DCLASS_B)
elseif(CLASS_C)
    add_definitions(-DCLASS_C)
elseif(CLASS_D)
    add_definitions(-DCLASS_D)
else()
    add_definitions(-DCLASS_E)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -pedantic-errors -Wno-implicit-fallthrough -Werror")

if(WITH_ANSI)
    message("Compiling using ANSI C standard")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c90")
    add_definitions(-DWITH_ANSI)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
endif(WITH_ANSI)

set(RANDOM_SRC "../util/simple_random.c")
message("Random src:${RANDOM_SRC}")

file(GLOB BASIC_SRC ../basic/*.c)
foreach(PATH ${BASIC_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH} ${RANDOM_SRC})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()

target_link_libraries(array_stats m)
target_link_libraries(exp_int m)
target_link_libraries(eq_root m)

file(GLOB KERNELS_SRC ../kernels/*.c)
foreach(PATH ${KERNELS_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH} ${RANDOM_SRC})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()

target_link_libraries(biquad m)

target_link_libraries(fft m)
target_link_libraries(fir_avg m)
target_link_libraries(dct m)
target_link_libraries(gauss_filter m)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -O0 -std=c++11 -pthread")

if(MULTI_THREAD)
    find_package (Threads)

    file(GLOB KERNELS_SRC_MT ../kernels/*.cpp)
    foreach(PATH ${KERNELS_SRC_MT})
        get_filename_component(BENCH_NAME ${PATH} NAME_WE)
        message("Configuring ${BENCH_NAME}...")
        configure_file(main.c.in mains/main_${BENCH_NAME}.c )
        add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH} ${RANDOM_SRC})
        target_include_directories(${BENCH_NAME} PRIVATE ../include)
        target_link_libraries(${BENCH_NAME} Threads::Threads)
    endforeach()

    target_link_libraries(gauss_filter_RGB m)
endif()

file(GLOB KERNELS_SRC ../apps/*.c*)
foreach(PATH ${KERNELS_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH} ${RANDOM_SRC})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()

target_link_libraries(acas m)
target_link_libraries(canny m)
target_link_libraries(ann m)


file(GLOB SYNTHETIC_SRC ../synthetic/*.cpp)
foreach(PATH ${SYNTHETIC_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring ${BENCH_NAME}...")
    configure_file( main.c.in mains/main_${BENCH_NAME}.c )
    add_executable(${BENCH_NAME} mains/main_${BENCH_NAME}.c ${PATH})
    target_include_directories(${BENCH_NAME} PRIVATE ../include)
endforeach()


IF(NUCLEO_F746ZG AND (CLASS_A OR CLASS_B))
    add_subdirectory(STM32F7)
endif()
