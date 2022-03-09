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
#ifdef USER_ERODE
#include "data/erode_image.h"
#endif

#define KERNEL_SIZE 3

MEASURE_GLOBAL_VARIABLES()



#ifndef USER_ERODE
static int mat_in[IMG_HEIGHT][IMG_WIDTH];
static int mat_out[IMG_HEIGHT][IMG_WIDTH];
#endif

/* 3x3 structuring elements with origin in (1,1) */
static int kernel[KERNEL_SIZE][KERNEL_SIZE]={
    {1,1,1},
    {1,1,1},
    {1,1,1}
};

/**
 * @brief Performs 2D convolution of KERNEL_SIZExKERNEL_SIZE kernel with mat_in
 * 
 * @param p_x  center point x coordinate
 * @param p_y center point y coordinate
 * @return int result of 2d convolution of kernel centred in mat_in[p_x][p_y]
 */
static int convolution2D(int p_x, int p_y){
    int i,j,k_r,offset_x,offset_y;
    /*Kernel radius*/ 
    k_r=KERNEL_SIZE/2;

    /*kernel can be superimposed? if not is 0*/
    if(p_x-k_r<0 || p_y-k_r<0 || p_x+k_r>=IMG_HEIGHT || p_y+k_r>=IMG_WIDTH){
        return 0;
    }
    /*offset between kernel's indexes and array's ones*/ 
    offset_x=p_x-k_r;
    offset_y=p_y-k_r;

    for(i=p_x-k_r;i<=p_x+k_r;i++){
        for(j=p_y-k_r;j<=p_y+k_r;j++){
            if(!(kernel[i-offset_x][j-offset_y] && mat_in[i][j])){
                return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief Acutal morphological erosion implementation
 * 
 */
static void erode_routine(){
    int i,j;
    for(i=0;i<IMG_HEIGHT;i++){
        for(j=0;j<IMG_WIDTH;j++){
            mat_out[i][j]=convolution2D(i,j);
        }
    }
    
}

/**
 * @brief It performs Morphological erosion on a random binary matrix . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void erode(){
    int i;
    #ifndef USER_ERODE
    
    for (i = 0; i < IMG_HEIGHT; i++){
        random_get_barray(mat_in[i],IMG_WIDTH);
    }
    #endif
    
    MEASURE_START();
    
    erode_routine();
    
    MEASURE_STOP();

}