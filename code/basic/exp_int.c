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

MEASURE_GLOBAL_VARIABLES()

static double fact(int n){
    if(n==0){
        return 1;
    }
    return n*fact(n-1);
}

static double dabs(double n){
    if(n<0){
        return -n;
    }
    return n;
}
static double exp_int_routine(double x, int n){
    //computed using Taylor series expansion
    double term_1=0;

    for(int i=1;i<=n;i++){
        term_1 += pow(x,i)/(fact(i)*i);
    }

    return EULER + log(dabs(x)) + term_1; 
}

/**
 * @brief 
 * 
 * @param seed 
 */
void exp_int(int seed){

    random_set_seed(seed);
    
    double x;
    do{
        x=(random_get()<0.5?-1:1) * random_get();
    }while (x==0);

    int n= random_get()*ARRAY_LENGTH;

    //printf("x=%f\tn=%d\t%f\n",x,n);
    double res;
    MEASURE_START();
    for(int i=0; i<1;i++){
        res=exp_int_routine(x,n);
    }
    MEASURE_STOP();
   
    //printf("res=%f\n",res);


    
}