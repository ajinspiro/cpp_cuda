#include <iostream>
#include <string>
#include <cmath>

// Program to find the sum, mean and standard deviation of an array of numbers using pointers.
int main(int argc, char **argv)
{
    std::cout << "Hello, from SumMeanStdDeviationCppPointers!\n";
    if (argc < 3)
    {
        std::cout << "SumMeanStdDeviationCppPointers: invalid usage. Provide at least 2 numbers as inputs in command line." << std::endl;
        return EXIT_FAILURE;
    }
    int size = sizeof(float) * (argc - 1);
    float *store = (float *)malloc(size);
    std::memset(store, 0, size);
    float sum = 0.0, mean = 0.0, std_dev = 0.0;
    for (size_t i = 1; i < argc; i++)
    {
        float num = std::stof(argv[i]);
        sum += num;
        store[i - 1] = num;
    }
    mean = sum / (argc - 1);
    for (size_t i = 0; i < argc - 1; i++)
    {
        std_dev += std::pow(store[i] - mean, 2);
    }
    std_dev /= argc - 1;
    std_dev = std::sqrt(std_dev);
    std::cout << "Sum: " << sum << std::endl
              << "Mean: " << mean << std::endl
              << "SD:" << std_dev << std::endl;
}