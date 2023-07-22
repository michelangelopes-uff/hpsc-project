#include <stdio.h>
#include <stdlib.h>
#define CL_TARGET_OPENCL_VERSION 120

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MAX_SOURCE_SIZE (0x100000)
 
int main(int argc, char** argv) {

    #pragma region Starting

    cl_platform_id platforms[10];
    cl_uint numPlatforms;
    clGetPlatformIDs(10, platforms, &numPlatforms);

    printf("Number of OpenCL Platforms: %u\n", numPlatforms);

    for (cl_uint i = 0; i < numPlatforms; i++) {
        cl_uint numDevices;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 10, NULL, &numDevices);

        cl_device_id devices[10];
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

        printf("Devices on Platform %u:\n", i + 1);

        for (cl_uint j = 0; j < numDevices; j++) {
            char deviceName[128];
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
            printf("Device %u: %s\n", j + 1, deviceName);

            char deviceDriverVersion[128];
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, sizeof(deviceDriverVersion), deviceDriverVersion, NULL);
            printf("Driver Version: %s\n", deviceDriverVersion);

            char deviceOpenCLVersion[128];
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, sizeof(deviceOpenCLVersion), deviceOpenCLVersion, NULL);
            printf("OpenCL Version: %s\n", deviceOpenCLVersion);

            cl_uint computeUnits;
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &computeUnits, NULL);
            printf("Compute Units: %u\n", computeUnits);

            cl_ulong globalCacheSize;
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &globalCacheSize, NULL);
            printf("Global Memory Cache Size: %lu bytes\n", globalCacheSize);

            cl_uint cacheLineSize;
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint), &cacheLineSize, NULL);
            printf("Global Memory Cache Line Size: %u bytes\n", cacheLineSize);

            cl_ulong localMemSize;
            clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &localMemSize, NULL);
            printf("Local Memory Size: %lu bytes\n", localMemSize);
        }
    }

    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    size_t maxWorkGroupSize;
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);

    size_t maxWorkItemDimensions;
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(size_t), &maxWorkItemDimensions, NULL);

    size_t* maxWorkItemSizes = (size_t*)malloc(maxWorkItemDimensions * sizeof(size_t));
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, maxWorkItemDimensions * sizeof(size_t), maxWorkItemSizes, NULL);

    printf("Maximum Work Group Size: %zu\n", maxWorkGroupSize);
    printf("Maximum Work Item Dimensions: %zu\n", maxWorkItemDimensions);
    printf("Maximum Work Item Sizes: ");

    for (size_t i = 0; i < maxWorkItemDimensions; i++) {
        printf("%zu ", maxWorkItemSizes[i]);
    }
    printf("\n");

    free(maxWorkItemSizes);

    #pragma endregion
}