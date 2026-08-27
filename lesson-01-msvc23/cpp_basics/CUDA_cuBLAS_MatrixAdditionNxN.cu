#include <iostream>
#include <string>
#include <stdfloat>
#include <cuda_runtime.h>
#include <cublas_v2.h>

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

int main(int, char **)
{
    std::cout << "CUDA_cuBLAS_MatrixAdditionNxN: Performs scalar multiplication to 2 input matrices and then adds them or subtracts them." << std::endl;
    std::cout << "Enter order (n) of matrices (both matrices must be square matrices and must have the same order): " << std::endl;
    std::string order{};
    std::getline(std::cin, order);

    // incomplete
    return EXIT_SUCCESS;
}