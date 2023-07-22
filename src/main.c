#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#include "headers/image.h"
#include "headers/filter.h"

int main(int argc, char** argv)
{
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
    double start_time_omp = 0, end_time_omp = 0;


    #ifdef _OPENMP
        printf("OpenMP - Mean filter\n");
        for(int i = 0; i < count; i++) {
            int height = sizes[i][1];
            int width = sizes[i][0];
            float** inputPixels = allocImagePixels(width, height);
            // float** outputPixels = allocImagePixels(width, height);
            
            getImagePixelsFromRawFile(inputPixels, images[i], width, height);

            // run_mean_filter(inputPixels, outputPixels, images[i], width, height);
            for(int iteration = 0; iteration < iterations; iteration++)
            {
                start_time_omp = omp_get_wtime();
                // start_time = clock();
                meanFilter(inputPixels, width, height, window_size);
                // end_time = clock();
                end_time_omp = omp_get_wtime();
                cpu_time_used += (end_time_omp - start_time_omp);
                // printf("%f\n", cpu_time_used);
            }
            // cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            average_cpu_time_used = cpu_time_used / iterations;
            printf("%d x %d: %.6f seconds\n", width, height, average_cpu_time_used);

            // printImageInfo(inputPixels, width, height);
            freeImagePixels(inputPixels, width, height);
            // freeImagePixels(outputPixels, width, height);
        }
        printf("OpenMP - Median filter\n");
        for(int i = 0; i < count; i++) {
            int height = sizes[i][1];
            int width = sizes[i][0];
            float** inputPixels = allocImagePixels(width, height);
            // float** outputPixels = allocImagePixels(width, height);
            
            getImagePixelsFromRawFile(inputPixels, images[i], width, height);

            // run_mean_filter(inputPixels, outputPixels, images[i], width, height);
            for(int iteration = 0; iteration < iterations; iteration++)
            {
                start_time_omp = omp_get_wtime();
                // start_time = clock();
                medianFilter(inputPixels, width, height, window_size);
                // end_time = clock();
                end_time_omp = omp_get_wtime();
                cpu_time_used += (end_time_omp - start_time_omp);
                // printf("%f\n", cpu_time_used);
            }
            // cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            average_cpu_time_used = cpu_time_used / iterations;
            printf("%d x %d: %.6f seconds\n", width, height, average_cpu_time_used);

            // printImageInfo(inputPixels, width, height);
            freeImagePixels(inputPixels, width, height);
            // freeImagePixels(outputPixels, width, height);
        }
    #else
        printf("Sequencial - Mean filter\n");
        for(int i = 0; i < count; i++) {
            int height = sizes[i][1];
            int width = sizes[i][0];
            float** inputPixels = allocImagePixels(width, height);
            // float** outputPixels = allocImagePixels(width, height);
            
            getImagePixelsFromRawFile(inputPixels, images[i], width, height);

            start_time = clock();
            // run_mean_filter(inputPixels, outputPixels, images[i], width, height);
            for(int iteration = 0; iteration < iterations; iteration++)
                meanFilter(inputPixels, width, height, window_size);
            end_time = clock();

            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            average_cpu_time_used = cpu_time_used / iterations;
            printf("%d x %d: %.6f seconds\n", width, height, average_cpu_time_used);

            // printImageInfo(inputPixels, width, height);
            freeImagePixels(inputPixels, width, height);
            // freeImagePixels(outputPixels, width, height);
        }
        printf("Sequencial - Median filter\n");
        for(int i = 0; i < count; i++) {
            int height = sizes[i][1];
            int width = sizes[i][0];
            float** inputPixels = allocImagePixels(width, height);
            // float** outputPixels = allocImagePixels(width, height);
            
            getImagePixelsFromRawFile(inputPixels, images[i], width, height);

            start_time = clock();
            // run_mean_filter(inputPixels, outputPixels, images[i], width, height);
            for(int iteration = 0; iteration < iterations; iteration++)
                medianFilter(inputPixels, width, height, window_size);
            end_time = clock();

            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            average_cpu_time_used = cpu_time_used / iterations;
            printf("%d x %d: %.6f seconds\n", width, height, average_cpu_time_used);

            // printImageInfo(inputPixels, width, height);
            freeImagePixels(inputPixels, width, height);
            // freeImagePixels(outputPixels, width, height);
        }
    #endif

    return 0;
}