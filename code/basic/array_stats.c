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
#include <math.h>

#include "user.h"
#include "simple_random.h"

MEASURE_GLOBAL_VARIABLES()

static double array_1[ARRAY_LENGTH];
static double array_2[ARRAY_LENGTH];


static double sum(double a[]){
    double s=0;
    int i;

    for(i=0;i<ARRAY_LENGTH;i++){
        s += a[i];
    }
    return s;
}

static double mean(double a[]){
    return sum(a)/ARRAY_LENGTH;
}

static double var(double a[]){
    double m=mean(a), v=0;
    int i;

    for(i=0; i<ARRAY_LENGTH;i++){
        v += (a[i]-m)*(a[i]-m);
    }
    return v/ARRAY_LENGTH;
}

static double std(double a[]){
    return sqrt(var(a));
}

static double corr(double a[],double b[]){
    double mean_a=mean(a),mean_b=mean(b),temp=0;
    int i;

    for(i=0; i<ARRAY_LENGTH;i++){
        temp+=((a[i]-mean_a)*(b[i]-mean_b))/ARRAY_LENGTH;
    }

    return temp/(std(a)*std(b));

}

/**
 * @brief Actual array stats implementation
 * 
 */
static void array_stats_routine(){

    sum(array_1);
    sum(array_2);

    mean(array_1);
    mean(array_2);

    var(array_1);
    var(array_2);

    std(array_1);
    std(array_2);

    corr(array_1,array_2);

}

/**
 * @brief It computes some basic statistics on two random arrays . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void array_stats(){

    random_get_array(array_1,ARRAY_LENGTH);
    random_get_array(array_2,ARRAY_LENGTH);
    
    MEASURE_START();

    array_stats_routine();

    MEASURE_STOP();

}
