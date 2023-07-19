#ifndef FILTER_H
#define FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} Point;

float euclideanDistance(int x1, int y1, int x2, int y2);
float computeWeight(float distance, float sigma);
void nlmFilter(float** image, int width, int height, float sigma, int window_size);
void swap(float* a, float* b);
void bubbleSort(float* arr, int size);
float median(float* arr, int size);
void medianFilter(float** image, int width, int height, int window_size);

#endif