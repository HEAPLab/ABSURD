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

//TODO
static double f(double x){
    return x;
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
    for(int i=0;i<n;i++){
        //uniformly sampled point 
        double ran_x=a+(b-a)*random_get();
        sum += f(ran_x);
    }
    return ((b-a)*sum)/n;
}

/**
 * @brief It computes integral exploiting Monte Carlo method. The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 * 
 * @param seed seed used to initialize random number generator  
 */
void mc_integral(int seed){

    random_set_seed(seed);
    

    int s = random_get()*ARRAY_LENGTH;

    MEASURE_START();
    for(int i=0; i<1;i++){
        mc_integral_routine(0,1,s);
    }
    MEASURE_STOP(); 
}