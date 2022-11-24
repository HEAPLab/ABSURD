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

ANN_VAR_NOBOUNDS()
static double mat[MATRIX_SIZE][MATRIX_SIZE];

/**
 * @brief Actual implementation
 * 
 * @return int number of non negative numbers
 */
static int matrix_nn_routine(){
    ANN_VAR(0,MATRIX_SIZE) int cnt;
    ANN_VAR(0,MATRIX_SIZE) int i;
    ANN_VAR(0,MATRIX_SIZE) int j;
    
    cnt=0;
    ANN_LOOP_BOUND(MATRIX_SIZE)
    for(i=0; i<MATRIX_SIZE;i++){
        ANN_LOOP_BOUND(MATRIX_SIZE)
        for(j=0; j<MATRIX_SIZE;j++){
            if(mat[i][j] > 0) cnt++;
        }
    }

    return cnt;
}

/**
 * @brief It counts the non negative numbers in a random square matrix . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void matrix_nn(){
    ANN_VAR(0,MATRIX_SIZE) int i;
    ANN_VAR(0,MATRIX_SIZE) int j;
    ANN_VAR(0,MATRIX_SIZE) int res;
    /*Matrix initialization*/
    
    ANN_LOOP_BOUND(MATRIX_SIZE)
    for(i=0; i<MATRIX_SIZE;i++){
        random_get_array(mat[i],MATRIX_SIZE);
    }

    MEASURE_START();
    
    res=matrix_nn_routine();
    
    MEASURE_STOP();

    ANN_LOOP_BOUND(MATRIX_SIZE)
    for(i=0; i<MATRIX_SIZE;i++){
        ANN_LOOP_BOUND(MATRIX_SIZE)
        for(j=0; j<MATRIX_SIZE;j++){
            if(mat[i][j] > 0) res--;
        }
    }

    CHECK_RESULT(res==0);

}
