//
// Created by deangeli on 4/7/17.
//

#include "bagOfVisualWords.h"
#include "featureVector.h"

#include <vector>

using namespace std;

FeatureMatrix* computeFeatureVectors(DirectoryManager* directoryManager, int patchSize){

    int binSize = 64;
    Image* firstImage = readImage(directoryManager->files[0]->path);
    int patchX_axis = firstImage->nx/patchSize;
    int patchY_axis = firstImage->ny/patchSize;
    int numberPatchsPerImage = patchX_axis*patchY_axis;
    int numberPatchs = numberPatchsPerImage*directoryManager->nfiles;

    FeatureMatrix* featureMatrix = createFeatureMatrix(numberPatchs);
    destroyImage(&firstImage);
    int k=0;

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

FeatureMatrix * computeFeatureVectorsImage(Image* image, int patchSize){
    int binSize = 64;
    int patchX_axis = image->nx/patchSize;
    int patchY_axis = image->ny/patchSize;
    int numberPatchs = patchX_axis*patchY_axis;

    FeatureMatrix* featureMatrix = createFeatureMatrix(numberPatchs);
    destroyImage(&image);
    int k=0;
    for (int y = 0; y <= image->ny-patchSize; y +=patchSize) {
        for (int x = 0; x <= image->nx-patchSize; x += patchSize) {
            Image* patch = extractSubImage(image,x,y,patchSize,patchSize,true);
            featureMatrix->featureVector[k] = computeHistogramForFeatureVector(patch,binSize,true);
            k++;
            destroyImage(&patch);
        }
    }
    destroyImage(&image);
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

FeatureMatrix* kMeansClustering(FeatureMatrix* featureMatrix, int numberOfCluster) {
    FeatureMatrix* dict = createFeatureMatrix(numberOfCluster);
    FeatureMatrix* newClusters;
    int i, j, k, index = 0, indexNewCluster, loop;

    int * featureVectorsPerCluster = (int*) calloc(numberOfCluster, sizeof(int));
    bool *isUsed = (bool*)calloc(featureMatrix->nFeaturesVectors, sizeof(*isUsed));
    int* labels = (int*)calloc(featureMatrix->nFeaturesVectors, sizeof(*labels));

    k = 0;
    while (k < numberOfCluster) {
        int randomIndex = RandomInteger(0,featureMatrix->nFeaturesVectors);
        if(!isUsed[randomIndex]){
            dict->featureVector[k] = copyFeatureVector(featureMatrix->featureVector[randomIndex]);
            isUsed[randomIndex] = true;
            k++;
        }
    }

    for (loop = 0; loop < numberOfCluster; loop++) {
            //initialize newClusters (each feature is initilalized with 0)
            newClusters = createFeatureMatrix(numberOfCluster, featureMatrix->featureVector[0]->size);

            //Attribute the closest cluster to each data point
            for (i=0; i < featureMatrix->nFeaturesVectors; i++) {
                index = findNearestCluster(featureMatrix->featureVector[i], dict);
                labels[i] = index;
                featureVectorsPerCluster[index]++;

                //Sum the values to calculate the mean
                for (j=0; j < featureMatrix->featureVector[i]->size; j++) {
                    newClusters->featureVector[index]->features[j] += dict->featureVector[i]->features[j];
                }
            }

            //divide each feature by the number of featureVectors in the actual cluster
            for (i = 0; i < numberOfCluster; i++) {
                for (j = 0; j < newClusters->featureVector[i]->size; j++) {
                  newClusters->featureVector[index]->features[j] =
                    newClusters->featureVector[index]->features[j] / featureVectorsPerCluster[i];
                }

                indexNewCluster = findNearestCluster(newClusters->featureVector[index], featureMatrix);

                if(!isUsed[indexNewCluster]){
                    dict->featureVector[i] = copyFeatureVector(featureMatrix->featureVector[indexNewCluster]);
                    isUsed[indexNewCluster] = true;
                    isUsed[i] = false;
                }
            }
      }

    destroyFeatureMatrix(&newClusters);
    free(featureVectorsPerCluster);
    free(isUsed);
    free(labels);
    return dict;
}

FeatureMatrix * computeClusters(FeatureMatrix* matrixWordHistogram, int * labels, vector< vector<int> > controleWordHistogramLabel, int numberOfCluster){
    int i, j, k, currentCentroid;
    float currentDistance, maxDistance;
    vector<float> maximumDistanceAsCentroid(matrixWordHistogram->nFeaturesVectors, -1);
    vector<float> minimumDistanceCluster(numberOfCluster, -1);

    FeatureMatrix * clustersClassifier = createFeatureMatrix(numberOfCluster);

    //for each cluster we calculate its centroid
    for (i = 0; i < numberOfCluster; i++) {
        //Select one point as centroid then calculates its max distance to another point
        for (j = 0; j < (int)controleWordHistogramLabel[i].size(); j++) {
            currentCentroid = controleWordHistogramLabel[i][j];
            maxDistance = -1;

            for (k = 0; k < (int)controleWordHistogramLabel[i].size(); k++) {
                currentDistance = euclideanDistance(matrixWordHistogram->featureVector[currentCentroid],
                  matrixWordHistogram->featureVector[controleWordHistogramLabel[i][k]]);

                if(currentDistance > maxDistance){
                    maxDistance = currentDistance;
                }
            }
            maximumDistanceAsCentroid[currentCentroid] = maxDistance;
        }
    }

    //find one data point for each group that has the minimum max distance
    for (i = 0; i < matrixWordHistogram->nFeaturesVectors; i++) {
        if(minimumDistanceCluster[labels[i]] == -1 ||
                minimumDistanceCluster[labels[i]] > maximumDistanceAsCentroid[i])
            clustersClassifier->featureVector[labels[i]] = copyFeatureVector(matrixWordHistogram->featureVector[i]);
            minimumDistanceCluster[labels[i]] = maximumDistanceAsCentroid[i];
    }

    return clustersClassifier;
}

FeatureVector * computeWordHistogram(FeatureMatrix * imageFeatureMatrix, FeatureMatrix * dictionary){
    FeatureVector * wordHistogram = createFeatureVector(dictionary->nFeaturesVectors);
    int closestWordIndex, i;

    for (i = 0; i < imageFeatureMatrix->nFeaturesVectors; i++) {
        closestWordIndex = findNearestCluster(imageFeatureMatrix->featureVector[i], dictionary);
        wordHistogram->features[closestWordIndex]++;
    }

    return wordHistogram;
}
