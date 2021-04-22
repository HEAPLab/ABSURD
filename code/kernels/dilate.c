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

/*#define ARRAY_LENGTH 11
static int array_in[ARRAY_LENGTH][ARRAY_LENGTH]={{0,0,0,0,0,0,0,0,0,0,0},
                                                 {0,1,1,1,1,0,0,1,1,1,0},
                                                 {0,1,1,1,1,0,0,1,1,1,0},
                                                 {0,1,1,1,1,1,1,1,1,1,0},
                                                 {0,1,1,1,1,1,1,1,1,1,0},
                                                 {0,1,1,0,0,0,1,1,1,1,0},
                                                 {0,1,1,0,0,0,1,1,1,1,0},
                                                 {0,1,1,0,0,0,1,1,1,1,0},
                                                 {0,1,1,1,1,1,1,1,0,0,0},
                                                 {0,1,1,1,1,1,1,1,0,0,0},
                                                 {0,0,0,0,0,0,0,0,0,0,0}
                                                 };
*/

/* 3x3 structuring elements with origin in (1,1) */
static int kernel[KERNEL_SIZE][KERNEL_SIZE]={
    {1,1,1},
    {1,1,1},
    {1,1,1}
};


static int superimpose(int p_x, int p_y){
    //Kernel origin coordinates
    int k_x=KERNEL_SIZE/2;
    int k_y=KERNEL_SIZE/2;

    //limit of the "superimposition"
    int s_xl=p_x-k_x<0? 0: p_x-k_x; 
    int s_xr=p_x+k_x>=ARRAY_LENGTH? ARRAY_LENGTH-1 : p_x+k_x;

    int s_yl=p_y-k_y<0? 0: p_y-k_y; 
    int s_yr=p_y+k_y>=ARRAY_LENGTH? ARRAY_LENGTH-1 : p_y+k_y;

    
    for(int i=s_xl;i<=s_xr;i++){
        for(int j=s_yl;j<=s_yr;j++){
            if(kernel[i-s_xl][j-s_yl] && array_in[i][j]){
                return 1;
            }
        }
    }
    return 0;
}

static void dilate_routine(){
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
void dilate(int seed){

    random_set_seed(seed);
    for (int i = 0; i < ARRAY_LENGTH; i++){
        random_get_barray(array_in[i],ARRAY_LENGTH);
    }
    
    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        dilate_routine();
    }
    MEASURE_STOP();
    
    /*for (int i = 0; i < ARRAY_LENGTH; i++){
        for (int j = 0; j < ARRAY_LENGTH; j++){
            printf("%d\t",array_out[i][j]);
        }
        printf("\n");
    }*/
}