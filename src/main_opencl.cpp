#include <stdio.h>
#include <stdlib.h>
#define CL_TARGET_OPENCL_VERSION 120

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "headers/image.h"
 
#define MAX_SOURCE_SIZE (0x100000)
 
int main(int argc, char** argv) {

    #pragma region Loading variables
	
    printf("started running\n");
    int height = atoi(argv[4]);
    int width = atoi(argv[3]);
    int window_size = 5;
    size_t vector_size = sizeof(float) * width * height;
    float* inputPixels = (float*) malloc(vector_size);

    printf("%s\n", argv[1]);
    printf("%d %d\n", width, height);

    getImagePixelsFromRawFile_flat(inputPixels, argv[1], width, height);

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
    printf("kernel loading done\n");

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
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clGetDeviceIDs( platforms[1], CL_DEVICE_TYPE_ALL, 1, 
            &device_id, &ret_num_devices);
	printf("ret at %d is %d\n", __LINE__, ret);
    
    #pragma endregion

    #pragma region Creating context
    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
 	printf("ret at %d is %d\n", __LINE__, ret);

    #pragma endregion

    #pragma region Creating queue
    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
 	printf("ret at %d is %d\n", __LINE__, ret);

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
	printf("ret at %d is %d\n", __LINE__, ret);

    #pragma endregion
 
    #pragma region Creating program
	printf("before building\n");
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

    #pragma endregion
 
    #pragma region Building program
    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);
    #pragma endregion

    #pragma region Creating kernel
	printf("after building\n");
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "mean_filter", &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

    #pragma endregion
 
    #pragma region Setting arguments
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

    ret = clSetKernelArg(kernel, 2, sizeof(cl_int), (void *)&width);
	printf("ret at %d is %d\n", __LINE__, ret);

    ret = clSetKernelArg(kernel, 3, sizeof(cl_int), (void *)&height);
	printf("ret at %d is %d\n", __LINE__, ret);

    ret = clSetKernelArg(kernel, 4, sizeof(cl_int), (void *)&window_size);
	printf("ret at %d is %d\n", __LINE__, ret);

	//added this to fix garbage output problem
	//ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);

    #pragma endregion
 
    #pragma region Executing kernel
	printf("before execution\n");
    // Execute the OpenCL kernel on the list
    cl_event kernel_event;
    size_t global_item_size = width * height; // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, 
            &global_item_size, &local_item_size, 0, NULL, &kernel_event);

    ret = clWaitForEvents(1, &kernel_event);

    #pragma endregion

    #pragma region Timing event

    unsigned long start = 0;
    unsigned long end = 0;

    clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, 
        sizeof(cl_ulong), &start, NULL);

    clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, 
        sizeof(cl_ulong), &end, NULL);

    float duration = (end - start) * 10e-9;

    printf("Kernel execution time: %f\n", duration);
    
    #pragma endregion

    #pragma region Getting output
    printf("after execution\n");
    // Read the memory buffer C on the device to the local variable C
    float* outputPixels = (float*) malloc(vector_size);
    ret = clEnqueueReadBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            vector_size, outputPixels, 0, NULL, NULL);

    #pragma endregion

    #pragma region Displaying/saving result
//  printf("after copying\n");
//     // Display the result to the screen
//     for(i = 0; i < LIST_SIZE; i++)
//         printf("%d + %d = %d\n", A[i], B[i], C[i]);

    #pragma endregion

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
    free(outputPixels);

    #pragma endregion

    return 0;
}