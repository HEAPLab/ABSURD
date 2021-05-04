# Kernels benchmarks

This folder contains common algorithms of various complexity. In particular:
- Matrices operations
    - `matrix_mult`: multiplication between two random square matrices;
    - `lu_dec`: LU decomposition of random square matrix (Doolittle algorithm);
    - `matrix_det`: computation of the determinant of a random square matrix exploiting LU decomposition;
    - `matrix_inv`: computation of the inverse of a random square matrix exploiting LU decomposition;
- Image manipulation
    - Morphological operations
        - `erode`: it applies erosion operation on a random binary image;
        - `dilate`: it applies dilation operation on a random binary image;
    - Image filtering
        - `avg_filter`: it applies average filtering to a random grayscale image;
        - `avg_filter_RGB`: it applies average filtering to a random RGB image exploiting multiple threads;
        - `gauss_filter`: it applies gaussian filtering to a random grayscale image;
        - `gauss_filter_RGB`: it applies gaussian filtering to a random grayscale image;
        
- Sorting algorithms:
    - `binary_tree_sort`: it sorts a random array using binary tree sort algorithm;
    - `bogo_sort`: it sorts a random array using bogo sort algorithm;
    - `bubble_sort`: it sorts a random array using bubble sort algorithm;
    - `insertion_sort`: it sorts a random array using insertion sort algorithm;
    - `quick_sort`: it sorts a random array using quick sort algorithm;

- Digital signal processing:
    - `fft`: it computes FFT of a random array exploiting Cooley–Tukey algorithm;
    - `dct`: it computes DCT of a random array;
    - `biquad`: it applies biquad filter to a noisy cosine signal;
    - `fir_avg`: it applies moving average filter to a noisy sine signal;
- Misc:
    - `crc_32`: it computes CRC32 of a random byte array;
    - `md5`: it computes md5 of a random byte array;

