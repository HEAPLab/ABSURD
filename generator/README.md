This directory includes the execution time random generator based
on the statistical distribution of the execution time samples 
observed on the real platform. The data are in the `data` directory
that you can find in the repository root.

The generation is performed by estimating the [Empirical-CDF](https://en.wikipedia.org/wiki/Empirical_distribution_function)
and the applying the [Inverse Transform Sampling](https://en.wikipedia.org/wiki/Inverse_transform_sampling) method.

The generator is implemented in Python and is very simple to use. 
Take a look at the `test.py` file for usage.
