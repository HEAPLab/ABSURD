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

#define KERNEL_SIZE 3

MEASURE_GLOBAL_VARIABLES()

static int array_in[ARRAY_LENGTH][ARRAY_LENGTH];
static int array_out[ARRAY_LENGTH][ARRAY_LENGTH];
/*#define ARRAY_LENGTH 13
static int array_in[ARRAY_LENGTH][ARRAY_LENGTH]={{1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,0,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                 {1,1,1,1,1,1,1,1,1,1,1,1,1}
                                                 };
*/

/* 3x3 structuring elements with origin in (1,1) */
static int kernel[KERNEL_SIZE][KERNEL_SIZE]={
    {1,1,1},
    {1,1,1},
    {1,1,1}
};


static int superimpose(int p_x, int p_y){
    //Kernel radius 
    int k_r=KERNEL_SIZE/2;

    //kernel can be superimposed? if not is 0
    if(p_x-k_r<0 || p_y-k_r<0 || p_x+k_r>=ARRAY_LENGTH || p_y+k_r>=ARRAY_LENGTH){
        return 0;
    }
    //offset between kernel's indexes and array's ones 
    int offset_x=p_x-k_r;
    int offset_y=p_y-k_r;

    for(int i=p_x-k_r;i<=p_x+k_r;i++){
        for(int j=p_y-k_r;j<=p_y+k_r;j++){
            if(!(kernel[i-offset_x][j-offset_y] && array_in[i][j])){
                return 0;
            }
        }
    }
    return 1;
}

static void erode_routine(){
   for(int i=0;i<ARRAY_LENGTH;i++){
       for(int j=0;j<ARRAY_LENGTH;j++){
           array_out[i][j]=superimpose(i,j);
       }
    }
    
}

/**
 * @brief 
 * 
 * @param seed 
 */
void erode(int seed){

    random_set_seed(seed);
    for (int i = 0; i < ARRAY_LENGTH; i++){
        random_get_barray(array_in[i],ARRAY_LENGTH);
    }
    
    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        erode_routine();
    }
    MEASURE_STOP();

    /*for (int i = 0; i < ARRAY_LENGTH; i++){
        for (int j = 0; j < ARRAY_LENGTH; j++){
            printf("%d\t",array_out[i][j]);
        }
        printf("\n");
    }*/
}