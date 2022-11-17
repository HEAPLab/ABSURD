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

typedef struct{
    double re,im;
} complex;

static complex array_in[FFT_LENGTH];
static complex array_out[FFT_LENGTH];

/**
 * @brief It performs sum between two complex numbers
 * 
 * @param a first complex number
 * @param b second complex number
 * @return complex sum of a and b
 */
static complex complex_sum(complex a, complex b){
    complex res;
    res.re=a.re + b.re;
    res.im=a.im + b.im;
    return res;
}
/**
 * @brief It performs multiplication between two complex numbers
 * 
 * @param a first complex number
 * @param b second complex number
 * @return complex mult of a and b
 */
static complex complex_mult(complex a, complex b){
    complex res;
    res.re=(a.re * b.re) - (a.im*b.im);
    res.im=(a.im*b.re) + (a.re*b.im);
    return res;
}
/**
 * @brief It translates exponential form to Re and Im components
 * 
 * @param x exponent
 * @return Re and Im components
 */
static complex complex_exp(double x){
    /* e^(i*x)=cos(x) + i*sin(x)*/
    complex res;
    res.re=cos(x);
    res.im=sin(x);
    return res;
}

/**
 * @brief Actual fft implementation using Cooley–Tukey algorithm (radix-2 DIT case)
 * 
 * @return Fourier transform for input array
 */
static void fft_routine(){
    int k;

    for(k=0;k<FFT_LENGTH;k++){
        /*X_k=[sum{0,N/2-1} x_2n * e^(i*(-2*pi*2n*k)/N)] + [sum{0,N/2-1} x_(2n+1) * e^(i*(-2*pi*(2n+1)*k)/N)]*/
        int n;
        complex even_sum,odd_sum;

        even_sum.re=0;
        even_sum.im=0;

        for(n=0;n<FFT_LENGTH;n=n+2){
            complex n_term = complex_mult(array_in[n],complex_exp((-2*M_PI*n*k)/FFT_LENGTH));

            complex_sum(even_sum,n_term);
        }
        
        
        odd_sum.re=0;
        odd_sum.im=0;
        for(n=1;n<FFT_LENGTH;n=n+2){
            complex n_term = complex_mult(array_in[n],complex_exp((-2*M_PI*n*k)/FFT_LENGTH));

            complex_sum(odd_sum,n_term);
        }

        array_out[k] = complex_sum(even_sum,odd_sum);
    }
}

/**
 * @brief It computes FFT of a random array exploiting Cooley–Tukey algorithm. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void fft(){
    int i;
    /*Matrix initialization*/
    
    for(i=0; i<FFT_LENGTH;i++){
        complex x;
        x.re=random_get();
        x.im=random_get();
        
        array_in[i]=x;
    }

    MEASURE_START();
    
    fft_routine();
    
    MEASURE_STOP();


}
