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

static double matrix[ARRAY_LENGTH][ARRAY_LENGTH];

/**
 * @brief 
 * 
 * @param n 
 * @return int 
 */
static int multi_search_routine(double n){
    for(int i=0; i<ARRAY_LENGTH;i++){
        for(int j=0; j<ARRAY_LENGTH;j++){
            if(matrix[i][j] == n) return 1;
        }
    }

    return -1;
}

/**
 * @brief 
 * 
 * @param seed 
 */
void multi_search(int seed){
    
    //Matrix initialization
    random_set_seed(seed);
    for(int i=0; i<ARRAY_LENGTH;i++){
        random_get_array(matrix[i],ARRAY_LENGTH);
    }

    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        multi_search_routine(random_get());
    }
    MEASURE_STOP();

}
