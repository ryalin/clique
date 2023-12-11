#include <map>
#include <set>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define BLOCKDIMX 32
#define BLOCKDIMY 32
#define THREADCOUNT 1024

__global__ void kernelCliqueSolver() {

}

bool parallelCuda(std::map<int,std::set<int>> graph, int targetCount) {
  dim3 blockDim(BLOCKDIMX, BLOCKDIMY);
  dim3 gridDim((image->height + BLOCKDIMX - 1) / BLOCKDIMX,
                (image->width + BLOCKDIMY - 1) / BLOCKDIMY);

    float* device_x;
    float* device_y;
    float* device_result;

    // allocate device memory buffers on the GPU using cudaMalloc
    cudaMalloc(&device_x, sizeof(float) * N);
    cudaMalloc(&device_y, sizeof(float) * N);
    cudaMalloc(&device_result, sizeof(float) * N);

    // start timing after allocation of device memory
    double startTime = CycleTimer::currentSeconds();

    // copy input arrays to the GPU using cudaMemcpy
    cudaMemcpy(device_x, xarray, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(device_y, yarray, sizeof(float) * N, cudaMemcpyHostToDevice);

  kernelRenderCircles<<<gridDim, blockDim>>>();
  cudaDeviceSynchronize();
  return true;
}