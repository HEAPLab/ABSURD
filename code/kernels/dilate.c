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
#ifdef USER_DILATE
#include "data/dilate_image.h"
#endif

#define KERNEL_SIZE 3

MEASURE_GLOBAL_VARIABLES()



#ifndef USER_DILATE
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
    int i,j,k_x,k_y,s_xl,s_xr,s_yl,s_yr;
    /*Kernel origin coordinates*/
    k_x=KERNEL_SIZE/2;
    k_y=KERNEL_SIZE/2;

    /*Limit of the "superimposition"*/
    s_xl=p_x-k_x<0? 0: p_x-k_x; 
    s_xr=p_x+k_x>=IMG_HEIGHT? IMG_HEIGHT-1 : p_x+k_x;

    s_yl=p_y-k_y<0? 0: p_y-k_y; 
    s_yr=p_y+k_y>=IMG_WIDTH? IMG_WIDTH-1 : p_y+k_y;

    
    for(i=s_xl;i<=s_xr;i++){
        for(j=s_yl;j<=s_yr;j++){
            if(kernel[i-s_xl][j-s_yl] && mat_in[i][j]){
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Actual Morphological dilation implementation
 * 
 */
static void dilate_routine(){
    int i,j;
    for(i=0;i<IMG_HEIGHT;i++){
        for(j=0;j<IMG_WIDTH;j++){
            mat_out[i][j]=convolution2D(i,j);
        }
    }
    
}

/**
 * @brief It performs Morphological dilation on a random binary matrix . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void dilate(){
    int i;

    #ifndef USER_DILATE
    
    for (i = 0; i < IMG_HEIGHT; i++){
        random_get_barray(mat_in[i],IMG_WIDTH);
    }
    #endif
    
    MEASURE_START();
    
    dilate_routine();
    
    MEASURE_STOP();
    
}