#include "headers/filter.h"

float computeWeight(float distance, float sigma) {
    // printf("exp(%f)\n", -distance / (sigma * sigma * 100));
    return exp(-distance / (sigma * sigma));
}

int check_borders(int row, int column, int height, int width) {
    if(row >= 0 && row < height && column >= 0 && column < width)
        return 1;

    return 0;
}

float** nlmFilter(float** image, int width, int height, float sigma, int window_size, int patch_size) {
    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }
    // printf("window = %d --- patch = %d\n", window_size, patch_size);
    int aux;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // printf("[i] = %d --- [j] = %d\n", i, j);
            float sumWeights = 0.0;
            float sumValues = 0.0;

            for (int m = -window_size/2; m <= window_size/2; m++) {
                for (int n = -window_size/2; n <= window_size/2; n++) {
                    int x = i + m;
                    int y = j + n;
                    if (check_borders(x, y, height, width) && x != i && y != j) {
                        // printf("[m] = %d --- [n] = %d\n", m, n);
                        // printf("[x] = %d --- [y] = %d\n", x, y);
                        // printf("x and y are OK!\n");
                        float distance = 0.0;

                        // squaredEuclideanDistance
                        for (int k = 0; k < patch_size; k++) {
                            for (int l = 0; l < patch_size; l++) {
                                int new_i = i + k;
                                int new_j = j + l;
                                int new_x = x + k;
                                int new_y = y + l;

                                // printf("[new_i] = %d --- [new_j] = %d === %f\n", new_i, new_j, image[new_i][new_j]);
                                // printf("[new_x] = %d --- [new_y] = %d === %f\n", new_x, new_y, image[new_x][new_y]);
                                if(check_borders(new_i, new_j, height, width) && check_borders(new_x, new_y, height, width)){
                                    float diff = image[new_i][new_j] - image[new_x][new_y];
                                    distance += diff * diff;
                                }
                                else {
                                    // printf("Out of border\n");
                                }
                                // printf("---\n");
                            }
                        }

                        // distance = sqrt(distance);
                        // printf("distance = %f\n", distance);
                        // squaredEuclideanDistance(&image[i][j], &image[x][y], patch_size);
                        float weight = computeWeight(distance, sigma);

                        printf("weight = %.20f\n", weight);
                        if(weight - 0.000001 > 0.0){
                            sumWeights += weight;
                            sumValues += weight * image[x][y];
                            printf("sumWeights = %f\n", sumWeights);
                            printf("sumValues = %f\n", sumValues);
                            // printf("-------------------\n");
                        }
                    }
                }
            }
            if(sumValues == 0 || sumWeights == 0)
                filteredImage[i][j] = image[i][j];
            else {
                filteredImage[i][j] = sumValues / sumWeights;
                printf("original = %f --- filtered = %f\n", image[i][j], filteredImage[i][j]);
            }
        }
    }

    return filteredImage;
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

float** medianFilter(float** image, int width, int height, int window_size) {
    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }

    float window[window_size * window_size];

    #pragma omp parallel for collapse(2) shared(image) private(window)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // printf("[i] = %d --- [j] = %d\n", i, j);
            int k = 0;
            for (int m = -window_size / 2; m <= window_size / 2; m++) {
                for (int n = -window_size / 2; n <= window_size / 2; n++) {
                    int x = i + m;
                    int y = j + n;

                    if (x >= 0 && x < height && y >= 0 && y < width) {
                        // printf("[x] = %d --- [y] = %d\n", x, y);
                        // printf("x and y are OK!\n");
                        window[k] = image[x][y];
                        k++;
                    }
                }
            }

            filteredImage[i][j] = median(window, k);
            // printf("original = %f --- filtered = %f\n", image[i][j], filteredImage[i][j]);
        }
    }

    return filteredImage;
}

float** meanFilter(float** image, int width, int height, int window_size) {
    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }

    float window[window_size * window_size];

    #pragma omp parallel for collapse(2) shared(image) private(window)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // printf("[i] = %d --- [j] = %d\n", i, j);
            // int k = 0;
            float sum = 0.0;
            for (int m = -window_size / 2; m <= window_size / 2; m++) {
                for (int n = -window_size / 2; n <= window_size / 2; n++) {
                    int x = i + m;
                    int y = j + n;

                    if (x >= 0 && x < height && y >= 0 && y < width) {
                        // printf("[x] = %d --- [y] = %d\n", x, y);
                        // printf("x and y are OK!\n");
                        sum += image[x][y];
                        // k++;
                    }
                }
            }

            filteredImage[i][j] = sum / (window_size * window_size);
            // printf("original = %f --- filtered = %f\n", image[i][j], filteredImage[i][j]);
        }
    }

    return filteredImage;
}