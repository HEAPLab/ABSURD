
#ifdef CLASS_A
#define ARRAY_LENGTH 10
#define MATRIX_SIZE 10
#define BOGO_LENGTH 5
#define FFT_LENGTH 8
#define IMG_HEIGHT 32
#define IMG_WIDTH 32
#define NR_PLANES 1 /* acas */
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_B
#define ARRAY_LENGTH 100
#define MATRIX_SIZE 100
#define BOGO_LENGTH 6
#define FFT_LENGTH 128
#define IMG_HEIGHT 64
#define IMG_WIDTH 64
#define NR_PLANES 2 /* acas */
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_C
#define ARRAY_LENGTH 1000
#define MATRIX_SIZE 300
#define BOGO_LENGTH 8
#define FFT_LENGTH 512
#define IMG_HEIGHT 128
#define IMG_WIDTH 128
#define NR_PLANES 4 /* acas */
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_D
#define ARRAY_LENGTH 5000
#define MATRIX_SIZE 400
#define BOGO_LENGTH 9
#define FFT_LENGTH 1024
#define IMG_HEIGHT 256
#define IMG_WIDTH 256
#define NR_PLANES 8 /* acas */
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_E
#define ARRAY_LENGTH 10000
#define MATRIX_SIZE 500
#define BOGO_LENGTH 10
#define FFT_LENGTH 2048
#define IMG_HEIGHT 512
#define IMG_WIDTH 512
#define NR_PLANES 16 /* acas */
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

