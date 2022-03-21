This folder contains the datasets of execution times of the benchmarks run on two platforms:
* ST NUCLEO-F746ZG, a development board with STM32F746ZG MCU clocked at 216MHz and equipped with 320KB of SRAM.
* Odroid-H2, equipped with an  Intel 2.5 Ghz quad-core x86 processor and 4GB of dual-channel DDR4 memory.

In particular:
* All the execution times are available under the `exec_times/` directory, split per platform and per class.
 * All the measurement units are in microseconds (with nanosecond resolution for Odroid-H2)

* The directory `stat/` contains the basic statistical information (average, minimum observed execution time, 
  maximum observed execution time, standard deviation, coefficient of variation)
* The directory `pwcet/` contains the pWCET (Generalized Pareto Distributions) estimated by the
  [chronovise](https://github.com/federeghe/chronovise) tool. Note that some of the generated pWCET may be
  incorrect and even several orders of magnitude larger than the Worst-Case Observed Time. Other pWCET
  estimations failed and are not available. The objective of this file is not to perform a proper complete
  pWCET analysis, but only to provide an hint related to the pWCET when possible.
* The directory `plots/` contain the histograms (and pWCET complementary-CDF when available) of all the
  measurements.
