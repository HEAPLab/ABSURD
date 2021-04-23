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

/**
 * @brief Actual LU decomposition implementation
 * 
 */
static void lu_dec_routine(){
    
    for(int i=0;i<ARRAY_LENGTH;i++){
        //compute u mat i row
        for(int j=i;j<ARRAY_LENGTH;j++){
            double sum = 0;
            for (int k = 0; k < i; k++){
                sum += (l[i][k] * u[k][j]);
            }
            u[i][j]=mat[i][j] - sum;
        }  
        

        //compute l mat j column
        for(int j=i;j<ARRAY_LENGTH;j++){
            if(i==j){
                l[j][i] = 1; 
            }
            else
            {
                double sum = 0;
                for (int k = 0; k < i; k++){
                    sum += l[j][k] * u[k][i];
                }
                
                l[j][i]= (mat[j][i] - sum) / u[i][i];
            }
            
        }
    }   
    
}

/**
 * @brief It performs LU decomposition using Doolittle algorithm on a random square matrix for ITERATIONS times and measures the execution time
 * 
 * @param seed seed used to initialize random number generator  
 */
void lu_dec(int seed){
    
    //Matrix initialization
    random_set_seed(seed);
    for(int i=0; i<ARRAY_LENGTH;i++){
        random_get_array(mat[i],ARRAY_LENGTH);
    }

    for(int i=0; i<ARRAY_LENGTH;i++){
        for(int j=0; j<ARRAY_LENGTH;j++){
            l[i][j]=0;
            u[i][j]=0;
        }
    }
    
    /*for(int i=0; i<ARRAY_LENGTH;i++){
        for(int j=0; j<ARRAY_LENGTH;j++){
            printf("%.2f\t",mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        lu_dec_routine();
    }
    MEASURE_STOP();

   /* for(int i=0; i<ARRAY_LENGTH;i++){
        for(int j=0; j<ARRAY_LENGTH;j++){
            printf("%.2f\t",u[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for(int i=0; i<ARRAY_LENGTH;i++){
        for(int j=0; j<ARRAY_LENGTH;j++){
            printf("%.2f\t",l[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/
}