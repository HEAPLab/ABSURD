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
 * @brief Actual insertion sort implementation
 * 
 */
static void insertion_sort_routine(){
    int i;
    for(i=1; i<ARRAY_LENGTH; i++){
        int j=i-1;
        double temp=array[i];

        while(j>=0 && array[j]>temp){
            array[j+1]=array[j];
            j--;
        }
        array[j+1]=temp;
    }
    
}

/**
 * @brief It performs insertion sort on a random array . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void insertion_sort(){
    int i;
    
    random_get_array(array,ARRAY_LENGTH);

    MEASURE_START();
    
    insertion_sort_routine();
    
    MEASURE_STOP();
    
    for(i=0;i<ARRAY_LENGTH-1;i++){
        if(array[i]>array[i+1])
            break;
    }
    CHECK_RESULT(i==ARRAY_LENGTH-1);
}