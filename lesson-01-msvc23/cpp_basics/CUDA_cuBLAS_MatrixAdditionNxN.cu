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

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

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

template <Numeric T>
T read_number_from_stdin(std::string error_message)
{
    T value{};
    std::variant<DUError, T> du = read_number_from_stdin<T>();
    std::visit(overloaded{[error_message](DUError) -> void
                          {
                              std::cout << error_message << std::endl;
                              exit(EXIT_FAILURE);
                          },
                          [&value](T success_value) -> void
                          {
                              value = success_value;
                          }},
               du);
    return value;
}

int main(int, char **)
{
    std::cout << "CUDA_cuBLAS_MatrixAdditionNxN: Performs NxN matrix addition." << std::endl;
    std::cout << "Enter order (N) of matrices (both matrices must be square matrices and must have the same order): ";
    unsigned int order_n = read_number_from_stdin<unsigned int>("Invalid input for matrix order");
    unsigned int matrix_bytes = order_n * order_n * sizeof(float);
    // Reading matrix A
    float *host_matrixA = new float[order_n * order_n];
    std::cout << "Input matrixA (column major ordering, newline separated): ";
    for (size_t i = 0; i < order_n * order_n; i++)
    {
        *(host_matrixA + i) = read_number_from_stdin<float>("Invalid input - exiting");
    }
    // Reading matrix B
    float *host_matrixB = new float[order_n * order_n];
    std::cout << "Input matrixB (column major ordering, newline separated): ";
    for (size_t i = 0; i < order_n * order_n; i++)
    {
        *(host_matrixB + i) = read_number_from_stdin<float>("Invalid input - exiting");
    }
    // Allocating memory for matrixA, matrixB and result matrixC on GPU
    float *device_matrixA = nullptr, *device_matrixB = nullptr, *device_matrixC = nullptr;
    check_cuda(cudaMalloc((void **)&device_matrixA, matrix_bytes), "Allocating memory on GPU for matrixA failed.");
    check_cuda(cudaMalloc((void **)&device_matrixB, matrix_bytes), "Allocating memory on GPU for matrixB failed.");
    check_cuda(cudaMalloc((void **)&device_matrixC, matrix_bytes), "Allocating memory on GPU for matrixC failed.");
    // Copying matrixA and matrixB elements to GPU
    check_cuda(cudaMemcpy(device_matrixA, host_matrixA, matrix_bytes, cudaMemcpyHostToDevice), "Copying matrixA from host to GPU failed.");
    check_cuda(cudaMemcpy(device_matrixB, host_matrixB, matrix_bytes, cudaMemcpyHostToDevice), "Copying matrixB from host to GPU failed.");
    // Initialize cublas
    cublasHandle_t cublas_handle{};
    check_cublas(cublasCreate(&cublas_handle), "cuBLAS failed to initialize.");
    // Scalars that will be multiplied to matrixA and matrixB before addition on GPU. We will use additive identity (1.0).
    float alpha = 1.0, beta = 1.0;
    // matrixA + matrixB = matrixC
    check_cublas(cublasSgeam(cublas_handle, CUBLAS_OP_N, CUBLAS_OP_N, order_n, order_n, &alpha, device_matrixA, order_n, &beta, device_matrixB, order_n, device_matrixC, order_n), "geam error.");
    // Copying result from GPU to host
    float *host_matrixC = new float[order_n * order_n];
    check_cuda(cudaMemcpy(host_matrixC, device_matrixC, matrix_bytes, cudaMemcpyDeviceToHost), "Copying result to host failed.");
    // Printing result matrixC
    std::cout << "Result:" << std::endl;
    for (size_t i = 0; i < order_n; i++)
    {
        for (size_t j = 0; j < order_n * order_n; j += order_n)
        {
            std::cout << "\t" << *(host_matrixC + i + j);
        }
        std::cout << std::endl;
    }
    cudaFree(device_matrixA);
    cudaFree(device_matrixB);
    cudaFree(device_matrixC);
    cublasDestroy(cublas_handle);
    delete host_matrixA;
    delete host_matrixB;
    delete host_matrixC;
    return EXIT_SUCCESS;
}
/*
2
1
-1
2
3
4
1
0
5

*/