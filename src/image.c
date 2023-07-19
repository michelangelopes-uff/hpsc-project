#include "headers/image.h"

void setImageDimensionsFromFile(Image* image, char* nfFilename){
	FILE * file = fopen(nfFilename, "r");
	char str[1000];
	int z;

	if (!file) {
		printf("O arquivo %s não foi encontrado.", nfFilename);
		exit(0);
	}

	while (fscanf(file, "%s", str)!=EOF){
		if (!strcmp(str,"\%image_dimensions")){
			fscanf(file, "%i %i %i", &image->height, &image->width, &z);
		}
	}

	fclose(file);
}

void getImagePixelsFromFile(Image* image, char* rawFilename){
	FILE * file = fopen(rawFilename,"rb");
	unsigned char pixel_color;

	if (file) {
		allocImagePixels(image);

		for (int i = 0; i < image->height; i++){
			for (int j = 0; j< image->width; j++){
				if (fread(&pixel_color, sizeof(unsigned char), 1, file)!=EOF){
					// printf("%d %d %d\n", i, j, pixel_color);
					image->pixels[i][j] = (float) pixel_color;
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

void generateImageFromPixels(Image* image, char* filename) {
	FILE* file = fopen(filename, "wb");

	for(int i = 0; i < image->height; i++){
		for(int j = 0; j < image->width; j++){
			unsigned char buffer = (unsigned char) (int) image->pixels[i][j];
			fwrite(&buffer, sizeof(char), 1, file);
		}
	}

	fclose(file);
}

void printImageInfo(Image* image) {
	printf("Dimensões: (x, y) = (%d, %d)\n\n", image->height, image->width);

	printf("Matriz:\n");

	for(int i = 0; i < image->height; i++){
		for(int j = 0; j < image->width; j++){
			printf("%f\t", image->pixels[i][j]);
		}

		printf("\n");
	}
}

void allocImagePixels(Image* image) {
	image->pixels = (float**) malloc(image->height * sizeof(float*));

	for(int i = 0; i < image->height; i++) {
		image->pixels[i] = (float*) malloc(image->width * sizeof(float));
	}
}

void freeImagePixels(Image* image) {
	for(int i = 0; i < image->height; i++) {
		free(image->pixels[i]);
	}

	free(image->pixels);
}
