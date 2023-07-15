#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define KERNEL_SIZE 5
#define SIGMA 1.0

void gaussianFilter(float** image, int width, int height) {
    float kernel[KERNEL_SIZE][KERNEL_SIZE] = {
        { 1,  4,  7,  4,  1},
        { 4, 16, 26, 16,  4},
        { 7, 26, 41, 26,  7},
        { 4, 16, 26, 16,  4},
        { 1,  4,  7,  4,  1}
    };

    float sumKernel = 0.0;
    for (int i = 0; i < KERNEL_SIZE; i++) {
        for (int j = 0; j < KERNEL_SIZE; j++) {
            sumKernel += kernel[i][j];
        }
    }

    // Normalize the kernel
    for (int i = 0; i < KERNEL_SIZE; i++) {
        for (int j = 0; j < KERNEL_SIZE; j++) {
            kernel[i][j] /= sumKernel;
        }
    }

    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }

    int padding = KERNEL_SIZE / 2;

    // Apply the filter
    for (int i = padding; i < height - padding; i++) {
        for (int j = padding; j < width - padding; j++) {
            float sum = 0.0;
            for (int m = 0; m < KERNEL_SIZE; m++) {
                for (int n = 0; n < KERNEL_SIZE; n++) {
                    sum += kernel[m][n] * image[i + m - padding][j + n - padding];
                }
            }
            filteredImage[i][j] = sum;
        }
    }

    // Copy filtered image back to the original image
    for (int i = padding; i < height - padding; i++) {
        for (int j = padding; j < width - padding; j++) {
            image[i][j] = filteredImage[i][j];
        }
    }

    // Free memory
    for (int i = 0; i < height; i++) {
        free(filteredImage[i]);
    }
    free(filteredImage);
}

int main() {
    // Example usage
    int width = 10;
    int height = 10;

    float** image = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        image[i] = (float*)malloc(width * sizeof(float));
        for (int j = 0; j < width; j++) {
            image[i][j] = i + j;  // Example input image
        }
    }

    gaussianFilter(image, width, height);

    // Display filtered image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%.2f ", image[i][j]);
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);

    return 0;
}
