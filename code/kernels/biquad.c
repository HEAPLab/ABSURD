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

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MEASURE_GLOBAL_VARIABLES()


static double array_in[ARRAY_LENGTH];
static double array_out[ARRAY_LENGTH];



/**
 * @brief Actual biquad direct form 1 implementation
 * 
 * @param a0 Digital biquad filter a0 param
 * @param a1 Digital biquad filter a1 param
 * @param a2 Digital biquad filter a2 param
 * @param b0 Digital biquad filter b0 param
 * @param b1 Digital biquad filter b1 param
 * @param b2 Digital biquad filter b2 param
 */
static void biquad_routine(double a0, double a1, double a2, double b0, double b1, double b2){
    int i;

    /*Params normalization*/
    a1/=a0;
    a2/=a0;
    b0/=a0;
    b1/=a0;
    b2/=a0;

    array_out[0]=b0*array_in[0];
    array_out[1]=b0*array_in[1]+b1*array_in[0]-a1*array_out[0];

    for(i=2;i<ARRAY_LENGTH;i++){
        array_out[i]=b0*array_in[i]+b1*array_in[i-1]+b2*array_in[i-2]-a1*array_out[i-1]-a2*array_out[i-2];
    }
}

/**
 * @brief It applies a biquad lowpass filter to an input signal. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void biquad(){
    double alpha,w0,Q=0.5,f0=120,a0,a1,a2,b0,b1,b2;
    int i;

    /*Signal initialization*/
    
    for(i=0;i<ARRAY_LENGTH;i++){
        array_in[i]=cos(2*M_PI*120*0.001*i)+random_get();
    }

    /*param initialization*/
    Q=0.5;
    f0=120;
    w0=(2*M_PI*f0)/ARRAY_LENGTH;
    alpha=sin(w0)/(2*Q);

    
    a0=1+alpha;
    a1=-2*cos(w0);
    a2=1-alpha;

    b0=(1-cos(w0))/2;
    b2=(1-cos(w0))/2;

    b1=1-cos(w0);
    
    MEASURE_START();

    biquad_routine(a0,a1,a2,b0,b1,b2);

    MEASURE_STOP();


}