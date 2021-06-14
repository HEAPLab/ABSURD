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


static double mat[ARRAY_LENGTH][ARRAY_LENGTH];
static double l[ARRAY_LENGTH][ARRAY_LENGTH];
static double u[ARRAY_LENGTH][ARRAY_LENGTH];
static double tmp[ARRAY_LENGTH];
static double inv[ARRAY_LENGTH][ARRAY_LENGTH];
/**
 * @brief It performs LU decomposition of mat matrix
 * 
 */
static void lu_decomposition(){
    int i;
    for(i=0;i<ARRAY_LENGTH;i++){
        /*compute u matrix i row*/
        int j;
        for(j=i;j<ARRAY_LENGTH;j++){
            double sum = 0;
            int k;
            for (k = 0; k < i; k++){
                sum += (l[i][k] * u[k][j]);
            }
            u[i][j]=mat[i][j] - sum;
        }  
        

        /*compute l matrix j column*/
        for(j=i;j<ARRAY_LENGTH;j++){
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
 * @brief Actual mat determinant implementation 
 * 
 * @return double mat determinant
 */
static double matrix_det(){
    int i;
    double det;
    
    det=1;
    for(i=0;i<ARRAY_LENGTH;i++){
        det *= l[i][i]*u[i][i];
    }
    return det;
}

/**
 * @brief Actual matrix inversion implementation exploiting LU decomposition
 * 
 * @return int 1 if inverse exists, 0 otherwise
 */
static int matrix_inv_routine(){
    int j;
    lu_decomposition();
    
    if(matrix_det() == 0){
        return 0;
    }
  
    /*forward-backward pass foreach column*/
    for(j=0;j<ARRAY_LENGTH;j++){
        /*first j rows equals to 0*/
        int i;
        for(i=0;i<j;i++) tmp[i]=0;
        tmp[j]=1;
        for(i=j+1;i<ARRAY_LENGTH;i++){
            double sum=0;
            int k;
            for(k=0;k<i;k++) sum+=l[i][k]*tmp[k];
            tmp[i]=-sum;
        }

        /*backward pass*/
        for(i=ARRAY_LENGTH-1;i>=0;i--){
            double sum=0;
            int k;
            for(k=i+1;k<ARRAY_LENGTH;k++) sum+=u[i][k]*inv[k][j];
            inv[i][j]=(-sum+tmp[i])/u[i][i];
        }
       
    }

    return 1;
}

/**
 * @brief It computes, if possible, the inverse of a random square matrix. The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 * 
 * @param seed seed used to initialize random number generator  
 */
void matrix_inv(int seed){
    int i;
    /*Matrix initialization*/
    random_set_seed(seed);
    for(i=0; i<ARRAY_LENGTH;i++){
        random_get_array(mat[i],ARRAY_LENGTH);
    }



    MEASURE_START();
    for(i=0; i<ITERATIONS;i++){
        matrix_inv_routine();
    }
    MEASURE_STOP();



}