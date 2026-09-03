#include <iostream>
#include <cuda_runtime.h>

__global__ void hello_from_gpu()
{
    printf("Hello World from GPU thread %d!\n", threadIdx.x);
}

int main()
{
    hello_from_gpu<<<1, 5>>>();
    cudaError_t launchError = cudaGetLastError(); // Checking launch configuration errors
    if (launchError != cudaSuccess)
    {
        std::cerr << "CUDA: Kernal launch failed: " << cudaGetErrorString(launchError) << std::endl;
        return EXIT_FAILURE;
    }
    cudaDeviceSynchronize();
    cudaError_t syncError = cudaGetLastError(); // Runtime errors inside the kernel
    if (syncError != cudaSuccess)
    {
        std::cerr << "CUDA: Kernal sync failed: " << cudaGetErrorString(syncError) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}