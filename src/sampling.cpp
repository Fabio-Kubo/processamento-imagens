//
// Created by deangeli on 5/20/17.
//

#include "sampling.h"

GVector* gridSampling(Image* image, size_t patchSizeX,size_t patchSizeY){
    size_t nPatchs_X = image->nx/patchSizeX;
    size_t nPatchs_Y = image->ny/patchSizeY;
    size_t nPatchs = nPatchs_X*nPatchs_Y;
    GVector* vector_images = createNullVector(nPatchs,sizeof(Image*));
    int k = 0;
    for (size_t y = 0; y <= (size_t)image->ny-patchSizeY; y +=patchSizeY) {
        for (size_t x = 0; x <= (size_t)image->nx-patchSizeX; x += patchSizeX) {
            Image *subImage = extractSubImage(image, x, y,patchSizeX, patchSizeY, true);
            subImage->imageROI.coordinateX = x;
            subImage->imageROI.coordinateY = y;
            subImage->imageROI.coordinateZ = 0;
            subImage->imageROI.size_x = patchSizeX;
            subImage->imageROI.size_y = patchSizeY;
            subImage->imageROI.size_z = 1;
            VECTOR_GET_ELEMENT_AS(Image*,vector_images,k) = subImage;
            k++;
        }
    }
    return vector_images;
}


GVector* randomSampling_noImage(Image* image, size_t numberOfPatchs, size_t patchSizeX,size_t patchSizeY, int seed){
    srand(seed);
    size_t patchsGenerated = 0;
    int randomXcoordinate;
    int randomYcoordinate;
    int lowerBound_X = 0 +  patchSizeX/2;
    int upperBound_X = image->nx -  (patchSizeX/2);
    int lowerBound_Y = 0 +  patchSizeY/2;
    int upperBound_Y = image->ny -  (patchSizeY/2);

    if(lowerBound_X > upperBound_X){
        printf("[randomSampling_noImage] can not generated patchs. Lower bound X is greather than upper bound\n");
        return NULL;
    }

    if(lowerBound_Y > upperBound_Y){
        printf("[randomSampling_noImage] can not generated patchs. Lower bound Y is greather than upper bound\n");
        return NULL;
    }

    GVector* vector = createNullVector(numberOfPatchs,sizeof(RegionOfInterest));
    while (numberOfPatchs > patchsGenerated){
        randomXcoordinate = randomInteger(lowerBound_X,upperBound_X);
        randomYcoordinate = randomInteger(lowerBound_Y,upperBound_Y);
        RegionOfInterest regionOfInterest;

        regionOfInterest.coordinateX = randomXcoordinate - patchSizeX/2;
        regionOfInterest.coordinateY = randomYcoordinate - patchSizeY/2;
        regionOfInterest.coordinateZ = 0;

        regionOfInterest.size_x = patchSizeX;
        regionOfInterest.size_y = patchSizeY;
        regionOfInterest.size_z = 1;

        VECTOR_GET_ELEMENT_AS(RegionOfInterest*,vector,patchsGenerated) = &regionOfInterest;
        patchsGenerated++;
    }

    return vector;
}
