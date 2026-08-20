#include <iostream>
#include <string>
#include <cmath>

int main(int argc, char *argv[])
{
    if (argc < 3)
    { // exe name and 2 operands is min
        std::cerr << "SumMeanStdDeviationCppArray: Improper usage. Need at least 2 operands." << std::endl;
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
    std::cout << "Sum: " << sum << " " << "Avg: " << avg << " " << "Std dev:" << std_dev << std::endl;
    return EXIT_SUCCESS;
}