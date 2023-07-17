#include <stdio.h>
#include <stdlib.h>

#define WINDOW_SIZE 3

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

int median(int* arr, int size) {
    bubbleSort(arr, size);
    return arr[size / 2];
}

void medianFilter(int** image, int width, int height) {
    int** filteredImage = (int**)malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (int*)malloc(width * sizeof(int));
    }

    int window[WINDOW_SIZE * WINDOW_SIZE];

    #pragma omp parallel for collapse(2) shared(image) private(window)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int k = 0;
            for (int m = -WINDOW_SIZE / 2; m <= WINDOW_SIZE / 2; m++) {
                for (int n = -WINDOW_SIZE / 2; n <= WINDOW_SIZE / 2; n++) {
                    int x = i + m;
                    int y = j + n;

                    if (x >= 0 && x < height && y >= 0 && y < width) {
                        window[k++] = image[x][y];
                    }
                }
            }

            filteredImage[i][j] = median(window, k);
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

int main() {
    // Example usage
    int width = 10;
    int height = 10;

    int** image = (int**)malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        image[i] = (int*)malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) {
            image[i][j] = i + j;  // Example input image
        }
    }

    medianFilter(image, width, height);

    // Display filtered image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", image[i][j]);
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
