#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_SIZE 5
#define PATCH_SIZE 3
#define H 0.8

typedef struct {
    int x;
    int y;
} Point;

float euclideanDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float computeWeight(float distance) {
    return exp(-distance / (H * H));
}

void nlmFilter(float** image, int width, int height) {
    float** filteredImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        filteredImage[i] = (float*)malloc(width * sizeof(float));
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float sumWeights = 0.0;
            float sumValues = 0.0;

            for (int m = -WINDOW_SIZE; m <= WINDOW_SIZE; m++) {
                for (int n = -WINDOW_SIZE; n <= WINDOW_SIZE; n++) {
                    int x = i + m;
                    int y = j + n;

                    if (x >= 0 && x < height && y >= 0 && y < width) {
                        float distance = euclideanDistance(i, j, x, y);
                        float weight = computeWeight(distance);
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

    nlmFilter(image, width, height);

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
