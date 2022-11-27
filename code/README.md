
## Directory Content
- `include`: Support header files and custom input data examples

- `basic`: Very simple programs, such as simple loops or finding the maximum of an array
- `kernels`: Common algorithms of various complexity (sorting, fft, etc.)
- `apps`: Complex and complete applications
- `synthetic`: Not really computing anything, e.g., distribution-based loops, etc.
- `test`: Contains code and CMake files used to test and generate the datasets.
- `util`: Utility functions (currently the implementation of the PRNG function)

## Benchmarks configuration
ABSURD benchmark suite is easily re-configurable in order to leave the possibility to the users to tailor the benchmarks to their needs.
To configure the suite the users just have to edit the [user.h](include/user.h) and [dev_classes.h](include/dev_classes.h) as explained in the following sections.

### Pseudo-Random Generator Seed
Users can set the seed used by the PRNG function editing the `SEED` macro inside [user.h](include/user.h) file. The sequence generated depends only on the seed used by the generator, thus allowing to have input reproducibility

### Benchmarks input
To make ABSURD usable in as many scenarios as possible, we implemented the concept of device classes (5 classes, letter A to E) defing various parameters, such as array lengths or the input size, with the idea of ​​having the size of the inputs linked to the device type, targetting from microcontrollers (with A class) to high-end devices (with E class). 
The users can find inside the file [dev_classes.h](include/dev_classes.h) the classes definitions and can modify them if not compatible with their scenarios.
Moreover, for benchmarks that work on images we offer the possibility to define custom input uncommenting the following macros inside [user.h](include/user.h):
```
/**Uncomment the following constants to enable the use of user-defined images in the respective benchmarks  **/

/*#define USER_AVG_FILTER_RGB*/
/*#define USER_GAUSS_FILTER_RGB*/

/*#define USER_AVG_FILTER*/
/*#define USER_GAUSS_FILTER*/
/*#define USER_DILATE*/
/*#define USER_ERODE*/

/*#define USER_IMAGE_CANNY*/
/*#define USER_IMAGE_SCALING*/
/*#define USER_JPEG_COMPRESS*/
``` 
The user-defined images must be defined as you can see in the example files in [data](include/data) folder.

### Time measurement
Time measurement macro functions are defined inside [user.h](include/user.h) file and in the subfiles under `include/platforms` directory.
By implementing the macros `MEASURE_GLOBAL_VARIABLES()`,`MEASURE_START()` and `MEASURE_STOP()` users are able to respectively: perform the setup needed for the measurement, start and stop the measure.
For example:
```
/** Variables declared in the global scope to support measurements **/
#define MEASURE_GLOBAL_VARIABLES()  static struct timespec start,stop; \
                                    double result;

/** The code to be executed when the time measurement starts **/
#define MEASURE_START() do{                                         \
                            result=0;                               \
                            clock_gettime(CLOCK_MONOTONIC, &start); \
                        } while(0);

/** The code to be executed when the time measurement stops **/
#define MEASURE_STOP()  do{                                                                               \
                            clock_gettime(CLOCK_MONOTONIC, &stop);                                        \
                            result=(stop.tv_sec - start.tv_sec)*BILLION + (stop.tv_nsec - start.tv_nsec); \
                        } while(0);
```
### Output test
Some benchmarks offer the possibility to verify the obtained result at the end of computation through `CHECK_RESULT(x)` macro defined inside [user.h](include/user.h) file. Users can implement their version editing this file.
For example:
```
#define CHECK_RESULT(x) assert(x);
```  
## How to compile
We include inside the repository the `CMake` files and code used to test and generate the datasets with Odroid-H2 and ST NUCLEO-F746ZG boards
To build the suite on your own, you have to clone the repository and create a folder where to save generated Makefiles, object files and output binaries:
```
    git clone git@github.com:HEAPLab/ABSURD.git
    cd ABSURD/code/test
    mkdir build
    cd build
```
Once done that, you have to configure the build through `cmake` command. For example:
```
cmake -DCLASS_A=ON -DNUCLEO_F746ZG=ON -DMULTI_THREAD=ON ..
```
Here is a full list of the available options and their descriptions:
- `CLASS_A`: compile using class A input parameters, default OFF;
- `CLASS_B`: compile using class B input parameters, default OFF;
- `CLASS_C`: compile using class C input parameters, default OFF;
- `CLASS_D`: compile using class D input parameters, default OFF;
- `CLASS_E`: compile using class E input parameters, default OFF;
- `MULTI_THREAD`: compile multi-threaded benchmarks. Default ON;
- `WITH_ANSI`: use ANSI C standard. Default OFF;
- `NUCLEO_F746ZG`: compile benchmarks for ST NUCLEO-F746ZG board. Default OFF. Due to memory size, benchmarks can be compiled only if class A or B are selected;

Finally, to compile:
```
make -j[number of jobs]
```

## Annotations
If you switch to `annotated` branch, you can find the same benchmarks but with annotated source code. 
In order to properly run your WCET tool, you need to set the following macros:
 - `#define ANN_LOOP_BOUND(iters)` - Maximum number of iterations of the following loop is `iters`
 - `#define ANN_VAR(min,max)` - The next variable declaration has boundaries [`min`;`max`]
 - `#define ANN_VAR_NOBOUNDS()` - The next variable declaration has no boundaries (bounded by variable itself)
 - `#define ANN_RECURSION(rec_nr)` - The next function is recursive and can be called at most `rec_nr` times

The entry point of each benchmark is not annotated but can be easily identified as the first non-static function. 
