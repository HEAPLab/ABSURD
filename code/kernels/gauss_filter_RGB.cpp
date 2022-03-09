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
extern "C" {
#include "simple_random.h"
}

#include <math.h>
#include <thread>

#ifdef USER_GAUSS_FILTER_RGB
#include "data/gauss_filter_RGB_image.h"
#endif

#define KERNEL_SIZE 5


#define SIGMA 1.0

MEASURE_GLOBAL_VARIABLES()

#ifndef USER_GAUSS_FILTER_RGB
static unsigned char mat_in[3][IMG_HEIGHT][IMG_WIDTH];
static unsigned char mat_out[3][IMG_HEIGHT][IMG_WIDTH];
#endif

/* KERNEL_SIZExKERNEL_SIZE gaussian filter with origin in (1,1) */
static double kernel[KERNEL_SIZE][KERNEL_SIZE];

extern "C" void gauss_filter_RGB();
/**
 * @brief It generates a KERNEL_SIZE x KERNEL_SIZE gaussian kernel
 * 
 */
static void gaussian_kernel_init(){
    double sum=0;
    for (int i = 0; i < KERNEL_SIZE; i++) {
        for (int j = 0; j < KERNEL_SIZE; j++) {
            double x = i - (KERNEL_SIZE - 1) / 2.0;
            double y = j - (KERNEL_SIZE - 1) / 2.0;
            kernel[i][j] =  exp(((pow(x, 2) + pow(y, 2)) / ((2 * pow(SIGMA, 2)))) * (-1));
            sum += kernel[i][j];
        }
    }

    for (int i = 0; i < KERNEL_SIZE; i++) {
        for (int j = 0; j < KERNEL_SIZE; j++) {
            kernel[i][j] /= sum;
        }
    }

}

/**
 * @brief Performs 2D convolution of KERNEL_SIZExKERNEL_SIZE kernel with mat_in
 * 
 * @param channel current channel of the image
 * @param p_x  center point x coordinate
 * @param p_y center point y coordinate
 * @return int result of 2d convolution of kernel centred in mat_in[p_x][p_y]
 */
static int convolution2D(int channel,int p_x, int p_y){
    //Kernel radius 
    int k_r=KERNEL_SIZE/2;

    //kernel can be superimposed? if not we are on borders, then we keep the values unchanged
    if(p_x-k_r<0 || p_y-k_r<0 || p_x+k_r>=IMG_HEIGHT || p_y+k_r>=IMG_WIDTH){
        return mat_in[channel][p_x][p_y];
    }
    //offset between kernel's indexes and array's ones 
    int offset_x=p_x-k_r;
    int offset_y=p_y-k_r;

    double temp=0;
    for(int i=p_x-k_r;i<=p_x+k_r;i++){
        for(int j=p_y-k_r;j<=p_y+k_r;j++){
            temp+=kernel[i-offset_x][j-offset_y] * mat_in[channel][i][j];
        }
    }
    return temp;
}

/**
 * @brief Actual gaussian filter implementation
 * @param channel image channel to be elaborated
 */
static void gauss_filter_RGB_routine(int channel){
   for(int i=0;i<IMG_HEIGHT;i++){
       for(int j=0;j<IMG_WIDTH;j++){
           mat_out[channel][i][j]=convolution2D(channel,i,j);
       }
    }
    
}

/**
 * @brief It performs gaussian filtering on a random grayscale image . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void gauss_filter_RGB(){
    #ifndef USER_GAUSS_FILTER_RGB
    
    for(int c=0;c<3;c++){
        for (int i = 0; i < IMG_HEIGHT; i++){
            for (int j = 0; j < IMG_WIDTH; j++){
                mat_in[c][i][j]=random_get()*256;
            }
        }
    }
    #endif
    //kernel initialization
    gaussian_kernel_init();
    
    MEASURE_START();
    
    std::thread r(gauss_filter_RGB_routine,0);
    std::thread g(gauss_filter_RGB_routine,1);
    std::thread b(gauss_filter_RGB_routine,2);

    r.join();
    g.join();
    b.join();
    
    MEASURE_STOP();

}