//
// Created by deangeli on 4/7/17.
//

#include "bagOfVisualWords.h"


FeatureMatrix* computeFeatureVectors(DirectoryManager* directoryManager, int patchSize){

    //Image* currentImage;
//    Image* patch;
//    FeatureVector* patchVector;
    int binSize = 64;
    Image* firstImage = readImage(directoryManager->files[0]->path);
    int patchX_axis = firstImage->nx/patchSize;
    int patchY_axis = firstImage->ny/patchSize;
    int numberPatchsPerImage = patchX_axis*patchY_axis;
    int numberPatchs = numberPatchsPerImage*directoryManager->nfiles;
    //FeatureMatrix* featureMatrix = createFeatureMatrix();
    FeatureMatrix* featureMatrix = createFeatureMatrix(numberPatchs);
    destroyImage(&firstImage);
    int k=0;

//#pragma omp parallel for
    for (size_t fileIndex = 0; fileIndex < directoryManager->nfiles; ++fileIndex) {
        Image* currentImage = readImage(directoryManager->files[fileIndex]->path);

        for (int y = 0; y <= currentImage->ny-patchSize; y +=patchSize) {
            for (int x = 0; x <= currentImage->nx-patchSize; x += patchSize) {
                Image* patch = extractSubImage(currentImage,x,y,patchSize,patchSize,true);
                featureMatrix->featureVector[k] = computeHistogramForFeatureVector(patch,binSize,true);
                k++;
                destroyImage(&patch);
            }
        }
        destroyImage(&currentImage);
    }
    return featureMatrix;
}

FeatureMatrix* computeFeatureVectors(Image* imagePack, int patchSize){
    Image* currentSlice;
    Image* patch;
    Histogram* histogram;
    FeatureVector* patchVector;
    int patchX_axis = imagePack->nx/patchSize;
    int patchY_axis = imagePack->ny/patchSize;
    int numberPatchsPerImage = patchX_axis*patchY_axis;
    int numberPatchs = numberPatchsPerImage*imagePack->nz;
    int binSize = 64;

    FeatureMatrix* featureMatrix = createFeatureMatrix(numberPatchs);
    int k=0;
    for (int z = 0; z < imagePack->nz; ++z) {
        currentSlice = getSlice(imagePack,z);
        for (int y = 0; y <= imagePack->ny-patchSize; y +=patchSize) {
            for (int x = 0; x <= imagePack->nx-patchSize; x += patchSize) {
                patch = extractSubImage(currentSlice,x,y,patchSize,patchSize,true);
                histogram = computeHistogram(patch,binSize,true);
                patchVector = createFeatureVector(histogram);
                featureMatrix->featureVector[k] = patchVector;
                k++;
                destroyHistogram(&histogram);
                destroyImage(&patch);
            }
        }
        destroyImage(&currentSlice);
    }
    return featureMatrix;
}

FeatureMatrix* kMeansClustering(FeatureMatrix* featureMatrix, int numberOfCluster){
    FeatureMatrix* dict = createFeatureMatrix(numberOfCluster);
    int k = 0;
    bool *isUsed = (bool*)calloc(featureMatrix->nFeaturesVectors,sizeof(*isUsed));
    int* labels = (int*)calloc(featureMatrix->nFeaturesVectors,sizeof(*labels));
    while (k < numberOfCluster) {
        int randomIndex = RandomInteger(0,featureMatrix->nFeaturesVectors);
        if(isUsed[randomIndex] == false){
            dict->featureVector[k] = copyFeatureVector(featureMatrix->featureVector[randomIndex]);
            isUsed[randomIndex] = true;
            k++;
        }
    }
    free(isUsed);
    //not finished yet





    return NULL;
}
