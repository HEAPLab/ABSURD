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
#include "stdio.h"
#define RESULT  1.15445

MEASURE_GLOBAL_VARIABLES()

static double dabs(double x){
    if(x<0) x=-x;
    return x;
}
/**
 * @brief f(x)=1/((x+1)^3+1)
 * 
 * @param x variable
 * @return double f(x)=1/((x+1)^3+1)
 */
static double f(double x){
    return 1/((x+1)*(x+1)*(x+1)+1);
}
/**
 * @brief Actual Monte Carlo integration algorithm 
 * 
 * @param a lower limit of integration
 * @param b upper limit of integration
 * @param n number of iterations
 * @return double integral estimate
 */
static double mc_integral_routine(double a, double b, int n){
    double sum=0;
    int i;
    for(i=0;i<n;i++){
        /*uniformly sampled point*/ 
        double ran_x=a+(b-a)*random_get();
        sum += f(ran_x);
    }
    return ((b-a)*sum)/n;
}

/**
 * @brief It computes integral of a function difficult to integrate analytically, exploiting Monte Carlo method. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void mc_integral(){

    double res;

    MEASURE_START();

    res=mc_integral_routine(-1,2,ITERATIONS);
    MEASURE_STOP(); 
    
    CHECK_RESULT(dabs(res-RESULT)<CLASS_PRECISION);
}
