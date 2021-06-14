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
 * @brief It randomly shuffle the array
 * 
 */
static void knuth_shuffle(){
    int i;
    for(i=ARRAY_LENGTH-1;i>=0;i--){
        int j= random_get()*ARRAY_LENGTH;

        double temp=array[i];
        array[i]=array[j];
        array[j]=temp;
        
    }

}
/**
 * @brief It checks if the array is ordered
 * 
 * @return int 1 if the array is ordered, 0 otherwise
 */
static int is_ordered(){
    int i;
    for(i=0;i<ARRAY_LENGTH-1;i++){
        if(array[i]>array[i+1]){
            return 0;
        }
    }
    return 1;
}
/**
 * @brief Actual bogo sort implementation
 * 
 */
static void bogo_sort_routine(){
    while (!is_ordered()){
        knuth_shuffle();
    }
}

/**
 * @brief It performs bogo sort on a random array . The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 * 
 * @param seed seed used to initialize random number generator  
 */
void bogo_sort(int seed){
    int i;
    random_set_seed(seed);
    random_get_array(array,ARRAY_LENGTH);
    
    MEASURE_START();
    for(i=0; i<ITERATIONS;i++){
        bogo_sort_routine();
    }
    MEASURE_STOP();
   
    
}