#include <iostream>
#include <string>
#include <charconv>
#include <stdfloat>
#include <variant>
#include <cuda_runtime.h>
#include <cublas_v2.h>

struct DUError
{
}; // for use with std::variant

// INCOMPLETE -- dont run this program
void check_cuda(cudaError_t cudaError, std::string message)
{
    if (cudaError == cudaSuccess)
    {
        return;
    }
    std::cout << "CUDA failure: " << message << std::endl;
    exit(EXIT_FAILURE);
}

void check_cublas(cublasStatus_t cublasStatus, std::string message)
{
    if (cublasStatus == CUBLAS_STATUS_SUCCESS)
    {
        return;
    }
    std::cout << "cuBLAS failure: " << message << std::endl;
    exit(EXIT_FAILURE);
}

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
std::variant<DUError, T> read_number_from_stdin()
{
    std::variant<DUError, T> value{};
    std::string input_string{};
    T number_that_was_read = 0;
    std::getline(std::cin, input_string);
    auto [ptr, ec] = std::from_chars(input_string.data(), input_string.data() + input_string.size(), number_that_was_read);
    if (ec != std::errc())
        value = DUError();
    else
        value = number_that_was_read;
    return value;
}

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int main(int, char **)
{
    std::cout << "CUDA_cuBLAS_MatrixAdditionNxN: Performs NxN matrix addition." << std::endl;
    std::cout << "Enter order (N) of matrices (both matrices must be square matrices and must have the same order): ";
    unsigned int order_n = 0;
    std::variant<DUError, unsigned int> order_n_variant = read_number_from_stdin<unsigned int>();
    if ((std::holds_alternative<DUError>(order_n_variant)))
    {
        std::cout << "Invalid input - exiting..." << std::endl;
        return EXIT_FAILURE;
    }
    std::visit(
        overloaded{
            [](DUError) {}, // this is a stub. error is handled in the if block above.
            [&order_n](unsigned int value)
            {
                order_n = value;
            }},
        order_n_variant);
    // matrixA + matrixB = matrixC
    // Reading matrix A
    
    // std::cout << "N is " << (int)order_n << std::endl;
    // incomplete
    return EXIT_SUCCESS;
}