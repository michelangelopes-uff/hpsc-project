#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getImagePixelsFromRawFile(float** pixels, char* rawFilename, int width, int height);
void getImagePixelsFromRawFile_flat(float* pixels, char* rawFilename, int width, int height);
void setRawFileFromImagePixels(float** pixels, char* rawFilename, int width, int height);
void setRawFileFromImagePixels_flat(float* pixels, char* rawFilename, int width, int height);
void printImageInfo(float** pixels, int width, int height);
float** allocImagePixels(int width, int height);
void freeImagePixels(float** pixels, int width, int height);

#endif