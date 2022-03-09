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


#define ACCURACY 0.00000001

MEASURE_GLOBAL_VARIABLES()

static double dabs(double x){
    if(x<0) x=-x;
    return x;
}

/**
 * @brief Actual square root implementation 
 * 
 * @param s 
 * @return double square root of s
 */
static double bsqrt_routine(int s){
   double x,y;
   if(s<0){
       return -1;
   }
   x=s;
   y=(x+s/x)/2;

   while(x-y>ACCURACY){
       x=y;
       y=(x+s/x)/2;
   }
   return x;
}

/**
 * @brief It computes square root of a random double using Babylonian algorithm. The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void bsqrt(){

    int i,s;
    double res;
    
    

    s = random_get()*ARRAY_LENGTH;


    MEASURE_START();
    for(i=0; i<ITERATIONS;i++){
        res=bsqrt_routine(s);
    }
    MEASURE_STOP();
 
    
    CHECK_RESULT((res!=-1 && s>=0) && dabs(res*res-s)<CLASS_PRECISION);
    
}