/******************************************************************************
*   Copyright 2021 Politecnico di Milano
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*******************************************************************************/
#ifndef USER_H_
#define USER_H_

/** Set this to 0 to disable the outer loop on benchmarks (it will considerably reduce the execution time) **/
#define USE_ITERATIONS_LOOP 1

#include "dev_classes.h"

/**Uncomment the following constants to enable the use of user-defined images in the respective benchmarks  **/

/*#define USER_AVG_FILTER_RGB*/
/*#define USER_GAUSS_FILTER_RGB*/

/*#define USER_AVG_FILTER*/
/*#define USER_GAUSS_FILTER*/
/*#define USER_DILATE*/
/*#define USER_ERODE*/

/*#define USER_IMAGE_CANNY*/
/*#define USER_IMAGE_SCALING*/
/*#define USER_JPEG_COMPRESS*/

#define UINT32_T unsigned int

/** User-needed libraries (e.g., for measurements) **/
#include <assert.h>
#include <stdio.h>
#include <time.h>
#ifdef NUCLEO_F746ZG
#include "stm32f7xx_hal.h"
#endif

/** User-needed constants (e.g., for measurements) **/
#define BILLION 1000000000L
#define SEED 123U

#ifndef WITH_ANSI
#ifdef NUCLEO_F746ZG
   #define MEASURE_GLOBAL_VARIABLES()  extern TIM_HandleTypeDef htim5;\
                                       unsigned int result;
   #define MEASURE_START()  do { \
                              HAL_TIM_Base_Start(&htim5); \
                           } while(0);

   #define MEASURE_STOP() do { \
                                 HAL_TIM_Base_Stop(&htim5); \
                                  result=__HAL_TIM_GET_COUNTER(&htim5); \
                                 __HAL_TIM_SET_COUNTER(&htim5,0); \
                              } while(0);
#else
/** Variables declared in the global scope to support measurements **/
#define MEASURE_GLOBAL_VARIABLES()  static struct timespec start,stop; \
                                    double result;

/** The code to be executed when the time measurement starts **/
#define MEASURE_START()  do { \
                              result=0; \
                              clock_gettime(CLOCK_MONOTONIC, &start); \
                           } while(0);

/** The code to be executed when the time measurement stops **/
#define MEASURE_STOP() do { \
                              clock_gettime(CLOCK_MONOTONIC, &stop); \
                              result=(stop.tv_sec - start.tv_sec)*BILLION + (stop.tv_nsec - start.tv_nsec); \
                           } while(0);

#endif
#define CHECK_RESULT(x) assert(x);
#else
   #ifdef NUCLEO_F746ZG
   #define MEASURE_GLOBAL_VARIABLES()  extern TIM_HandleTypeDef htim5;\
                                       long unsigned int result;
   #define MEASURE_START()  do { \
                              HAL_TIM_Base_Start(&htim5); \
                           } while(0);

   #define MEASURE_STOP() do { \
                                 HAL_TIM_Base_Stop(&htim5); \
                                  result=__HAL_TIM_GET_COUNTER(&htim5); \
                                 __HAL_TIM_SET_COUNTER(&htim5,0); \

                              } while(0);
   #else
   #define MEASURE_GLOBAL_VARIABLES()
   #define MEASURE_START()
   #define MEASURE_STOP() 

#endif

#define CHECK_RESULT(x) assert(x);
#endif

#endif //USER_H_