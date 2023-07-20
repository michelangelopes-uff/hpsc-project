#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int height; //x
	int width; //y
	float** pixels;
}Image;

void getImagePixelsFromFile(Image* image, char* rawFilename);
void printImageInfo(Image* image);
void generateImageFromPixels(Image* image, char* filename);
void allocImagePixels(Image* image);
void freeImagePixels(Image* image);

#endif