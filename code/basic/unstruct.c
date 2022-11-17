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

/**
 * @brief An unstructured example using a do-while inside a switch
 * 
 * @param n 
 * @return int 
 */
static int unstruct_routine(int n){

    int i=0;

    switch (n%10)
    {
    case 0: do{     i++;
    case 9:         i++;
    case 8:         i++;
    case 7:         i++;
    case 6:         i++;
    case 5:         i++;
    case 4:         i++;
    case 3:         i++;
    case 2:         i++;
    case 1:         i++;
    }while (i<n);}
    
    return i;

}

/**
 * @brief 
 * 
 * @param seed 
 */
void unstruct(){

    int i,n,res;
    

    n=random_get()*ARRAY_LENGTH+1;
    
    MEASURE_START();
    for(i=0; i<ITERATIONS;i++){
        res=unstruct_routine(n);
    }
    MEASURE_STOP();

    CHECK_RESULT(res == n);
}
