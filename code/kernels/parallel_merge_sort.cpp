#include <thread>

#include "user.h"
extern "C" {
#include "simple_random.h"
}

#define NR_THREADS 3

MEASURE_GLOBAL_VARIABLES()

static UINT32_T array[PARALLEL_SORT_LENGTH];
static UINT32_T array_out[PARALLEL_SORT_LENGTH];

/**
 * @brief Worker thread execution
 * 
 */
static void parallel_merge_sort_thread(int id) {
    const int range_start = id * PARALLEL_SORT_LENGTH / NR_THREADS;
    int range_end   = (id+1) * PARALLEL_SORT_LENGTH / NR_THREADS;
    
    if (id == NR_THREADS-1) {
        range_end=PARALLEL_SORT_LENGTH;    // Last thread gets the last number in odds arrays
    }
    
    // Use bubble sort on small arrays
    for (int i=range_start; i<range_end-1; i++) {
        for(int j=0;j<range_end-i-1;j++) {

            if(array[range_start+j]>array[range_start+j+1]) {
                UINT32_T temp=array[range_start+j];
                array[range_start+j]=array[range_start+j+1];
                array[range_start+j+1]=temp;
            }
        }
    }
    
}

/**
 * @brief Actual parallel merge sort implementation
 * 
 */
static void parallel_merge_sort_routine(void) {
    int positions[NR_THREADS];
    std::thread threads[NR_THREADS];

    for(int i=0; i<NR_THREADS; i++) {
        threads[i] = std::move(std::thread(parallel_merge_sort_thread, i));  // Move semantics to avoid overhead
        positions[i]=i*PARALLEL_SORT_LENGTH / NR_THREADS;   // Initial index positions
    }

    // Wait the ordering of each thread
    for(int i=0; i<NR_THREADS; i++) {
        threads[i].join();
    }

    
    // Now we perform the merge
    for(int i=0; i<PARALLEL_SORT_LENGTH; i++) {
        UINT32_T min = -1;
        int min_idx = -1;
        for(int j=0; j<NR_THREADS; j++) {
            if ((positions[j] < (j+1) * PARALLEL_SORT_LENGTH / NR_THREADS)
               || (j == NR_THREADS-1 && positions[j] < PARALLEL_SORT_LENGTH ) ) {

                if (array[positions[j]] < min) {
                    min = array[positions[j]];
                    min_idx = j;
                }
            }
        }

        positions[min_idx]++;
        array_out[i] = min;
    }

}



/**
 * @brief It performs a parallel merge sort on a random array. The execution time is measured
 *        through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
extern "C"
void parallel_merge_sort(void) {
    int i;
    
    random_get_iarray(array, PARALLEL_SORT_LENGTH);
    
    MEASURE_START();
    
    parallel_merge_sort_routine();
    
    MEASURE_STOP();
   
    
    for(i=0;i<PARALLEL_SORT_LENGTH-1;i++) {
        if(array_out[i] > array_out[i+1]) {
            break;
        }
    }

    CHECK_RESULT(i==PARALLEL_SORT_LENGTH-1);
    
}
