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

static int partition(int low,int high){
    int i,j;
    double pivot,temp;
    
    pivot = array[high];
    i=low;
    for(j=low;j<=high;j++){
        if(array[j]<pivot){
            temp=array[j];
            array[j]=array[i];
            array[i]=temp;
            i++;
        }
    }
    temp=array[i];
    array[i]=array[high];
    array[high]=temp;
            
    return i;
}
/**
 * @brief Actual quick sort recursive implementation
 * 
 * @param low start index for partitions creation
 * @param high end index for partitions creation
 */
static void quick_sort_routine(int low,int high){

    if(low<high){
        int pivot;
        pivot=partition(low,high);
        quick_sort_routine(low, pivot-1);
        quick_sort_routine(pivot+1,high);
    }
    
}

/**
 * @brief It performs quick sort on a random array . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void quick_sort(){
    int i;

    
    random_get_array(array,ARRAY_LENGTH);

    MEASURE_START();
    
    quick_sort_routine(0,ARRAY_LENGTH-1);
    
    MEASURE_STOP();

    for(i=0;i<ARRAY_LENGTH-1;i++){
        if(array[i]>array[i+1])
            break;
    }
    CHECK_RESULT(i==ARRAY_LENGTH-1);
    
}