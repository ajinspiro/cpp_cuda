### Find the sum, mean and standard deviation of a set of numbers read from command line using C++ pointers

```cpp
#include <iostream>
#include <string>
#include <cmath>

// Program to find the sum, mean and standard deviation of an array of numbers using pointers.
int main(int argc, char **argv)
{
    std::cout << "Hello, from SumMeanStdDeviationCpp!\n";
    if (argc < 3)
    {
        std::cout << "SumMeanStdDeviationCpp: invalid usage. Provide at least 2 numbers as inputs in command line." << std::endl;
        return EXIT_FAILURE;
    }
    int size = sizeof(float) * (argc - 1);
    float *store = (float *)malloc(size);
    std::memset(store, 0, size);
    float sum = 0.0, mean = 0.0, std_dev = 0.0;
    for (size_t i = 1; i < argc; i++)
    {
        // std::cout << argv[i] << std::endl;
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
```

### Same program using C++ template lib

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>

int main(int argc, char **argv)
{
    std::cout << "Hello, from SumMeanStdDeviationTemplateCpp!" << std::endl;
    if (argc < 3)
    {
        std::cout << "SumMeanStdDeviationCpp: invalid usage. Provide at least 2 numbers as inputs in command line." << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<float> store = {};
    float sum{0.0}, mean = {0.0}, std_dev = {0.0};
    for (size_t i = 1; i < argc; i++)
    {
        float number = std::stof(argv[i]);
        store.insert(store.end(), number);
        sum += number;
    }
    mean = sum / (argc - 1);
    float acc_res = std::accumulate(
        store.begin(), store.end(), 0.0,
        [mean](float acc, float curr_el)
        {
            return acc + std::pow(curr_el - mean, 2);
        });
    std_dev = std::sqrt(acc_res / (argc - 1));

    std::cout << "Sum: " << sum << std::endl
              << "Mean: " << mean << std::endl
              << "SD:" << std_dev << std::endl;
}
```