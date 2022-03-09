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

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_CXX_FLAGS "")

enable_language(ASM)


set(TOOLCHAIN_PREFIX arm-none-eabi-)


set(ARM_TOOLCHAIN_DIR "/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/bin")
set(BINUTILS_PATH "/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/bin")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -O0 -std=c++11")

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(RANDOM_SRC "../../util/simple_random.c")
file(GLOB BASIC_SRC ../../basic/*.c)
foreach(PATH ${BASIC_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring STM32 ${BENCH_NAME}...")
    configure_file( Core/Src/stm32_main.c.in mains/main_${BENCH_NAME}.c )
    file(GLOB CORE_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Src/*.c)
    file(GLOB CORE_INC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc/*.h)
    file(GLOB CORE_STR ${CMAKE_SOURCE_DIR}/STM32F7/Core/Startup/*.S)
    file(GLOB HAL_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
    set_property(SOURCE ${CORE_STR} PROPERTY LANGUAGE C)
    
    set(CUBEMX_FILES ${CORE_SRC} ${CORE_INC} ${CORE_STR} ${HAL_SRC})
    
    add_executable(${BENCH_NAME}_stm32 mains/main_${BENCH_NAME}.c ${PATH} ${CUBEMX_FILES} ${RANDOM_SRC})
    target_compile_definitions(${BENCH_NAME}_stm32 PUBLIC -DNUCLEO_F746ZG)
    target_include_directories(${BENCH_NAME}_stm32 PRIVATE 
                            "../../include"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Include")
    CHECK_INCLUDE_FILE("stm32f7xx_hal.h" x)
    CHECK_INCLUDE_FILE("main.h" z)
    CHECK_INCLUDE_FILE("user.h" y)
    message("Include:${x} ${y} ${z} ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc")
    target_compile_definitions(${BENCH_NAME}_stm32 PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F746xx
        )
        target_compile_options(${BENCH_NAME}_stm32 PRIVATE
        -mcpu=cortex-m7
        -std=gnu11
        -Wno-fatal-errors
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        
        -fdata-sections
        -ffunction-sections
        -fstack-usage
        -MMD
        -MP
        -Werror
        --specs=nano.specs
        -c
        -O0
        )

target_link_options(${BENCH_NAME}_stm32 PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F7/STM32F746ZGTX_FLASH.ld --specs=nosys.specs 
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -static
        -lc -lm 


        
        -Wl,-Map=wcet_template.map,--cref
        
        
        )
        # Print executable size
    add_custom_command(TARGET ${BENCH_NAME}_stm32
    POST_BUILD
    COMMAND arm-none-eabi-size ${BENCH_NAME}_stm32)

    # Create hex file
    add_custom_command(TARGET ${BENCH_NAME}_stm32
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.hex
    COMMAND arm-none-eabi-objcopy -O binary ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.bin)
endforeach()

target_link_libraries(array_stats_stm32 m)
target_link_libraries(exp_int_stm32 m)
target_link_libraries(eq_root_stm32 m)

file(GLOB KERNELS_SRC ../../kernels/*.c)
foreach(PATH ${KERNELS_SRC})
    get_filename_component(BENCH_NAME ${PATH} NAME_WE)
    message("Configuring STM32 ${BENCH_NAME}...")
    configure_file( Core/Src/stm32_main.c.in mains/main_${BENCH_NAME}.c )
    file(GLOB CORE_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Src/*.c)
    file(GLOB CORE_INC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc/*.h)
    file(GLOB CORE_STR ${CMAKE_SOURCE_DIR}/STM32F7/Core/Startup/*.S)
    file(GLOB HAL_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
    set_property(SOURCE ${CORE_STR} PROPERTY LANGUAGE C)
    
    set(CUBEMX_FILES ${CORE_SRC} ${CORE_INC} ${CORE_STR} ${HAL_SRC})
    
    add_executable(${BENCH_NAME}_stm32 mains/main_${BENCH_NAME}.c ${PATH} ${CUBEMX_FILES} ${RANDOM_SRC})
    target_compile_definitions(${BENCH_NAME}_stm32 PUBLIC -DNUCLEO_F746ZG)
    target_include_directories(${BENCH_NAME}_stm32 PRIVATE 
                            "../../include"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
                            "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Include")
    CHECK_INCLUDE_FILE("stm32f7xx_hal.h" x)
    CHECK_INCLUDE_FILE("main.h" z)
    CHECK_INCLUDE_FILE("user.h" y)
    message("Include:${x} ${y} ${z} ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc")
    target_compile_definitions(${BENCH_NAME}_stm32 PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F746xx
        )
        target_compile_options(${BENCH_NAME}_stm32 PRIVATE
        -mcpu=cortex-m7
        -std=gnu11
        
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        
        -fdata-sections
        -ffunction-sections
        -fstack-usage
        -MMD
        -MP
        -Werror
        --specs=nano.specs
        -c
        -O0
        )

target_link_options(${BENCH_NAME}_stm32 PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F7/STM32F746ZGTX_FLASH.ld --specs=nosys.specs 
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -static
        -lc -lm 


        
        -Wl,-Map=wcet_template.map,--cref
        
        
        )
        # Print executable size
    add_custom_command(TARGET ${BENCH_NAME}_stm32
    POST_BUILD
    COMMAND arm-none-eabi-size ${BENCH_NAME}_stm32)

    # Create hex file
    add_custom_command(TARGET ${BENCH_NAME}_stm32
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.hex
    COMMAND arm-none-eabi-objcopy -O binary ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.bin)
endforeach()

target_link_libraries(biquad_stm32 m)

target_link_libraries(fft_stm32 m)
target_link_libraries(fir_avg_stm32 m)
target_link_libraries(dct_stm32 m)
target_link_libraries(gauss_filter_stm32 m)

file(GLOB KERNELS_SRC ../../apps/*.c)
foreach(PATH ${KERNELS_SRC})
get_filename_component(BENCH_NAME ${PATH} NAME_WE)
message("Configuring STM32 ${BENCH_NAME}...")
configure_file( Core/Src/stm32_main.c.in mains/main_${BENCH_NAME}.c )
file(GLOB CORE_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Src/*.c)
file(GLOB CORE_INC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc/*.h)
file(GLOB CORE_STR ${CMAKE_SOURCE_DIR}/STM32F7/Core/Startup/*.S)
file(GLOB HAL_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
set_property(SOURCE ${CORE_STR} PROPERTY LANGUAGE C)

set(CUBEMX_FILES ${CORE_SRC} ${CORE_INC} ${CORE_STR} ${HAL_SRC})

add_executable(${BENCH_NAME}_stm32 mains/main_${BENCH_NAME}.c ${PATH} ${CUBEMX_FILES} ${RANDOM_SRC}) 
target_compile_definitions(${BENCH_NAME}_stm32 PUBLIC -DNUCLEO_F746ZG)
target_include_directories(${BENCH_NAME}_stm32 PRIVATE 
                        "../../include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Include")
CHECK_INCLUDE_FILE("stm32f7xx_hal.h" x)
CHECK_INCLUDE_FILE("main.h" z)
CHECK_INCLUDE_FILE("user.h" y)
message("Include:${x} ${y} ${z} ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc")
target_compile_definitions(${BENCH_NAME}_stm32 PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F746xx
    )
    target_compile_options(${BENCH_NAME}_stm32 PRIVATE
    -mcpu=cortex-m7
    -std=gnu11
    
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    
    -fdata-sections
    -ffunction-sections
    -fstack-usage
    -MMD
    -MP
    -Werror
    --specs=nano.specs
    -c
    -O0
    )

target_link_options(${BENCH_NAME}_stm32 PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F7/STM32F746ZGTX_FLASH.ld --specs=nosys.specs 
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -static
    -lc -lm 


    
    -Wl,-Map=wcet_template.map,--cref
    
    
    )
    # Print executable size
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-size ${BENCH_NAME}_stm32)

# Create hex file
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-objcopy -O ihex ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.hex
COMMAND arm-none-eabi-objcopy -O binary ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.bin)
endforeach()

target_link_libraries(acas_stm32 m)
target_link_libraries(canny_stm32 m)
target_link_libraries(ann_stm32 m)

file(GLOB KERNELS_SRC ../../apps/*.cpp)
foreach(PATH ${KERNELS_SRC})
get_filename_component(BENCH_NAME ${PATH} NAME_WE)
message("Configuring STM32 ${BENCH_NAME}...")
configure_file( Core/Src/stm32_main.c.in mains/main_${BENCH_NAME}.c )
file(GLOB CORE_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Src/*.c)
file(GLOB CORE_INC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc/*.h)
file(GLOB CORE_STR ${CMAKE_SOURCE_DIR}/STM32F7/Core/Startup/*.S)
file(GLOB HAL_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
set_property(SOURCE ${CORE_STR} PROPERTY LANGUAGE C)

set(CUBEMX_FILES ${CORE_SRC} ${CORE_INC} ${CORE_STR} ${HAL_SRC})


add_executable(${BENCH_NAME}_stm32 mains/main_${BENCH_NAME}.c ${PATH} ${CUBEMX_FILES} ${RANDOM_SRC}) 
target_compile_definitions(${BENCH_NAME}_stm32 PUBLIC -DNUCLEO_F746ZG)
target_include_directories(${BENCH_NAME}_stm32 PRIVATE 
                        "../../include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Include")
CHECK_INCLUDE_FILE("stm32f7xx_hal.h" x)
CHECK_INCLUDE_FILE("main.h" z)
CHECK_INCLUDE_FILE("user.h" y)
message("Include:${x} ${y} ${z} ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc")
target_compile_definitions(${BENCH_NAME}_stm32 PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F746xx
    )
    target_compile_options(${BENCH_NAME}_stm32 PRIVATE
    -mcpu=cortex-m7
    
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    
    -fdata-sections
    -ffunction-sections
    -fstack-usage
    -MMD
    -MP
    -Werror
    --specs=nano.specs
    -c
    -O0
    )

target_link_options(${BENCH_NAME}_stm32 PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F7/STM32F746ZGTX_FLASH.ld --specs=nosys.specs 
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -static
    -lc -lm 


    
    -Wl,-Map=wcet_template.map,--cref
    
    
    )
    # Print executable size
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-size ${BENCH_NAME}_stm32)

# Create hex file
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-objcopy -O ihex ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.hex
COMMAND arm-none-eabi-objcopy -O binary ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.bin)
endforeach()

file(GLOB KERNELS_SRC ../../kernels/*[a-z].cpp)
foreach(PATH ${KERNELS_SRC})
get_filename_component(BENCH_NAME ${PATH} NAME_WE)
message("Configuring STM32 ${BENCH_NAME}...")
configure_file( Core/Src/stm32_main.c.in mains/main_${BENCH_NAME}.c )
file(GLOB CORE_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Src/*.c)
file(GLOB CORE_INC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc/*.h)
file(GLOB CORE_STR ${CMAKE_SOURCE_DIR}/STM32F7/Core/Startup/*.S)
file(GLOB HAL_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
set_property(SOURCE ${CORE_STR} PROPERTY LANGUAGE C)

set(CUBEMX_FILES ${CORE_SRC} ${CORE_INC} ${CORE_STR} ${HAL_SRC})


add_executable(${BENCH_NAME}_stm32 mains/main_${BENCH_NAME}.c ${PATH} ${CUBEMX_FILES} ${RANDOM_SRC}) 
target_compile_definitions(${BENCH_NAME}_stm32 PUBLIC -DNUCLEO_F746ZG)
target_include_directories(${BENCH_NAME}_stm32 PRIVATE 
                        "../../include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Include")
CHECK_INCLUDE_FILE("stm32f7xx_hal.h" x)
CHECK_INCLUDE_FILE("main.h" z)
CHECK_INCLUDE_FILE("user.h" y)
message("Include:${x} ${y} ${z} ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc")
target_compile_definitions(${BENCH_NAME}_stm32 PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F746xx
    )
    target_compile_options(${BENCH_NAME}_stm32 PRIVATE
    -mcpu=cortex-m7
    
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    
    -fdata-sections
    -ffunction-sections
    -fstack-usage
    -MMD
    -MP
    -Werror
    --specs=nano.specs
    -c
    -O0
    )

target_link_options(${BENCH_NAME}_stm32 PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F7/STM32F746ZGTX_FLASH.ld --specs=nosys.specs 
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -static
    -lc -lm 


    
    -Wl,-Map=wcet_template.map,--cref
    
    
    )
    # Print executable size
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-size ${BENCH_NAME}_stm32)

# Create hex file
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-objcopy -O ihex ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.hex
COMMAND arm-none-eabi-objcopy -O binary ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.bin)
endforeach()

file(GLOB KERNELS_SRC ../../synthetic/*.cpp)
foreach(PATH ${KERNELS_SRC})
get_filename_component(BENCH_NAME ${PATH} NAME_WE)
message("Configuring STM32 ${BENCH_NAME}...")
configure_file( Core/Src/stm32_main.c.in mains/main_${BENCH_NAME}.c )
file(GLOB CORE_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Src/*.c)
file(GLOB CORE_INC ${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc/*.h)
file(GLOB CORE_STR ${CMAKE_SOURCE_DIR}/STM32F7/Core/Startup/*.S)
file(GLOB HAL_SRC ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
set_property(SOURCE ${CORE_STR} PROPERTY LANGUAGE C)

set(CUBEMX_FILES ${CORE_SRC} ${CORE_INC} ${CORE_STR} ${HAL_SRC})


add_executable(${BENCH_NAME}_stm32 mains/main_${BENCH_NAME}.c ${PATH} ${CUBEMX_FILES}) 
target_compile_definitions(${BENCH_NAME}_stm32 PUBLIC -DNUCLEO_F746ZG)
target_include_directories(${BENCH_NAME}_stm32 PRIVATE 
                        "../../include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Core/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
                        "${CMAKE_SOURCE_DIR}/STM32F7/Drivers/CMSIS/Include")
CHECK_INCLUDE_FILE("stm32f7xx_hal.h" x)
CHECK_INCLUDE_FILE("main.h" z)
CHECK_INCLUDE_FILE("user.h" y)
message("Include:${x} ${y} ${z} ${CMAKE_SOURCE_DIR}/STM32F7/Drivers/STM32F7xx_HAL_Driver/Inc")
target_compile_definitions(${BENCH_NAME}_stm32 PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F746xx
    )
    target_compile_options(${BENCH_NAME}_stm32 PRIVATE
    -mcpu=cortex-m7
    
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    
    -fdata-sections
    -ffunction-sections
    -fstack-usage
    -MMD
    -MP
    -Werror
    --specs=nano.specs
    -c
    -O0
    )

target_link_options(${BENCH_NAME}_stm32 PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F7/STM32F746ZGTX_FLASH.ld --specs=nosys.specs 
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -static
    -lc -lm 


    
    -Wl,-Map=wcet_template.map,--cref
    
    
    )
    # Print executable size
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-size ${BENCH_NAME}_stm32)

# Create hex file
add_custom_command(TARGET ${BENCH_NAME}_stm32
POST_BUILD
COMMAND arm-none-eabi-objcopy -O ihex ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.hex
COMMAND arm-none-eabi-objcopy -O binary ${BENCH_NAME}_stm32 ${BENCH_NAME}_stm32.bin)
endforeach()