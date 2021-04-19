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


static double matrix[ARRAY_LENGHT][ARRAY_LENGHT];

/**
 * @brief 
 * 
 * @return int 
 */
static int matrix_nn_routine(){
    int cnt=0;

    for(int i=0; i<ARRAY_LENGHT;i++){
        for(int j=0; j<ARRAY_LENGHT;j++){
            if(matrix[i][j] > 0) cnt++;
        }
    }

    return cnt;
}

/**
 * @brief 
 * 
 * @param seed 
 */
void matrix_nn(int seed){
    
    //Matrix initialization
    random_set_seed(seed);
    for(int i=0; i<ARRAY_LENGHT;i++){
        random_get_array(matrix[i],ARRAY_LENGHT);
    }

    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        matrix_nn_routine();
    }
    MEASURE_STOP();

}