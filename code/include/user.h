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

/*
   32-bits Uniform random number generator U[0,1) lfsr113, based on the
   work of Pierre L'Ecuyer (http://www.iro.umontreal.ca/~lecuyer/myftp/papers/tausme2.ps)
*/

#ifndef USER_H_
#define USER_H_

#include <time.h>

#define CLASS_A
#include "dev_classes.h"


#define UINT32_T unsigned int


#define MEASURE_START()  do { \
                              result=0; \
                              clock_gettime(CLOCK_MONOTONIC, &start); \
                           } while(0);

#define MEASURE_STOP() do { \
                              clock_gettime(CLOCK_MONOTONIC, &stop); \
                              result=(stop.tv_sec - start.tv_sec)*BILLION + (stop.tv_nsec - start.tv_nsec); \
                           } while(0);

#define BILLION 1000000000L

//User defined variables
struct timespec start,stop;
double result;

#endif
