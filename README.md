# ABSURD: Another Benchmark Suite for Real-Time Embedded Systems

An open-source collection of benchmarks for real-time systems.

## Why another benchmark suite?

Many benchmark suites are available, however, they have numerous limitations
and missing features with respect to modern embedded applications.

### Main features
The benchmarks contained in this suite are developed with the following
features and goals:
* Written in C or C++ 
* Do not need any external library
* The use of the C or C++ standard library is limited to the necessary cases
* Input sizes and values are never hard-coded and can be provided by the user or
  randomly generated
* Cover a wide range of systems, from a small microcontroller to a modern
  multi-core processor
* When possible, provide an automatic check for the generated output to verify the
  correct execution of the benchmark

### List of benchmarks
Check the list of benchmarks with their features
[here](docs/list-of-benchmarks.pdf).

## How to run this benchmark suite
The variety of embedded systems (platform architectures, compilers, tools,
operating systems, etc.) make the distribution of already compiled binaries for
the benchmark suite difficult and probably useless. You have to compile the C
programs, and the compilation process (e.g., make/cmake files) is up to you.
These benchmarks are developed to simplify the integration process. For
instance, they rely only on a small subset of standard C libraries\*, which are
usually available even in small microcontrollers.

Check the README in the `code` directory for further instruction on how to
compile the benchmark suite.

_\* With the exception of C++ multithread programs_

## Dataset & Reproducibility
We acquired time measurements on real platforms and published the data under
the `data` directory. In addition, to ensure reproducibility, a snapshot of
the code and the dataset is periodically performed (via Github tags) and
published on different platforms (such as zenodo.org). You can find the
resulting datasets and other information in the `data` directory.

## License
All the software and datasets are released under the Apache License. Please
check the [LICENSE](LICENSE) file.

## Contributors
The software is mainly developed by [HEAP Lab - Politecnico di Milano](https://heaplab.deib.polimi.it).
Current maintainer: federico.reghenzani ~snail~ polimi.it
Please check the Github authors' list for the individual contributors.
