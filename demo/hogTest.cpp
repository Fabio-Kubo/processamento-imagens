#include "FL.h"

int main(int argc, char **argv) {

    int patchSizeX = 32;
    int patchSizeY = 32;
    HogManager* hogManager = createHogManager();
    Image* seila = readImage("../data/lena.pgm");
    //hogManager->image = readImage("../data/lena.pgm");
    hogManager->useUnsignedGradients = true;
    hogManager->binSize = 20;
    hogManager->cellSizeX = patchSizeX/2;
    hogManager->cellSizeY = patchSizeY/2;
    hogManager->cellsPerBlockX = 2;
    hogManager->cellsPerBlockY = 2;
    hogManager->strideX = hogManager->cellSizeX;
    hogManager->strideY = hogManager->cellSizeY;
    //computeHogDescriptor(hogManager);

    GVector *vector = randomSampling_noImage(seila,4,patchSizeX,patchSizeY);
    for (int i = 0; i < vector->size; ++i) {
        RegionOfInterest ROI = VECTOR_GET_ELEMENT_AS(RegionOfInterest,vector,i);
        printImageRegionOfInterest(&ROI);
    }
    hogManager->image = seila;
    computeHogDescriptorForRegionsOfInterest(vector,hogManager);
    //computeHogDescriptorForRegionsOfInterest

    destroyVector(&vector);

    destroyHogManager(&hogManager);

    return 0;
}


