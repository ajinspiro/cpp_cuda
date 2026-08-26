#include <iostream>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#define IDX2C(i, j, ld) ((j) * (ld) + (i)) // indexing for column major ordering

// INCOMPLETE -- dont run this program
int main(int, char **)
{
    float matrix_A[2][2] = {{1, -1}, {2, -1}}, matrix_B[2][1] = {{-1}, {0}}, *matrixA_ptr = new float[4], *matrixB_ptr = new float[2];
    // populate matrixA_ptr in column major index
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            matrixA_ptr[IDX2C(i, j, 2)] = matrix_A[i][j];
        }
    }

    // populate matrixB_ptr in column major index
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 1; j++)
        {
            matrixB_ptr[IDX2C(i, j, 2)] = matrix_B[i][j];
        }
    }

    return EXIT_SUCCESS;
    cudaError_t res_1 = cudaMalloc(&matrixA_ptr, 4 * sizeof(float));
    if (res_1 != cudaSuccess)
    {
        std::cout << "CUDA_cuBLAS_SolveMatrix: Allocating matrixA in device memory failed. Exiting." << std::endl;
        free(matrixA_ptr);
        free(matrixB_ptr);
        return EXIT_FAILURE;
    }
    cudaError_t res_2 = cudaMalloc(&matrixB_ptr, 2 * sizeof(float));
    if (res_2 != cudaSuccess)
    {
        std::cout << "CUDA_cuBLAS_SolveMatrix: Allocating matrixB in device memory failed. Exiting." << std::endl;
        free(matrixA_ptr);
        free(matrixB_ptr);
        return EXIT_FAILURE;
    }
    cublasHandle_t cublas_handle = nullptr;
    cublasStatus_t cublas_status = cublasCreate(&cublas_handle);
    if(cublas_status != CUBLAS_STATUS_SUCCESS){
        std::cout << "CUDA_cuBLAS_SolveMatrix: CUBLAS initialization failed. Exiting." << std::endl;
        free(matrix_A);
        free(matrix_B);
        cudaFree(matrixA_ptr);
        cudaFree(matrixB_ptr);
        return EXIT_FAILURE;
    }
    
    // cublasSetMatrix(2, 2, sizeof(int), )

    // incomplete
}