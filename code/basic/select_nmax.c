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

MEASURE_GLOBAL_VARIABLES();

static double array[ARRAY_LENGTH];

/**
 * @brief 
 * 
 * @param a 
 * @param pos 
 * @return double 
 */
static double select_nmax_routine(double a[], int pos){
    double nmax = 0;
    for(int j = 0; j < ARRAY_LENGTH; j++){
            if(a[j] > nmax){
                nmax = a[j];
            } 
    }

    for(int i=0;i<pos-1;i++){   
        double max=0; 
        for(int j = 0; j < ARRAY_LENGTH; j++){
            if(a[j] > max && a[j]<nmax){
                nmax = a[j];
            } 
        }
        nmax=max;
        
    }
    return nmax;

}

/**
 * @brief It searches for the random n-th greater element in a random array for ITERATIONS times and measures the execution time
 * 
 * @param seed seed used to initialize random number generator  
 */
void select_nmax(int seed){
    
    //Array initialization
    random_set_seed(seed);
    random_get_array(array,ARRAY_LENGTH);

    int pos=ARRAY_LENGTH * random_get();  
    
    MEASURE_START();
    
    for(int i=0; i<ITERATIONS;i++){
        select_nmax_routine(array,pos);
    }
    
    MEASURE_STOP();

}
