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

static double mat[MATRIX_SIZE][MATRIX_SIZE];
static double l[MATRIX_SIZE][MATRIX_SIZE];
static double u[MATRIX_SIZE][MATRIX_SIZE];

/**
 * @brief Actual LU decomposition implementation
 * 
 */
static void lu_dec_routine(){
    int i,j;
    for(i=0;i<MATRIX_SIZE;i++){
        /*compute u mat i row*/
        for(j=i;j<MATRIX_SIZE;j++){
            double sum = 0;
            int k;
            for (k = 0; k < i; k++){
                sum += (l[i][k] * u[k][j]);
            }
            u[i][j]=mat[i][j] - sum;
        }  
        

        /*compute l mat j column*/
        for(j=i;j<MATRIX_SIZE;j++){
            if(i==j){
                l[j][i] = 1; 
            }
            else
            {
                double sum = 0;
                int k;
                for (k = 0; k < i; k++){
                    sum += l[j][k] * u[k][i];
                }
                
                l[j][i]= (mat[j][i] - sum) / u[i][i];
            }
            
        }
    }   
    
}

/**
 * @brief It performs LU decomposition using Doolittle algorithm on a random square matrix . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void lu_dec(){
    int i,j;
    
    /*Matrix initialization*/
    
    for(i=0; i<MATRIX_SIZE;i++){
        random_get_array(mat[i],MATRIX_SIZE);
    }

    for(i=0; i<MATRIX_SIZE;i++){
        for(j=0; j<MATRIX_SIZE;j++){
            l[i][j]=0;
            u[i][j]=0;
        }
    }
    

    MEASURE_START();

    lu_dec_routine();
    
    MEASURE_STOP();

}