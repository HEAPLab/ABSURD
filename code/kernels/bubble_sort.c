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

MEASURE_GLOBAL_VARIABLES()

static double array[ARRAY_LENGTH];

/**
 * @brief Actual bubble sort implementation
 * 
 */
static void bubble_sort_routine(){

    for(int i=0;i<ARRAY_LENGTH-1;i++){
        for(int j=0;j<ARRAY_LENGTH-i-1;j++){
            if(array[j]>array[j+1]){
                double temp=array[j];
                array[j]=array[j+1];
                array[j+1]=temp;
            }
        }
    }
    
}

/**
 * @brief It performs bubble sort on a random array for ITERATIONS times and measures the execution time
 * 
 * @param seed seed used to initialize random number generator  
 */
void bubble_sort(int seed){

    random_set_seed(seed);
    random_get_array(array,ARRAY_LENGTH);
    
    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        bubble_sort_routine();
    }
    MEASURE_STOP();
   
}