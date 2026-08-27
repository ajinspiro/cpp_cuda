#include <iostream>
#include <string>
#include <cuda_runtime.h>
#include <cublas_v2.h>

void check_cuda(cudaError_t cudaError, std::string message)
{
    if (cudaError == cudaSuccess)
    {
        return;
    }
    std::cout << "CUDA failed: " << message << std::endl;
    exit(EXIT_FAILURE);
}

void check_cublas(cublasStatus_t cublasStatus, std::string message)
{
    if (cublasStatus == CUBLAS_STATUS_SUCCESS)
    {
        return;
    }
    std::cout << "cuBLAS failed: " << message << std::endl;
    exit(EXIT_FAILURE);
}

const int n = 2; // dimention of square matrices to add

int main(int, char **)
{
    float host_matrixA[n * n] = {2, 0, 3, -1}; // column major ordering
    float host_matrixB[n * n] = {6, -4, 9, 1}; // column major ordering
    float *device_matrixA = nullptr, *device_matrixB = nullptr;
    check_cuda(cudaMalloc((void **)&device_matrixA, n * n * sizeof(float)), "Allocating memory for matrix A failed.");
    check_cuda(cudaMalloc((void **)&device_matrixB, n * n * sizeof(float)), "Allocating memory for matrix B failed.");
    check_cuda(cudaMemcpy(device_matrixA, host_matrixA, n * n * sizeof(float), cudaMemcpyHostToDevice), "Copying matrix A from host to device failed.");
    check_cuda(cudaMemcpy(device_matrixB, host_matrixB, n * n * sizeof(float), cudaMemcpyHostToDevice), "Copying matrix B from host to device failed.");
    cublasHandle_t cublas_handle = {};
    check_cublas(cublasCreate(&cublas_handle), "Handle creation failed.");
    float alpha = 1, beta = 1, *device_matrixC = nullptr;
    check_cuda(cudaMalloc((void **)&device_matrixC, n * n * sizeof(float)), "Allocating memory for device_matrixC failed.");
    check_cublas(cublasSgeam(cublas_handle, CUBLAS_OP_N, CUBLAS_OP_N, n, n, &alpha, device_matrixA, n, &beta, device_matrixB, n, device_matrixC, n), "geam failure.");
    float host_matrixC[n * n] = {};
    check_cuda(cudaMemcpy(host_matrixC, device_matrixC, n * n * sizeof(float), cudaMemcpyDeviceToHost), "Copying matrixC from device to host failed.");
    cudaFree(device_matrixA);
    cudaFree(device_matrixB);
    cudaFree(device_matrixC);
    cublasDestroy(cublas_handle);
    // TODO: print output. Correctness has been verified by using integrated debugger in vscode.
    return EXIT_SUCCESS;
}