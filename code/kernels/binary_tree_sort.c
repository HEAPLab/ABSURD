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

typedef struct{
    double data;
    int left;
    int right;
} node;

static double array[ARRAY_LENGTH];
static double ordered[ARRAY_LENGTH];

static node btree[ARRAY_LENGTH];
static int last_used;

/**
 * @brief It adds elements to the binary tree
 * 
 * @param elem element to be inserted in the binary tree
 * @param curr index of the current node
 */
static void insert_node(double elem,int curr){
    if(elem < btree[curr].data){
        if(btree[curr].left == -1){
            last_used++;
            btree[curr].left=last_used;
            btree[last_used].data=elem;
            btree[last_used].left=-1;
            btree[last_used].right=-1;
            
        }
        else{
            insert_node(elem, btree[curr].left);
        }
    }
    else{
        if(btree[curr].right==-1){
            last_used++;
            btree[curr].right=last_used;
            btree[last_used].data=elem;
            btree[last_used].left=-1;
            btree[last_used].right=-1;
        }
        else{
            insert_node(elem, btree[curr].right);
        }

    }
    
}
/**
 * @brief It recursively scan the binary tree to build the ordered array
 * 
 * @param node_idx index of the next node
 */
static void order_sweep(int node_idx){
    if(node_idx != -1){
        order_sweep(btree[node_idx].left);
        ordered[last_used++]=btree[node_idx].data;
        order_sweep(btree[node_idx].right);
    }
    
}

/**
 * @brief Actual binary tree sort implementation
 * 
 */
static void binary_tree_sort_routine(){
    int i;

    /*Insert first node*/
    btree[0].data=array[0];
    btree[0].left=-1;
    btree[0].right=-1;
    
    for(i=1;i<ARRAY_LENGTH;i++){
        insert_node(array[i],0);
    }

    last_used=0;

    order_sweep(0);
     
    /*Reset for other iterations*/
    last_used=0;
}

/**
 * @brief It performs binary tree sort on a random array . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void binary_tree_sort(){
    int i;
    
    random_get_array(array,ARRAY_LENGTH);

    MEASURE_START();
    
    binary_tree_sort_routine();
    
    MEASURE_STOP();

    for(i=0;i<ARRAY_LENGTH-1;i++){
        if(ordered[i]>ordered[i+1])
            break;
    }
    
    CHECK_RESULT(i==ARRAY_LENGTH-1);
}