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

#define IN_NODES 2

#define HIDDEN_NODES 2
#define HIDDEN_LAYERS 1

#define OUT_NODES 1
#define LR 0.1
#define NN_EPOCH 1


MEASURE_GLOBAL_VARIABLES()


ANN_VAR_NOBOUNDS() static double train_in[ARRAY_LENGTH][IN_NODES];
ANN_VAR_NOBOUNDS() static double train_out[ARRAY_LENGTH][OUT_NODES];
ANN_VAR_NOBOUNDS() static double net_out[OUT_NODES];

ANN_VAR_NOBOUNDS() static double in_weight[IN_NODES][HIDDEN_NODES];

ANN_VAR_NOBOUNDS() static double hl_weight[HIDDEN_LAYERS][HIDDEN_NODES][HIDDEN_NODES];
ANN_VAR_NOBOUNDS() static double hl_bias[HIDDEN_LAYERS][HIDDEN_NODES];

ANN_VAR_NOBOUNDS() static double out_weight[HIDDEN_NODES][OUT_NODES];
ANN_VAR_NOBOUNDS() static double out_bias[OUT_NODES];

ANN_VAR_NOBOUNDS() static double temp_out[HIDDEN_LAYERS][HIDDEN_NODES];
ANN_VAR_NOBOUNDS() static double delta_out[OUT_NODES];
ANN_VAR_NOBOUNDS() static double delta_hidden[HIDDEN_LAYERS][HIDDEN_NODES];

static double sigmoid(double x){
    return 1/(1+exp(-x));
}
static double d_sigmoid(double x){
    return x*(1-x);
}
static void init_train_data(){
    ANN_VAR(0,ARRAY_LENGTH) int i;
    for (i = 0; i < ARRAY_LENGTH; i++){
        random_get_array(train_in[i],IN_NODES);
    }
    for (i = 0; i < ARRAY_LENGTH; i++){
        random_get_array(train_out[i],OUT_NODES);
    }
    
   
}

static void init_weights(){
    ANN_VAR(0,IN_NODES) int i;
    ANN_VAR(0,OUT_NODES) int o;
    ANN_VAR(0,HIDDEN_NODES) int h;
    ANN_VAR(0,HIDDEN_LAYERS) int l;

    ANN_LOOP_BOUND(IN_NODES)
    for(i=0;i<IN_NODES;i++){
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for ( h = 0; h < HIDDEN_NODES; h++){
            in_weight[i][h]=random_get();
        }
        
    }

    ANN_LOOP_BOUND(HIDDEN_LAYERS)
    for(l=0;l<HIDDEN_LAYERS;l++){
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for ( h = 0; h < HIDDEN_NODES; h++){
            hl_bias[l][h]=random_get();
            ANN_LOOP_BOUND(HIDDEN_NODES)
            for(i=0;i<HIDDEN_NODES;i++){
                hl_weight[l][h][i]=random_get();
            }
        }
        
    }

    ANN_LOOP_BOUND(OUT_NODES)
    for(o=0;o<OUT_NODES;o++){
        out_bias[o]=random_get();
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for ( h = 0; h < HIDDEN_NODES; h++){
            out_weight[h][o]=random_get();
        }
    }

}
static void forward_pass(int train_idx){
    ANN_VAR(0,HIDDEN_NODES) int h;
    ANN_VAR(1,HIDDEN_LAYERS) int l;
    ANN_VAR(0,OUT_NODES) int y;

    ANN_LOOP_BOUND(HIDDEN_NODES)
    for(h=0;h<HIDDEN_NODES;h++){
        int x;
        double activation;
        
        
        activation=hl_bias[0][h];
        ANN_LOOP_BOUND(IN_NODES)
        for(x=0;x<IN_NODES;x++){
            activation+=(in_weight[x][h]*train_in[train_idx][x]);
        }
        temp_out[0][h]=sigmoid(activation);
    }
    
    ANN_LOOP_BOUND(HIDDEN_LAYERS)
    for(l=1;l<HIDDEN_LAYERS;l++){
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for(h=0;h<HIDDEN_NODES;h++){
            double activation;
            int x;

            activation=hl_bias[l][h];
            ANN_LOOP_BOUND(HIDDEN_NODES)
            for(x=0;x<HIDDEN_NODES;x++){
                activation+=(hl_weight[l][h][x]*temp_out[l-1][h]);
            }
            temp_out[l][h]=sigmoid(activation);
        }
    }
    
    ANN_LOOP_BOUND(OUT_NODES)
    for(y=0;y<OUT_NODES;y++){
        double activation;

        activation=out_bias[y];
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for(h=0;h<HIDDEN_NODES;h++){
            activation+=(out_weight[h][y]*temp_out[HIDDEN_LAYERS-1][h]);
        }
        net_out[y]=sigmoid(activation);
    }
}
static void back_propagation(int train_idx){
    
    ANN_VAR(0,IN_NODES) int x;
    ANN_VAR(0,HIDDEN_NODES) int h;
    ANN_VAR(0,HIDDEN_NODES-2) int l;
    ANN_VAR(0,OUT_NODES) int y;
    
    /*Compute deltas for OUTPUT LAYER*/
    ANN_LOOP_BOUND(OUT_NODES)
    for(y=0;y<OUT_NODES;y++){
       delta_out[y] = (train_out[train_idx][y]-net_out[y])*d_sigmoid(net_out[y]);
    }
    /* Compute deltas for HIDDEN LAYER */
    ANN_LOOP_BOUND(HIDDEN_NODES)
    for(h=0;h<HIDDEN_NODES;h++){
        ANN_VAR_NOBOUNDS() double d_error;

        d_error=0;
        ANN_LOOP_BOUND(OUT_NODES)
        for(y=0;y<OUT_NODES;y++){
            d_error+=delta_out[y]*out_weight[h][y];
        }
        delta_hidden[HIDDEN_LAYERS-1][h]=d_error*sigmoid(temp_out[HIDDEN_LAYERS-1][h]);
    }
    
    ANN_LOOP_BOUND(HIDDEN_NODES-1)
    for(l=HIDDEN_NODES-2;l>=0;l--){
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for(h=0;h<HIDDEN_NODES;h++){
            ANN_VAR_NOBOUNDS() double d_error;
            
            d_error=0;
            ANN_LOOP_BOUND(HIDDEN_NODES)
            for(y=0;y<HIDDEN_NODES;y++){
                d_error+=delta_hidden[l+1][y]*hl_weight[l][h][y];
            }
            delta_hidden[l][h]=d_error*sigmoid(temp_out[l][h]);
        }
    }
    
    /*Update weights*/
    ANN_LOOP_BOUND(OUT_NODES)
    for(y=0;y<OUT_NODES;y++){
       out_bias[y]+=delta_out[y]*LR;
       ANN_LOOP_BOUND(HIDDEN_NODES)
       for(h=0;h<HIDDEN_NODES;h++){
           out_weight[h][y]+=temp_out[HIDDEN_LAYERS-1][h]*delta_out[y]*LR;
       }
    }
    
    ANN_LOOP_BOUND(HIDDEN_NODES-1)
    for(l=HIDDEN_NODES-2;l>0;l--){
        ANN_LOOP_BOUND(HIDDEN_NODES)
        for(h=0;h<HIDDEN_NODES;h++){
            hl_bias[l][h]+=delta_hidden[l][h]*LR;
            ANN_LOOP_BOUND(IN_NODES)
            for(x=0;x<IN_NODES;x++){
                hl_weight[l][h][x]+=temp_out[l-1][x]*delta_hidden[l][h]*LR;
            }
        }
    }
    
    ANN_LOOP_BOUND(HIDDEN_NODES)
    for(h=0;h<HIDDEN_NODES;h++){
        hl_bias[0][h]+=delta_hidden[0][h]*LR;
        ANN_LOOP_BOUND(IN_NODES)
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
    ANN_VAR(0,NN_EPOCH)     int i;
    ANN_VAR(0,ARRAY_LENGTH) int j;

    init_weights();
    ANN_LOOP_BOUND(NN_EPOCH)
    for(i=0; i<NN_EPOCH;i++) {
        ANN_LOOP_BOUND(ARRAY_LENGTH)
        for(j=0; j<ARRAY_LENGTH; j++) {
            forward_pass(j);
            back_propagation(j);
        }
    }

}

/**
 * @brief It trains a neural network on a random dataset. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void ann(){

    init_train_data();

    MEASURE_START()
    
    ann_routine();    
    
    MEASURE_STOP()
}
