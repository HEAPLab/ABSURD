
#ifdef CLASS_A
#define ARRAY_LENGTH 100
#define FFT_LENGTH 128
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 100
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_B
#define ARRAY_LENGTH 1000
#define FFT_LENGTH 1024
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 10000
#else
    #define ITERATIONS 1
#endif
#else
#endif

#ifdef CLASS_C
#define ARRAY_LENGTH 10000
#define FFT_LENGTH 8192
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 100000 
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_D
#define ARRAY_LENGTH 100000
#define FFT_LENGTH 131072
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1000000 
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_E
#define ARRAY_LENGTH 1000000
#define FFT_LENGTH 1048576
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 10000000 
#else
    #define ITERATIONS 1
#endif
#endif

