#include "headers/filter.h"

float euclideanDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float computeWeight(float distance, float sigma) {
    return exp(-distance / (sigma * sigma));
}

void nlmFilter(float** image, int width, int height, float sigma, int window_size) {
    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float sumWeights = 0.0;
            float sumValues = 0.0;

            for (int m = -window_size; m <= window_size; m++) {
                for (int n = -window_size; n <= window_size; n++) {
                    int x = i + m;
                    int y = j + n;

                    if (x >= 0 && x < height && y >= 0 && y < width) {
                        float distance = euclideanDistance(i, j, x, y);
                        float weight = computeWeight(distance, sigma);
                        sumWeights += weight;
                        sumValues += weight * image[x][y];
                    }
                }
            }

            filteredImage[i][j] = sumValues / sumWeights;
        }
    }

    // Copy filtered image back to the original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = filteredImage[i][j];
        }
    }

    // Free memory
    for (int i = 0; i < height; i++) {
        free(filteredImage[i]);
    }
    free(filteredImage);
}

void swap(float* a, float* b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(float* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

float median(float* arr, int size) {
    bubbleSort(arr, size);
    return arr[size / 2];
}

void medianFilter(float** image, int width, int height, int window_size) {
    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }

    float window[window_size * window_size];

    // #pragma omp parallel for collapse(2) shared(image) private(window)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int k = 0;
            for (int m = -window_size / 2; m <= window_size / 2; m++) {
                for (int n = -window_size / 2; n <= window_size / 2; n++) {
                    int x = i + m;
                    int y = j + n;

                    if (x >= 0 && x < height && y >= 0 && y < width) {
                        window[k++] = image[x][y];
                    }
                }
            }
            // printf("TESTE1");
            filteredImage[i][j] = median(window, k);
            // printf("TESTE2");
        }
    }

    // Copy filtered image back to the original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = filteredImage[i][j];
        }
    }

    // Free memory
    for (int i = 0; i < height; i++) {
        free(filteredImage[i]);
    }
    free(filteredImage);
}
