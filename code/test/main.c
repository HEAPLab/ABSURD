#include <stdio.h>
#include <time.h>

#include "user.h"
#include "simple_random.h"



#define SEED 123

extern void binary_search(int seed);
extern void fibonacci(int seed);
extern void matrix_nn(int seed);
extern void multi_search(int seed);
extern void select_nmax(int seed);

int main(){

    printf("***************** HEAP WCET Benchmarks *****************\n");
    printf("Running with:\nITERATIONS:\t%d\nARRAY_LENGHT:\t%d\nSEED:\t%d\n",ITERATIONS,ARRAY_LENGHT,SEED);
    printf("********************************************************\n");

    binary_search(SEED);
    printf("[binary_search]\tMean execution time:\t%.2f ns\n",result/2);

    fibonacci(SEED);
    printf("[fibonacci]\tMean execution time:\t%.2f ns\n",result/ITERATIONS);

    matrix_nn(SEED);
    printf("[matrix_nn]\tMean execution time:\t%.2f ns\n",result/ITERATIONS);

    select_nmax(SEED);
    printf("[select_nmax]\tMean execution time:\t%.2f ns\n",result/ITERATIONS);

    multi_search(SEED);
    printf("[multi_search]\tMean execution time:\t%.2f ns\n",result/ITERATIONS);    
    
}