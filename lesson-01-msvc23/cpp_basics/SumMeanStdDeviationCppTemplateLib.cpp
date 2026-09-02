#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "SumMeanStdDeviationCppTemplateLib: invalid usage. Provide at least 2 numbers as inputs in command line." << std::endl;
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
              << "SD: " << std_dev << std::endl;
}