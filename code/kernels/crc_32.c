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

static unsigned char bytes_in[ARRAY_LENGTH];



/**
 * @brief Actual crc32 implementation
 * 
 * @return Cyclic redundancy check of the input bytes sequence.
 */
static unsigned int crc_32_routine(){
    
    unsigned int crc32 = 0xFFFFFFFF;
    int i;

    for(i=0;i<ARRAY_LENGTH;i++){
        int j;
        crc32 ^= bytes_in[i];
        for(j=0;j<8;j++){
            if(crc32 & 1){
               crc32 = (crc32 >> 1) ^ 0xEDB88320;
               
            } 
            else{
                crc32 >>= 1;
            }
        }
    }
    return ~crc32;
}

/**
 * @brief It computes CRC32 of a random byte array. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void crc_32(){
    
    int i;
    
    for(i=0;i<ARRAY_LENGTH;i++){
        bytes_in[i] = 0xFF & (int)(random_get()*ARRAY_LENGTH);
    }

 
    MEASURE_START();
    
    crc_32_routine();
    
    MEASURE_STOP();

    


}