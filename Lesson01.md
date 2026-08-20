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

### Finding cosine similarity

```cpp
#include <iostream>
#include <string>
#include <cmath>

int main(int argc, char **argv)
{
    std::cout << "Hello, from CosineSimilarity!\n";
    if (argc != 3)
    {
        std::cout << "CosineSimilarity: invalid usage. Provide 2 vectors as inputs in command line." << std::endl;
        return EXIT_FAILURE;
    }
    float vector_store[2][2] = {};
    for (size_t i = 1; i < argc; i++)
    {
        std::string vector_string = argv[i];
        size_t comma_index = vector_string.find(',');
        auto x = vector_string.substr(0, comma_index), y = vector_string.substr(comma_index + 1);
        vector_store[i - 1][0] = std::stof(x);
        vector_store[i - 1][1] = std::stof(y);
    }

    // Step 1: Find dot product
    auto dot_product = (vector_store[0][0] * vector_store[1][0]) + (vector_store[0][1] * vector_store[1][1]);
    // Step 2: Find magnitudes of each vector
    auto a_mag = std::sqrt(std::pow(vector_store[0][0], 2) + std::pow(vector_store[0][1], 2));
    auto b_mag = std::sqrt(std::pow(vector_store[1][0], 2) + std::pow(vector_store[1][1], 2));
    std::cout << "CosineSimilarity: " << dot_product / (a_mag * b_mag) << std::endl;
}

```