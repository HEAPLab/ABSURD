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

#include <random>

extern "C" void dist_uniform();

UINT32_T seed;

extern "C" void random_set_seed(UINT32_T _seed) {
	seed = _seed;
}

MEASURE_GLOBAL_VARIABLES()

void dist_uniform(){

    std::mt19937 gen(seed);

    std::uniform_int_distribution<> distrib(1, ITERATIONS);
    int nr = distrib(gen);
    int res = 0;

    MEASURE_START();
    
    for(int i=0; i<nr; i++){
        res++;
    }

    MEASURE_STOP();
    
    CHECK_RESULT(res == nr);
    
}
