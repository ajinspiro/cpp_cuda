#include <iostream>
#include <string>
#include <cmath>

#define ARRAY_SIZE 3

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > ARRAY_SIZE)
    {
        std::cerr << "SumMeanStdDeviationCppArray: Improper usage. Minumem 2 operands and maximum " << ARRAY_SIZE << " operands." << std::endl;
        return EXIT_FAILURE;
    }
    double store[ARRAY_SIZE] = {}, sum = 0.0, avg = 0.0, std_dev_tempstore[ARRAY_SIZE] = {}, std_dev_tempsum = 0.0, std_dev = 0.0;
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