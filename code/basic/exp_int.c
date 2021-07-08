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

#define EULER 0.5772156649
#define RESULT 1.0649071946 /*Ei(0.7)*/
MEASURE_GLOBAL_VARIABLES()



static double fact(int n){
    if(n==0){
        return 1;
    }
    return n*fact(n-1);
}
/**
 * @brief It computes abs for double type
 * 
 * @param n 
 * @return double 
 */
static double dabs(double n){
    if(n<0){
        return -n;
    }
    return n;
}
/**
 * @brief Actual implementation of exponential integral function using Taylor series expansion
 * 
 * @param x 
 * @param n series index up to which expand Taylor series 
 * @return double value of Ei(x)
 */
static double exp_int_routine(double x, int n){
    
    double term_1=0;

    int i;
    for(i=1;i<=n;i++){
        term_1 += pow(x,i)/(fact(i)*i);
    }

    return EULER + log(dabs(x)) + term_1; 
}

/**
 * @brief It computes the value of the exponential integral function for a random number . The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 * 
 * @param seed seed used to initialize random number generator  
 */
void exp_int(int seed){
    int i,n;
    double x,res;
    random_set_seed(seed);
    
    do{
        x=(random_get()<0.5?-1:1) * random_get();
    }while (x==0);

    n= random_get()*ARRAY_LENGTH;


    MEASURE_START();
    
    for(i=0; i<1;i++){
        res=exp_int_routine(0.7,n);
    }
    MEASURE_STOP();
   

    CHECK_RESULT(dabs(res-RESULT)<0.01);
    
}