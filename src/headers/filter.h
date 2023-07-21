#ifndef FILTER_H
#define FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int check_borders(int row, int column, int height, int width);
float computeWeight(float distance, float sigma);
float** nlmFilter(float** image, int width, int height, float sigma, int window_size, int patch_size);

void swap(float* a, float* b);
void bubbleSort(float* arr, int size);
float median(float* arr, int size);
float** medianFilter(float** image, int width, int height, int window_size);

float** meanFilter(float** image, int width, int height, int window_size);

#endif