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
#include "user.h"
#include "simple_random.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MEASURE_GLOBAL_VARIABLES()


static double array_in[ARRAY_LENGTH];
static double array_out[ARRAY_LENGTH];
static double weights[5];


/**
 * @brief Actual fir_avg direct form 1 implementation
 * 
 */
static void fir_avg_routine(){
    int i,j;
    for(i=0;i<ARRAY_LENGTH-5;i++){
        double accum=0;
        for(j=0;j<5;j++){
            accum+=weights[j]*array_in[i+j];
        }
        array_out[i]=accum/5;
    }
}

/**
 * @brief It applies a moving average filter to an input signal. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void fir_avg(){
    int i;
    /*Signal initialization*/
    
    for(i=0;i<ARRAY_LENGTH;i++){
        double dt=0.0001;
        array_in[i]=sin(2*M_PI*120*dt*i)+random_get();
    }

    /*weight initialization*/
    random_get_array(weights,5);

    
    MEASURE_START();
    
    fir_avg_routine();
    
    MEASURE_STOP();

}