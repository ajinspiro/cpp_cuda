# A collection of C++ programs

In this file, I would like to demostrate some statistics related simple math programs using C++.

### Find the sum, mean and standard deviation of a set of numbers read from command line using C++ arrays

```cpp
#include <iostream>
#include <string>
#include <cmath>

int main(int argc, char* argv[])
{
	if (argc < 3) { // exe name and 2 operands is min
		std::cerr << "SumAndAvgUsingArray: Improper usage. Need at least 2 operands." << std::endl;
		return EXIT_FAILURE;
	}
	float store[50] = {}, sum = 0.0, avg = 0.0, std_dev_tempstore[50] = {}, std_dev_tempsum = 0.0, std_dev = 0.0;
	for (int i = 1; i <= argc - 1; i++)
	{
		std::string numString = argv[i];
		float num = std::stof(numString);
		store[i - 1] = num;
	}
	for (size_t i = 0; i < argc - 1; i++)
	{
		sum += store[i];
	}
	avg = sum / (argc - 1);
	for (size_t i = 0; i < argc - 1; i++)
	{
		std_dev_tempstore[i] = std::pow((store[i] - avg), 2);
	}
	for (size_t i = 0; i < argc - 1; i++)
	{
		std_dev_tempsum += std_dev_tempstore[i];
	}
	std_dev = std::sqrt(std_dev_tempsum / (argc - 1));
	std::cout << "Sum: " << sum << " " << "Avg: " << avg << " " << "Std dev: " << std_dev << std::endl;
	return EXIT_SUCCESS;
}
```

### Same program using C++ pointers

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
              << "SD :" << std_dev << std::endl;
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
              << "SD :" << std_dev << std::endl;
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
    // Step 3: Divide
    std::cout << "CosineSimilarity: " << dot_product / (a_mag * b_mag) << std::endl;
}
```

### Find beta_0 and beta_1 (Linear regression closed form solution)

```cpp
#include <ranges>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// sample input (1,2) (-2,3) (5,6)
int main(void)
{
    std::cout << "Linear regression: Enter data set in the format (x,y) (x,y) (x,y). The format is whitespace sensitive."
              << std::endl
              << "> ";
    std::string input = {};
    std::vector<std::tuple<double, double>> dataset = {};
    std::tuple<double, double> x_y_sum_pair = {};
    double sum_x = 0.0, sum_y = 0.0, sum_x_squred = 0.0, sum_xy = 0.0, beta_0 = 0.0, beta_1 = 0.0, y_mean = 0.0, x_mean = 0.0;
    std::getline(std::cin, input);
    auto input_parts = input | std::views::split(' ');
    for (auto parenthesised_cordinate_pair : input_parts)
    {
        auto parenthesised_cordinate_pair_sv = std::string_view(parenthesised_cordinate_pair.begin(), parenthesised_cordinate_pair.end());
        auto parenthesis_stripped_cordinate_sv = parenthesised_cordinate_pair_sv.substr(1, parenthesised_cordinate_pair_sv.length() - 2);
        // split further by comma
        auto separated_cordinate_pair = parenthesis_stripped_cordinate_sv | std::views::split(',');
        for (auto [index, cordinate_component_string] : separated_cordinate_pair | std::views::enumerate)
        {
            auto temp = std::string(std::string_view(cordinate_component_string));
            double cordinate_component = std::stod(temp);
            if (index == 0)
            {
                dataset.push_back(std::tuple<double, double>(cordinate_component, 0));
            }
            else
            {
                std::get<1>(dataset.back()) = cordinate_component;
            }
        }
    }
    x_y_sum_pair = std::reduce(
        dataset.begin(),
        dataset.end(),
        std::tuple<double, double>(0, 0),
        [](std::tuple<double, double> cordinate_1, std::tuple<double, double> cordinate_2)
        {
            return std::tuple<double, double>(std::get<0>(cordinate_1) + std::get<0>(cordinate_2), std::get<1>(cordinate_1) + std::get<1>(cordinate_2));
        });
    sum_x = std::get<0>(x_y_sum_pair);
    sum_y = std::get<1>(x_y_sum_pair);
    sum_xy = std::transform_reduce(
        dataset.begin(),
        dataset.end(),
        0.0,                                       // reduce start value
        std::plus<>(),                             // reduce operation
        [](std::tuple<double, double> cordinate) { // transform operation
            return std::get<0>(cordinate) * std::get<1>(cordinate);
        });
    sum_x_squred = std::transform_reduce(
        dataset.begin(),
        dataset.end(),
        0.0,
        std::plus<>(),
        [](std::tuple<double, double> cordinate)
        {
            return std::pow(std::get<0>(cordinate), 2);
        });
    beta_1 = ((dataset.size() * sum_xy) - (sum_x * sum_y)) / ((dataset.size() * sum_x_squred) - std::pow(sum_x, 2));
    x_mean = sum_x / dataset.size();
    y_mean = sum_y / dataset.size();
    std::cout << "beta 1: " << beta_1 << std::endl;
    beta_0 = y_mean - (beta_1 * x_mean);
    std::cout << "beta 0: " << beta_0 << std::endl;
    return EXIT_SUCCESS;
}
```