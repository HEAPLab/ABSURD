project(HEAP-WCET-Benchmarks)
cmake_minimum_required(VERSION 3.00)

option(MULTI_THREAD "Compile multi-threaded benchmarks" ON)

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

target_link_libraries(eq_root m)
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


