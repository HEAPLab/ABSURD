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

static double mat_1[ARRAY_LENGTH][ARRAY_LENGTH];
static double mat_2[ARRAY_LENGTH][ARRAY_LENGTH];
static double res[ARRAY_LENGTH][ARRAY_LENGTH];

/**
 * @brief Actual matrices multiplication implementation
 * 
 */
static void matrix_mult_routine(){
    for(int i=0;i<ARRAY_LENGTH;i++){
        for(int j=0;j<ARRAY_LENGTH;j++){
            double sum=0;
            for(int k=0;k<ARRAY_LENGTH;k++){
                sum += mat_1[i][k]*mat_2[k][j];    
            }
            res[i][j]=sum;
        }
    }
}

/**
 * @brief It performs matrices multiplication between two random matrices for ITERATIONS times and measures the execution time
 * 
 * @param seed seed used to initialize random number generator  
 */
void matrix_mult(int seed){
    
    //Matrix initialization
    random_set_seed(seed);
    for(int i=0; i<ARRAY_LENGTH;i++){
        random_get_array(mat_1[i],ARRAY_LENGTH);
        random_get_array(mat_2[i],ARRAY_LENGTH);
    }

    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        matrix_mult_routine();
    }
    MEASURE_STOP();


}