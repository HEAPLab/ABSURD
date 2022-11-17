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


/**
 * @brief Actual DCT-II implementation
 * 
 * @return Discrete cosine transformation of input array
 */
static void dct_routine(){
    int k,n;
    for(k=0;k<ARRAY_LENGTH;k++){
       double sum=0;
       for(n=0;n<ARRAY_LENGTH;n++){
           sum += (array_in[n] * cos((M_PI*(n+0.5)*k)/ARRAY_LENGTH));
       }
       array_out[k]=sum;
   }
}

/**
 * @brief It computes DCT of a random array. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void dct(){
    /*Matrix initialization*/
    
    random_get_array(array_in,ARRAY_LENGTH);

    MEASURE_START();
    
    dct_routine();
    
    MEASURE_STOP();


}
