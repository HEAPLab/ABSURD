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

static double mat_1[MATRIX_SIZE][MATRIX_SIZE];
static double mat_2[MATRIX_SIZE][MATRIX_SIZE];
static double res[MATRIX_SIZE][MATRIX_SIZE];

/**
 * @brief Actual matrices multiplication implementation
 * 
 */
static void matrix_mult_routine(){
    int i,j;
    for(i=0;i<MATRIX_SIZE;i++){
        for(j=0;j<MATRIX_SIZE;j++){
            double sum=0;
            int k;
            for(k=0;k<MATRIX_SIZE;k++){
                sum += mat_1[i][k]*mat_2[k][j];    
            }
            res[i][j]=sum;
        }
    }
}

/**
 * @brief It performs matrices multiplication between two random matrices . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void matrix_mult(){
    int i;
    /*Matrix initialization*/
    
    for(i=0; i<MATRIX_SIZE;i++){
        random_get_array(mat_1[i],MATRIX_SIZE);
        random_get_array(mat_2[i],MATRIX_SIZE);
    }

    MEASURE_START();
    
    matrix_mult_routine();
    
    MEASURE_STOP();


}