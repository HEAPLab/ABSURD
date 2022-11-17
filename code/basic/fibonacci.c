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
#include "simple_random.h"
#include "user.h"


#ifdef CLASS_A
#define N 10L
#elif CLASS_B
#define N 15L
#elif CLASS_C
#define N 20L        
#elif CLASS_D
#define N 30L
#else
#define N 40L
#endif
MEASURE_GLOBAL_VARIABLES()

/**
 * Actual recursive implementation of Fibonacci's sequence.
 * 
 * @param n term to be computed
 * @return nth term of Fibonacci sequence
 */
static long int fibonacci_routine(long int n){
    if(n == 0) return 0;
    else if(n == 1) return 1;
    else return (fibonacci_routine(n-1) + fibonacci_routine(n-2));
}

/**
 * @brief It computes a random term of the Fibonacci's sequence . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void fibonacci(){
    long int res,n;

    n=(long int) N * random_get();
    
    MEASURE_START();
    
    res=fibonacci_routine(n);
    
    MEASURE_STOP();

    (void)res;  /* Unused */

}
