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

MEASURE_GLOBAL_VARIABLES();

/**
 * Compute recursively the nth term of Fibonacci sequence.
 * 
 * @param n term to be computed
 * @return nth term of Fibonacci sequence
 */
static int fibonacci_routine(int n){
    if(n == 0) return 0;
    else if(n == 1) return 1;
    else return (fibonacci_routine(n-1) + fibonacci_routine(n-2));
}

/**
 * @brief 
 * 
 * @param seed 
 */
void fibonacci(int seed){
    
    random_set_seed(seed);
    int n=ARRAY_LENGTH*random_get();
    
    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        fibonacci_routine(n);
    }
    
    MEASURE_STOP();

}
