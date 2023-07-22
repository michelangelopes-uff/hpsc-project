#include "headers/image.h"

void getImagePixelsFromRawFile(float** pixels, const char* rawFilename, int width, int height) {
	FILE * file = fopen(rawFilename,"rb");
	unsigned char pixel_color;

	if (file) {
		for (int i = 0; i < height; i++){
			for (int j = 0; j< width; j++){
				if (fread(&pixel_color, sizeof(unsigned char), 1, file)!=EOF){
					pixels[i][j] = (float) pixel_color;
				} 
				else {
					// If reached EOF before expected, return false
					fclose(file);
				}
			}
		}

        fclose(file);
	}
}

void getImagePixelsFromRawFile_flat(float* pixels, const char* rawFilename, int width, int height) {
	FILE * file = fopen(rawFilename,"rb");
	unsigned char pixel_color;

	if (file) {
		for (int i = 0; i < height; i++){
			for (int j = 0; j< width; j++){
				if (fread(&pixel_color, sizeof(unsigned char), 1, file)!=EOF){
					int index = (i * width) + j;
					pixels[index] = (float) pixel_color;
				} 
				else {
					fclose(file);
					exit(0);
				}
			}
		}

        fclose(file);
	}
}

void setRawFileFromImagePixels(float** pixels, const char* rawFilename, int width, int height) {
	FILE* file = fopen(rawFilename, "wb");

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			unsigned char buffer = (unsigned char) (int) pixels[i][j];
			// printf("[%d][%d] = %f -> %hhu\n", i, j, pixels[i][j], buffer);
			fwrite(&buffer, sizeof(char), 1, file);
		}
	}

	fclose(file);
}

void setRawFileFromImagePixels_flat(float* pixels, const char* rawFilename, int width, int height) {
	FILE* file = fopen(rawFilename, "wb");

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			int index = (i * width) + j;
			unsigned char buffer = (unsigned char) (int) pixels[index];
			// printf("[%d][%d] = %f -> %hhu\n", i, j, pixels[index], buffer);
			fwrite(&buffer, sizeof(char), 1, file);
		}
	}

	fclose(file);
}

void printImageInfo(float** pixels, int width, int height) {
	printf("Dimensões: (x, y) = (%d, %d)\n\n", height, width);

	printf("Matriz:\n");

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			// printf("%f\t", pixels[i][j]);
			printf("[%d][%d] = %f\n", i, j, pixels[i][j]);
		}
	}
}

float** allocImagePixels(int width, int height) {
	float** pixels = (float**) malloc(height * sizeof(float*));

	for(int i = 0; i < height; i++) {
		pixels[i] = (float*) malloc(width * sizeof(float));
	}

	return pixels;
}

void freeImagePixels(float** pixels, int width, int height) {
	for(int i = 0; i < height; i++) {
		free(pixels[i]);
	}

	free(pixels);
}
