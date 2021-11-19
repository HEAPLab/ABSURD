
#ifdef CLASS_A
#define ARRAY_LENGTH 10
#define MATRIX_SIZE 10
#define BOGO_LENGTH 5
#define FFT_LENGTH 8
#define IMG_HEIGHT 64
#define IMG_WIDTH 128
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
#define IMG_HEIGHT 128
#define IMG_WIDTH 128
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
#define FFT_LENGTH 1280
#define IMG_HEIGHT 576
#define IMG_WIDTH 720
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_D
#define ARRAY_LENGTH 10000
#define MATRIX_SIZE 400
#define BOGO_LENGTH 9
#define FFT_LENGTH 8192
#define IMG_HEIGHT 600
#define IMG_WIDTH 800
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

#ifdef CLASS_E
#define ARRAY_LENGTH 100000
#define MATRIX_SIZE 600
#define BOGO_LENGTH 10
#define FFT_LENGTH 65536
#define IMG_HEIGHT 720
#define IMG_WIDTH 1280
#if USE_ITERATIONS_LOOP
    #define ITERATIONS 1
#else
    #define ITERATIONS 1
#endif
#endif

