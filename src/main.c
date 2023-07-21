#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "headers/image.h"
#include "headers/filter.h"

void run_mean_filter(float** inputPixels, float** outputPixels, char* rawFilename, int width, int height) {
    int window_size = 1;
    
    for(int i = 5; i <= 15; i += 2) {
        window_size = i;

        char newImageFilename[100] = "";
        sprintf(newImageFilename, "%s_mean%d.raw", rawFilename, window_size);
        printf("%s\n", newImageFilename);
        outputPixels = meanFilter(inputPixels, width, height, window_size);
        setRawFileFromImagePixels(outputPixels, newImageFilename, width, height);
        freeImagePixels(outputPixels, width, height);
    }
}

void run_median_filter(float** inputPixels, float** outputPixels, char* rawFilename, int width, int height) {
    int window_size = 1;
    
    for(int i = 5; i <= 15; i += 2) {
        window_size = i;

        char newImageFilename[100] = "";
        sprintf(newImageFilename, "%s_median%d.raw", rawFilename, window_size);
        printf("%s\n", newImageFilename);
        outputPixels = medianFilter(inputPixels, width, height, window_size);
        setRawFileFromImagePixels(outputPixels, newImageFilename, width, height);
        freeImagePixels(outputPixels, width, height);
    }
}

void run_nlm_filter(float** inputPixels, float** outputPixels, char* rawFilename, int width, int height) {
    float sigma = 0;
    int window_size = 1;
    int patch_size = 1;
    
    int max = 5;
    int start = 5;

    for(int i = start; i <= max; i += 2) {
        patch_size = i;
        window_size = i + 2;

        for(int j = start; j <= max; j++) {
            // sigma = j * 0.1;
            sigma = 0.5;
            
            char newImageFilename[100] = "";
            sprintf(newImageFilename, "%s_nlm%d-%d.raw", rawFilename, j, window_size);
            printf("%s\n", newImageFilename);
            outputPixels = nlmFilter(inputPixels, width, height, sigma, window_size, patch_size);
            setRawFileFromImagePixels(outputPixels, newImageFilename, width, height);
            freeImagePixels(outputPixels, width, height);
        }
    }
}

int main(int argc, char** argv)
{
    int height = atoi(argv[4]);
    int width = atoi(argv[3]);
    float** inputPixels = allocImagePixels(width, height);
    float** outputPixels = allocImagePixels(width, height);

    printf("%s\n", argv[1]);
    printf("%d %d\n", width, height);

    getImagePixelsFromRawFile(inputPixels, argv[1], width, height);

    int window_size = atoi(argv[5]);

    char newImageFilename[100] = "";
    sprintf(newImageFilename, "%s_mean%d.raw", argv[2], window_size);
    printf("%s\n", newImageFilename);
    outputPixels = meanFilter(inputPixels, width, height, window_size);
    setRawFileFromImagePixels(outputPixels, newImageFilename, width, height);
    freeImagePixels(outputPixels, width, height);

    // run_mean_filter(inputPixels, outputPixels, argv[2], width, height);
    // run_median_filter(inputPixels, outputPixels, argv[2], width, height);
    // run_nlm_filter(inputPixels, outputPixels, argv[2], width, height);

    return 0;
}