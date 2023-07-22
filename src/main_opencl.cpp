#define CL_TARGET_OPENCL_VERSION 120
#define MAX_SOURCE_SIZE (0x100000)

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#include "headers/image.h"
 
int main(int argc, char** argv) {

    int count = 7;
    int dimensions = 2;

    const char* images[count] = {
        "./data/raw_input/00_random_10x10.raw",
        "./data/raw_input/01_image_1857x1317.raw",
        "./data/raw_input/02_image_1600x900.raw",
        "./data/raw_input/03_image_640x480.raw",
        "./data/raw_input/04_image_790x656.raw",
        "./data/raw_input/05_image_512x512.raw",
        "./data/raw_input/06_image_299x168.raw"
    };

    int sizes[count][dimensions] = {
        {10, 10},
        {1857, 1357},
        {1600, 900},
        {640, 480},
        {790, 656},
        {512, 512},
        {299, 168}
    };

    int iterations = 100;
    int window_size = 5;

    clock_t start_time, end_time;
    double cpu_time_used, average_cpu_time_used;

    for(int i = 0; i < count; i++) {

        #pragma region Loading variables
        // int i = 5;
        // printf("started running\n");
        int height = sizes[i][1];
        int width = sizes[i][0];
        size_t vector_size = sizeof(float) * width * height;
        float* inputPixels = (float*) malloc(vector_size);

        getImagePixelsFromRawFile_flat(inputPixels, images[i], width, height);

        #pragma endregion

        #pragma region Loading kernel
        // Load the kernel source code into the array source_str
        FILE *fp;
        char *source_str;
        size_t source_size;

        // fp = fopen("kernels/vector_add_kernel.cl", "r");
        fp = fopen("kernels/mean_filter.cl", "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        source_str = (char*)malloc(MAX_SOURCE_SIZE);
        source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose( fp );
        // printf("kernel loading done\n");

        #pragma endregion

        #pragma region Loading device
        // Get platform and device information
        cl_device_id device_id = NULL;   
        cl_uint ret_num_devices;
        cl_uint ret_num_platforms;
        
        cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
        cl_platform_id *platforms = NULL;
        platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

        ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
        // printf("ret at %d is %d\n", __LINE__, ret);

        ret = clGetDeviceIDs( platforms[1], CL_DEVICE_TYPE_ALL, 1, 
                &device_id, &ret_num_devices);
        // printf("ret at %d is %d\n", __LINE__, ret);
        
        #pragma endregion

        #pragma region Creating context
        // Create an OpenCL context
        cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
        // printf("ret at %d is %d\n", __LINE__, ret);

        #pragma endregion

        #pragma region Creating queue
        // Create a command queue
        cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
        // printf("ret at %d is %d\n", __LINE__, ret);

        #pragma endregion

        #pragma region Handling buffers
        // Create memory buffers on the device for each vector 
        cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
                vector_size, NULL, &ret);
        cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                vector_size, NULL, &ret);

        // Copy the lists A and B to their respective memory buffers
        ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
                vector_size, inputPixels, 0, NULL, NULL);
        // printf("ret at %d is %d\n", __LINE__, ret);

        #pragma endregion

        #pragma region Creating program
        // printf("before building\n");
        // Create a program from the kernel source
        cl_program program = clCreateProgramWithSource(context, 1, 
                (const char **)&source_str, (const size_t *)&source_size, &ret);
        // printf("ret at %d is %d\n", __LINE__, ret);

        #pragma endregion

        #pragma region Building program
        // Build the program
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        // printf("ret at %d is %d\n", __LINE__, ret);
        #pragma endregion

        #pragma region Creating kernel
        // printf("after building\n");
        // Create the OpenCL kernel
        cl_kernel kernel = clCreateKernel(program, "mean_filter", &ret);
        // printf("ret at %d is %d\n", __LINE__, ret);

        #pragma endregion

        #pragma region Setting arguments
        // Set the arguments of the kernel
        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
        // printf("ret at %d is %d\n", __LINE__, ret);

        ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
        // printf("ret at %d is %d\n", __LINE__, ret);

        ret = clSetKernelArg(kernel, 2, sizeof(cl_int), (void *)&width);
        // printf("ret at %d is %d\n", __LINE__, ret);

        ret = clSetKernelArg(kernel, 3, sizeof(cl_int), (void *)&height);
        // printf("ret at %d is %d\n", __LINE__, ret);

        ret = clSetKernelArg(kernel, 4, sizeof(cl_int), (void *)&window_size);
        // printf("ret at %d is %d\n", __LINE__, ret);

        //added this to fix garbage output problem
        //ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);

        #pragma endregion

        #pragma region Executing kernel
        // printf("before execution\n");
        // Execute the OpenCL kernel on the list
        // int local_width = width / 24; 

        // Assuming the global matrix size is 1857 x 1317
        size_t globalRows = width;
        size_t globalCols = height;

        // Get the maximum work group size supported by your GPU
        size_t maxWorkGroupSize;
        clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);

        // Calculate a reasonable work group size that evenly divides the matrix dimensions
        size_t workGroupSize = maxWorkGroupSize;
        while (globalRows % workGroupSize != 0) {
            workGroupSize--;
        }

        // Calculate the number of work groups in each dimension
        size_t numWorkGroupsX = globalRows / workGroupSize;
        size_t numWorkGroupsY = globalCols / workGroupSize;

        // Set the global and local sizes for your OpenCL kernel
        size_t globalSize[2] = {globalRows, globalCols};
        size_t localSize[2];

        cl_event kernel_event;

        size_t adjustWorkGroupSize = workGroupSize * 2;

        start_time = clock();
        do {
            adjustWorkGroupSize /= 2;
            localSize[0] = adjustWorkGroupSize;
            localSize[1] = adjustWorkGroupSize;
            // printf("%d %d\n", (int) globalSize[0], (int) globalSize[1]);
            // printf("%d %d\n", (int) localSize[0], (int) localSize[1]);
            
            ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, 
                    globalSize, localSize, 0, NULL, &kernel_event);
            // printf("ret at %d is %d\n", __LINE__, ret);

        } while(ret < 0);

        end_time = clock();
        cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        printf("%d x %d: %.6f seconds\n", width, height, cpu_time_used);

        ret = clWaitForEvents(1, &kernel_event);

        #pragma endregion

        #pragma region Timing event

        unsigned long start = 0;
        unsigned long end = 0;

        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, 
            sizeof(cl_ulong), &start, NULL);

        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, 
            sizeof(cl_ulong), &end, NULL);

        double duration = (end - start);

        // printf("Kernel execution time: %.20lf %d %d\n", duration, start, end);
        
        #pragma endregion

        // #pragma region Getting output
        printf("after execution\n");
        // // Read the memory buffer C on the device to the local variable C
        // float* outputPixels = (float*) malloc(vector_size);
        // ret = clEnqueueReadBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
        //         vector_size, outputPixels, 0, NULL, NULL);

        // #pragma endregion

        #pragma region Cleaning
        // Clean up
        ret = clFlush(command_queue);
        ret = clFinish(command_queue);
        ret = clReleaseEvent(kernel_event);
        ret = clReleaseKernel(kernel);
        ret = clReleaseProgram(program);
        ret = clReleaseMemObject(a_mem_obj);
        ret = clReleaseMemObject(b_mem_obj);
        ret = clReleaseCommandQueue(command_queue);
        ret = clReleaseContext(context);
        free(inputPixels);
        // free(outputPixels);

        #pragma endregion
    }

    return 0;
}