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
 * @brief Actual binary search implementation
 * 
 * @param n number to be searched
 * @return int position of the number
 */
static int binary_search_routine(double n){

    int l_pos=0,r_pos=ARRAY_LENGTH-1;

    while(l_pos<= r_pos){
        
        int pos=(l_pos+r_pos)/2;
        if(array[pos] < n) l_pos=pos+1;
        else if (array[pos] > n) r_pos=pos-1;
        else return pos;
    }

    return -1;
}

/**
 * @brief It performs binary search of a random number on a random array. The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void binary_search(){
    int i,n,res;
    

    
    random_get_sarray(array,ARRAY_LENGTH);

    n=random_get()*ARRAY_LENGTH;
    
    MEASURE_START();
    
    for(i=0; i<ITERATIONS;i++){
        res=binary_search_routine(array[n]);
    }
    MEASURE_STOP();

    CHECK_RESULT(res==n);

}
