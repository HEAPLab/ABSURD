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

static double matrix[MATRIX_SIZE][MATRIX_SIZE];

/**
 * @brief It scans the matrix for the desired element
 * 
 * @param n number to be searched
 * @return int 1 if found, 0 otherwise
 */
static int multi_search_routine(double n){
    int i,j;
    for(i=0; i<MATRIX_SIZE;i++){
        for(j=0; j<MATRIX_SIZE;j++){
            if(matrix[i][j] == n) return 1;
        }
    }

    return 0;
}

/**
 * @brief It searches a random number in a random  matrix . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void multi_search(){
     int i,x,y,res;
    /*Matrix initialization*/
    
   
    for(i=0; i<MATRIX_SIZE;i++){
        random_get_array(matrix[i],MATRIX_SIZE);
    }

    x=random_get()*MATRIX_SIZE;
    y=random_get()*MATRIX_SIZE;

    MEASURE_START();
    
    res=multi_search_routine(matrix[x][y]);
    
    MEASURE_STOP();

    CHECK_RESULT(res);

}
