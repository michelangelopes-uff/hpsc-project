#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "headers/image.h"
#include "headers/filter.h"

void run_median_filter(Image* image, Image* new_image, char* filename){
    int window_size = 1;
    
    for(int i = 5; i <= 15; i += 2) {
        window_size = i;

        char newImageFilename[100] = "";
        sprintf(newImageFilename, "%s_median%d.raw", filename, window_size);
        printf("%s\n", newImageFilename);
        new_image->pixels = medianFilter(image->pixels, image->width, image->height, window_size);
        generateImageFromPixels(new_image, newImageFilename);
        freeImagePixels(new_image);
    }
}

void run_mean_filter(Image* image, Image* new_image, char* filename){
    int window_size = 1;
    
    for(int i = 5; i <= 15; i += 2) {
        window_size = i;

        char newImageFilename[100] = "";
        sprintf(newImageFilename, "%s_mean%d.raw", filename, window_size);
        printf("%s\n", newImageFilename);
        new_image->pixels = meanFilter(image->pixels, image->width, image->height, window_size);
        generateImageFromPixels(new_image, newImageFilename);
        freeImagePixels(new_image);
    }
}

int main(int argc, char** argv)
{
    printf("%s\n", argv[1]);
    
    Image image;
    Image new_image;
    float sigma = 0;
    int window_size = 1;
    int patch_size = 1;

    image.width = atoi(argv[3]);
    image.height = atoi(argv[4]);
    new_image.width = image.width;
    new_image.height = image.height;

    getImagePixelsFromFile(&image, argv[1]);
    printf("%d %d\n", image.width, image.height);

    // run_median_filter(&image, &new_image, argv[2]);
    run_mean_filter(&image, &new_image, argv[2]);
    // int max = 5;
    // int start = 5;
    // for(int i = start; i <= max; i += 2) {
    //     patch_size = i;
    //     window_size = i + 2;

    //     for(int j = start; j <= max; j++) {
    //         // sigma = j * 0.1;
    //         sigma = 0.5;
            
    //         char newImageFilename[100] = "";
    //         sprintf(newImageFilename, "%s_nlm%d-%d.raw", argv[2], j, window_size);
    //         printf("%s\n", newImageFilename);
    //         new_image.pixels = nlmFilter(image.pixels, image.width, image.height, sigma, window_size, patch_size);
    //         // printImageInfo(&new_image);
    //         generateImageFromPixels(&new_image, newImageFilename);
    //         freeImagePixels(&new_image);
    //     }
    // }

    return 0;
}