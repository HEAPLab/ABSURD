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

#ifdef CLASS_A
#define NN_EPOCH 100

#define IN_NODES 2

#define HIDDEN_NODES 4
#define HIDDEN_LAYERS 1

#define OUT_NODES 1

#define LR 0.1

#elif CLASS_B
#define NN_EPOCH 1000

#define IN_NODES 4

#define HIDDEN_NODES 6
#define HIDDEN_LAYERS 2

#define OUT_NODES 2

#define LR 0.1
#elif CLASS_C
#define NN_EPOCH 10000

#define IN_NODES 6

#define HIDDEN_NODES 8
#define HIDDEN_LAYERS 3

#define OUT_NODES 3

#define LR 0.1
#elif CLASS_D
#define NN_EPOCH 100000

#define IN_NODES 8

#define HIDDEN_NODES 10
#define HIDDEN_LAYERS 4

#define OUT_NODES 4

#define LR 0.1
#elif CLASS_E
#define NN_EPOCH 1000000

#define IN_NODES 10

#define HIDDEN_NODES 12
#define HIDDEN_LAYERS 5

#define OUT_NODES 5

#define LR 0.1
#endif

MEASURE_GLOBAL_VARIABLES()


static double train_in[ARRAY_LENGTH][IN_NODES];
static double train_out[ARRAY_LENGTH][OUT_NODES];
static double net_out[OUT_NODES];

static double in_weight[IN_NODES][HIDDEN_NODES];

static double hl_weight[HIDDEN_LAYERS][HIDDEN_NODES][HIDDEN_NODES];
static double hl_bias[HIDDEN_LAYERS][HIDDEN_NODES];

static double out_weight[HIDDEN_NODES][OUT_NODES];
static double out_bias[OUT_NODES];

static double temp_out[HIDDEN_LAYERS][HIDDEN_NODES];
static double delta_out[OUT_NODES];
static double delta_hidden[HIDDEN_LAYERS][HIDDEN_NODES];

static void shuffle(){
    int i;
     /*shuffle data*/
    for(i=ARRAY_LENGTH-1;i>=0;i--){
        int x,j;

        j= random_get()*ARRAY_LENGTH;

        for(x=0;x<IN_NODES;x++){
            double val;
            val=train_in[i][x];
            train_in[i][x]=train_in[j][x];
            train_in[j][x]=val;
        }
    
        for(x=0;x<OUT_NODES;x++){
            double val;
            val=train_out[i][x];
            train_out[i][x]=train_out[j][x];
            train_out[j][x]=val;
        }
        
    }
}

static double sigmoid(double x){
    return 1/(1+exp(-x));
}
static double d_sigmoid(double x){
    return x*(1-x);
}
static void init_train_data(){
    int i;
    for (i = 0; i < ARRAY_LENGTH; i++){
        random_get_array(train_in[i],IN_NODES);
    }
    for (i = 0; i < ARRAY_LENGTH; i++){
        random_get_array(train_out[i],OUT_NODES);
    }
    
   
}

static void init_weights(){
    int i,h,l;

    for(i=0;i<IN_NODES;i++){
        for ( h = 0; h < HIDDEN_NODES; h++){
            in_weight[i][h]=random_get();
        }
        
    }
    for(l=0;l<HIDDEN_LAYERS;l++){
        for ( h = 0; h < HIDDEN_NODES; h++){
            hl_bias[l][h]=random_get();
            for(i=0;i<HIDDEN_NODES;i++){
                hl_weight[l][h][i]=random_get();
            }
        }
        
    }
    
    for(i=0;i<OUT_NODES;i++){
        out_bias[i]=random_get();
        for ( h = 0; h < HIDDEN_NODES; h++){
            out_weight[h][i]=random_get();
        }
    }

}
static void forward_pass(int train_idx){
    int h,l,y;

    for(h=0;h<HIDDEN_NODES;h++){
        int x;
        double activation;
        
        
        activation=hl_bias[0][h];
        for(x=0;x<IN_NODES;x++){
            activation+=(in_weight[x][h]*train_in[train_idx][x]);
        }
        temp_out[0][h]=sigmoid(activation);
    }
    for(l=1;l<HIDDEN_LAYERS;l++){
        for(h=0;h<HIDDEN_NODES;h++){
            double activation;
            int x;

            activation=hl_bias[l][h];
            for(x=0;x<HIDDEN_NODES;x++){
                activation+=(hl_weight[l][h][x]*temp_out[l-1][h]);
            }
            temp_out[l][h]=sigmoid(activation);
        }
    }
    for(y=0;y<OUT_NODES;y++){
        double activation;
        int x;

        activation=out_bias[y];
        for(h=0;h<HIDDEN_NODES;h++){
            activation+=(out_weight[h][y]*temp_out[HIDDEN_LAYERS-1][h]);
        }
        net_out[y]=sigmoid(activation);
    }
}
static void back_propagation(int train_idx){
    int y,h,l,x;
    /*Compute deltas for OUTPUT LAYER*/
    for(y=0;y<OUT_NODES;y++){
       delta_out[y] = (train_out[train_idx][y]-net_out[y])*d_sigmoid(net_out[y]);
    }
    /* Compute deltas for HIDDEN LAYER */
    for(h=0;h<HIDDEN_NODES;h++){
        double d_error;

        d_error=0;
        for(y=0;y<OUT_NODES;y++){
            d_error+=delta_out[y]*out_weight[h][y];
        }
        delta_hidden[HIDDEN_LAYERS-1][h]=d_error*sigmoid(temp_out[HIDDEN_LAYERS-1][h]);
    }
    for(l=HIDDEN_NODES-2;l>=0;l--){
        for(h=0;h<HIDDEN_NODES;h++){
            double d_error;
            
            d_error=0;
            for(y=0;y<HIDDEN_NODES;y++){
                d_error+=delta_hidden[l+1][y]*hl_weight[l][h][y];
            }
            delta_hidden[l][h]=d_error*sigmoid(temp_out[l][h]);
        }
    }
    
    /*Update weights*/
    for(y=0;y<OUT_NODES;y++){
       out_bias[y]+=delta_out[y]*LR;
       for(h=0;h<HIDDEN_NODES;h++){
           out_weight[h][y]+=temp_out[HIDDEN_LAYERS-1][h]*delta_out[y]*LR;
       }
    }
    for(l=HIDDEN_NODES-2;l>0;l--){
        for(h=0;h<HIDDEN_NODES;h++){
            hl_bias[l][h]+=delta_hidden[l][h]*LR;
            for(x=0;x<IN_NODES;x++){
                hl_weight[l][h][x]+=temp_out[l-1][x]*delta_hidden[l][h]*LR;
            }
        }
    }
    
    for(h=0;h<HIDDEN_NODES;h++){
        hl_bias[0][h]+=delta_hidden[0][h]*LR;
        for(x=0;x<IN_NODES;x++){
            in_weight[x][h]+=train_in[train_idx][x]*delta_hidden[0][h]*LR;
        }
    }
    
}

/**
 * @brief Actual neural network training implementation
 * 
 */
static void ann_routine(){
    int i,j;

    init_weights();
    for(i=0; i<NN_EPOCH;i++){
        
        shuffle();
        for(j=0; j<ARRAY_LENGTH; j++){
            forward_pass(j);
            
            back_propagation(j);
        }
    }

}

/**
 * @brief It trains a neural network on a random dataset. The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 * 
 * @param seed seed used to initialize random number generator  
 */
void ann(int seed){
    int i;

    random_set_seed(seed);
    
    init_train_data();

    MEASURE_START()
    for(i=0; i<ITERATIONS;i++){
        ann_routine();    
    }
    MEASURE_STOP()
}