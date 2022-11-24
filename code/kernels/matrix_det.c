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

ANN_VAR_NOBOUNDS() static double mat[MATRIX_SIZE][MATRIX_SIZE];
ANN_VAR_NOBOUNDS() static double l[MATRIX_SIZE][MATRIX_SIZE];
ANN_VAR_NOBOUNDS() static double u[MATRIX_SIZE][MATRIX_SIZE];
/**
 * @brief It performs LU decomposition of mat matrix
 * 
 */
static void lu_decomposition(){
    ANN_VAR(0,ARRAY_LENGTH) int i;
    ANN_VAR(0,ARRAY_LENGTH) int j;
    
    ANN_LOOP_BOUND(MATRIX_SIZE)
    for(i=0;i<MATRIX_SIZE;i++){
        /*compute u matrix i row*/
        ANN_LOOP_BOUND(MATRIX_SIZE)
        for(j=i;j<MATRIX_SIZE;j++){
            ANN_VAR_NOBOUNDS() double sum = 0;
            ANN_VAR(0,ARRAY_LENGTH) int k;
            ANN_LOOP_BOUND(MATRIX_SIZE)
            for (k = 0; k < i; k++){
                sum += (l[i][k] * u[k][j]);
            }
            u[i][j]=mat[i][j] - sum;
        }  
        

        /*compute l matrix j column*/
        ANN_LOOP_BOUND(MATRIX_SIZE)
        for(j=i;j<MATRIX_SIZE;j++){
            if(i==j){
                l[j][i] = 1; 
            }
            else
            {
                ANN_VAR_NOBOUNDS() double sum = 0;
                ANN_VAR(0,MATRIX_SIZE) int k;
                for (k = 0; k < i; k++){
                    sum += l[j][k] * u[k][i];
                }
                
                l[j][i]= (mat[j][i] - sum) / u[i][i];
            }
            
        }
    }   
    
}
/**
 * @brief Actual mat determinant implementation 
 * 
 * @return double mat determinant
 */
static double matrix_det_routine(){
    ANN_VAR_NOBOUNDS() double det;
    ANN_VAR(0,MATRIX_SIZE) int i;

    lu_decomposition();
    det=1;
    ANN_LOOP_BOUND(MATRIX_SIZE)
    for(i=0;i<MATRIX_SIZE;i++){
        det *= l[i][i]*u[i][i];
    }
    return det;
}

/**
 * @brief It computes determinant of  a random square matrix exploiting LU decomposition . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void matrix_det(){
    ANN_VAR(0,MATRIX_SIZE) int i;
    
    /*Matrix initialization*/
    
    ANN_LOOP_BOUND(MATRIX_SIZE)
    for(i=0; i<MATRIX_SIZE;i++){
        random_get_array(mat[i],MATRIX_SIZE);
    }

    MEASURE_START();
    
    matrix_det_routine();
    
    MEASURE_STOP();


}
