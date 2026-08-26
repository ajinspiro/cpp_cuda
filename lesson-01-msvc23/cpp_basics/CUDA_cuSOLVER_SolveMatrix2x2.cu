#include <iostream>
#include <cuda_runtime.h>
#include <cusolverDn.h>

void check_cuda(cudaError_t cudaError, std::string message)
{
    if (cudaError == cudaSuccess)
    {
        return;
    }
    std::cout << "CUDA failed: " << message << std::endl;
    exit(EXIT_FAILURE);
}

void check_cusolver(cusolverStatus_t cusolverStatus, std::string message)
{
    if (cusolverStatus == CUSOLVER_STATUS_SUCCESS)
    {
        return;
    }
    std::cout << "cuSOLVER failed: " << message << std::endl;
    exit(EXIT_FAILURE);
}

int main(int, char **)
{
    // -------------------------------------------------------------
    // System we are solving:
    //   2x + 1y = 5
    //   1x + 3y = 10
    // Column-major A: [2, 1, 1, 3]
    // -------------------------------------------------------------
    const int n = 2;
    float host_matrixA[n * n] = {2.0f, 1.0f,
                                 1.0f, 3.0f};
    float host_matrixB[n] = {5.0f, 10.0f};

    cusolverDnHandle_t cusolver_handle;
    check_cusolver(cusolverDnCreate(&cusolver_handle), "Handle creation failed.");

    // -------------------------------------------------------------
    // Device buffers -- note these are all PLAIN pointers, no
    // pointer-to-pointer "array of matrices" needed.
    // -------------------------------------------------------------
    float *device_matrixA = nullptr, *device_matrixB = nullptr;
    int *device_pivot = nullptr, *device_info = nullptr;
    check_cuda(cudaMalloc((void **)&device_matrixA, n * n * sizeof(float)), "Allocating matrixA on GPU failed.");
    check_cuda(cudaMalloc((void **)&device_matrixB, n * sizeof(float)), "Allocating matrixB on GPU failed.");
    check_cuda(cudaMalloc((void **)&device_pivot, n * sizeof(int)), "Allocating device_pivot on GPU failed.");
    check_cuda(cudaMalloc((void **)&device_info, sizeof(int)), "Allocating device_info on GPU failed.");

    check_cuda(cudaMemcpy(device_matrixA, host_matrixA, n * n * sizeof(float), cudaMemcpyHostToDevice), "Copying matrixA to GPU failed.");
    check_cuda(cudaMemcpy(device_matrixB, host_matrixB, n * sizeof(float), cudaMemcpyHostToDevice), "Copying matrixB to GPU failed.");

    // -------------------------------------------------------------
    // cuSOLVER's factorization needs a pre-allocated scratch
    // ("workspace") buffer. You first ask it how big that buffer
    // needs to be, then allocate exactly that much.
    // -------------------------------------------------------------
    int cusolver_getrf_buffersize = 0;
    // lda = leading dimension of A
    // Column-major storage means each column is stored contiguously.
    // lda tells the library how many elements to skip in memory to get
    // from the start of one column to the start of the next column — i.e.,
    // the physical stride between columns, which is not always the same
    // as the matrix's logical row count.
    check_cusolver(
        cusolverDnSgetrf_bufferSize(cusolver_handle, n, n, device_matrixA, n, &cusolver_getrf_buffersize),
        "Failed to get getrif buffersize.");
    float *device_workspace = nullptr;
    check_cuda(
        cudaMalloc((void **)&device_workspace, cusolver_getrf_buffersize * sizeof(float)),
        "cuSOLVER getrf workspace calculation failed.");
    // -------------------------------------------------------------
    // Step 1: LU-factorize A in place (A -> P,L,U)
    // -------------------------------------------------------------
    check_cusolver(
        cusolverDnSgetrf(cusolver_handle, n, n, device_matrixA, n, device_workspace, device_pivot, device_info),
        "cusolver getrf failed.");

    int host_info = 0;
    check_cuda(
        cudaMemcpy(&host_info, device_info, sizeof(int), cudaMemcpyDeviceToHost),
        "Failed to copy device_info to host_info. (cusolverDnSgetrf)");
    if (host_info != 0)
    {
        std::cout << "LU factorization failed, info = " << host_info << std::endl;
        exit(EXIT_FAILURE);
    }
    // -------------------------------------------------------------
    // Step 2: Solve directly for X using the LU factors -- no
    // explicit A^-1 is ever computed. d_B is overwritten with X.
    // -------------------------------------------------------------
    check_cusolver(
        cusolverDnSgetrs(
            cusolver_handle,
            CUBLAS_OP_N, // no transpose
            n,           // nrhs: number of right-hand-side columns (B is a single column here)
            1,
            device_matrixA, n, // the LU-factored matrix
            device_pivot,
            device_matrixB, // in: B, out: X (overwritten in place)
            n,
            device_info),
        "cuSOLVER failed to calculate solution for the system.");
    check_cuda(
        cudaMemcpy(&host_info, device_info, sizeof(int), cudaMemcpyDeviceToHost),
        "Failed to copy device_info to host_info. (cusolverDnSgetrs)");
    if (host_info != 0)
    {
        std::cout << "Solve failed, info = " << host_info << std::endl;
        exit(EXIT_FAILURE);
    }
    // -------------------------------------------------------------
    // d_B now holds X. Copy back and print.
    // -------------------------------------------------------------
    float host_matrixS[n] = {};
    check_cuda(
        cudaMemcpy(&host_matrixS, device_matrixB, n * sizeof(float), cudaMemcpyDeviceToHost),
        "Copying solution to host failed");
    printf("Solving:\n");
    printf("  %.1fx + %.1fy = %.1f\n", host_matrixA[0], host_matrixA[2], host_matrixB[0]);
    printf("  %.1fx + %.1fy = %.1f\n", host_matrixA[1], host_matrixA[3], host_matrixB[1]);
    printf("\nX = solve(A, B) via LU factorization (no explicit inverse):\n");
    printf("  x = %f\n", host_matrixS[0]);
    printf("  y = %f\n", host_matrixS[1]);

    // -------------------------------------------------------------
    // Cleanup
    // -------------------------------------------------------------
    cudaFree(device_matrixA);
    cudaFree(device_matrixB);
    cudaFree(device_pivot);
    cudaFree(device_info);
    cudaFree(device_workspace);
    cusolverDnDestroy(cusolver_handle);

    return EXIT_SUCCESS;
}