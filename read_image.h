#ifndef READ_IMAGE_H
#define READ_IMAGE_H

typedef struct {
	int x;
	int y;
	unsigned char** pixels;
}Image;

void setImageDimensionsFromFile(Image* image, char* nfFilename);
void generateImageFromArbitraryPixelsFromFile(Image* image, char* rawFilename);
void printImageInfo(Image* image);
void generateImageFromArbitraryPixels(Image* image);

#endif