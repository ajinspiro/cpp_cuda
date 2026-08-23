# cpp_basics : A collection of basic C++ programs

In this file, I would like to demostrate some statistics related simple math programs using C++.

1. Find the sum, mean and standard deviation of a set of numbers read from command line:
    - using C++ arrays. (code)
    - using C++ pointers. (code)
    - using C++ template lib. (code)
2. Find cosine similarity of a set of numbers read from command line. (code)
3. Find $\beta_0$ and $\beta_1$ using linear regression closed form solution equations: (code)
    - $\beta_1 = \frac{n \sum_{i=1}^{n} (x_i y_i) - \left(\sum_{i=1}^{n} x_i\right) \left(\sum_{i=1}^{n} y_i\right)}{n \sum_{i=1}^{n} x_i^2 - \left(\sum_{i=1}^{n} x_i\right)^2}$
    - $\beta_0 = \bar{y} - \beta_1 \bar{x}$