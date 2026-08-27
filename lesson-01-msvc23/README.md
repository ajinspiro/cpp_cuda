# cpp_basics : A collection of basic C++ programs

In this file, I would like to demostrate some statistics and matrices related simple math programs using C++.

1. Find the sum, mean and standard deviation of a set of numbers read from command line:
    - using C++ arrays. ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/SumMeanStdDeviationCppArray.cpp))
    - using C++ pointers. ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/SumMeanStdDeviationCppPointers.cpp))
    - using C++ template library. ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/SumMeanStdDeviationCppTemplateLib.cpp))
2. Find cosine similarity of a set of numbers read from command line. ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/CosineSimilarity.cpp))
3. Find $\beta_0$ and $\beta_1$ using linear regression closed form solution equations: ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/SimpleLinearRegressionClosedFormSolution.cpp))
    - $\beta_1 = \frac{n \sum_{i=1}^{n} (x_i y_i) - \left(\sum_{i=1}^{n} x_i\right) \left(\sum_{i=1}^{n} y_i\right)}{n \sum_{i=1}^{n} x_i^2 - \left(\sum_{i=1}^{n} x_i\right)^2}$
    - $\beta_0 = \bar{y} - \beta_1 \bar{x}$
4. Solve a system of linear equations using matrices ($X = A^{-1}B$) //_code:TODO_
    - nVIDIA cuSOLVER solution ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/CUDA_cuSOLVER_SolveMatrix.cu))
5. Matrix addition //_code:TODO_
    - nVIDIA cuBLAS solution ([code](https://github.com/ajinspiro/cpp_cuda/blob/main/lesson-01-msvc23/cpp_basics/CUDA_cuBLAS_MatrixAddition.cu))