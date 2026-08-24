#include <iostream>
#include <string>
#include <cmath>

// sample input command line args>CosineSimilarity 1,2 2,3
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