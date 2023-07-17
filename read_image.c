#include "read_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setImageDimensionsFromFile(Image* image, char* nfFilename){
	FILE * file = fopen(nfFilename, "r");
	char str[1000];
	int z;

	if (file) {
	    while (fscanf(file, "%s", str)!=EOF){
			if (!strcmp(str,"\%image_dimensions")){
	        	fscanf(file, "%i %i %i", &image->x, &image->y, &z);
			}
		}

	    fclose(file);
	} 
	else
	{
		printf("O arquivo %s não foi encontrado.", nfFilename);
		exit(0);
	}
}

void generateImageFromArbitraryPixelsFromFile(Image* image, char* rawFilename){
	FILE * file = fopen(rawFilename,"rb");
	unsigned char pixel_color;

	if (file) {
		image->pixels = (unsigned char**) malloc(image->x * sizeof(unsigned char*));

		for(int i = 0; i < image->x; i++) {
			image->pixels[i] = (unsigned char*) malloc(image->y * sizeof(unsigned char));
		}

		for (int i = 0; i < image->x; i++){
			for (int j = 0; j< image->y; j++){
				if (fread(&pixel_color, sizeof(unsigned char), 1, file)!=EOF){
					printf("%d %d %d\n", i, j, pixel_color);
					image->pixels[i][j] = pixel_color;
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

void generateImageFromArbitraryPixels(Image* image) {
	FILE* file = fopen("teste4.raw", "wb");

	for(int i = 0; i < image->x; i++){
		for(int j = 0; j < image->y; j++){
			// char buffer = j + i * (image->x);
			unsigned char buffer = image->pixels[i][j];

			if(buffer > 100) {
				buffer = 0;
			}

			fwrite(&buffer, sizeof(char), 1, file);
		}
	}
}

void printImageInfo(Image* image) {
	printf("Dimensões: (x, y) = (%d, %d)\n\n", image->x, image->y);

	printf("Matriz:\n");

	for(int i = 0; i < image->x; i++){
		for(int j = 0; j < image->y; j++){
			printf("%d\t", image->pixels[i][j]);
		}

		printf("\n");
	}
}