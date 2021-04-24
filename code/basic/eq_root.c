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

MEASURE_GLOBAL_VARIABLES()

typedef struct{
    double re;
    double im;
} complex;

static complex roots[2];

/**
 * @brief Actual quadratic equation solver implementation
 * 
 * @param a 
 * @param b 
 * @param c 
 */
static void eq_root_routine(double a, double b, double c){

    double delta=b*b-4*a*c;

    if(delta>=0){
        delta=sqrt(delta);

        roots[0].re=(-b+delta)/(2*a);
        roots[0].im=0;

        roots[1].re=(-b-delta)/(2*a);
        roots[1].im=0;
    }
    else{
        delta=sqrt(-delta);
        roots[0].re=-b/(2*a);
        roots[0].im=delta/(2*a);

        roots[1].re=-b/(2*a);
        roots[1].im=-delta/(2*a);
    }
}

/**
 * @brief It computes complex roots of a random quadratic equation . The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 * 
 * @param seed seed used to initialize random number generator  
 */
void eq_root(int seed){

    random_set_seed(seed);
    
    double a=random_get();
    double b=random_get()*2;
    double c=random_get();

   // printf("INPUT:%f\t%f\t%f\n",a,b,c);
    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        eq_root_routine(a,b,c);
    }
    MEASURE_STOP();
    //printf("solution 1:%f\t%f\n",roots[0].re,roots[0].im);
    //printf("solution 2:%f\t%f\n",roots[1].re,roots[1].im);


    
}