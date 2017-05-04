#include "FL.h"
#include "bagOfVisualWords.h"

int main(int argc, char **argv) {

    DirectoryManager* directoryManager;
    FeatureMatrix * featureMatrix;
    int patchSize, numberOfCluster;

    numberOfCluster = 6;
    patchSize = 10;

/*----------------------------------------------------------------------------*/
/*---------------------------Computing Dictionary-----------------------------*/
/*----------------------------------------------------------------------------*/

    //Load dictionary images
    directoryManager = loadDirectory("../processedData/dictionary-creation", 1);

    //compute feature matrix
    featureMatrix = computeFeatureVectors(directoryManager, patchSize);

    //get the words
    FeatureMatrix * dictionary = kMeansClustering(featureMatrix, numberOfCluster);

    //free memory
    destroyDirectoryManager(directoryManager);
    destroyFeatureMatrix(&featureMatrix);
/*----------------------------------------------------------------------------*/
/*--------------------------------TRAINING------------------------------------*/
/*----------------------------------------------------------------------------*/
    FeatureMatrix * imageFeatureMatrix, * matrixWordHistogram;
    Image* currentImage;

    //Load training images
    directoryManager = loadDirectory("../processedData/training", 1);

    int* labels = (int *)calloc((int)directoryManager->nfiles, sizeof(int));

    matrixWordHistogram = createFeatureMatrix((int)directoryManager->nfiles);

    //go through images
    for (int i = 0; i < (int)directoryManager->nfiles; i++) {
        currentImage = readImage(directoryManager->files[fileIndex]->path);
        imageFeatureMatrix = computeFeatureVectorsImage(currentImage, patchSize);
        labels[i] = directoryManager->files[fileIndex]->label;
        matrixWordHistogram->featureVector[i] = computeWordHistogram(imageFeatureMatrix, dictionary);

        destroyImage(&currentImage);
        destroyFeatureMatrix(&imageFeatureMatrix);
    }

    //TODO treinar classificador com o wordHistogram e labels (achar os cluster de cada label)
    FeatureMatrix * clustersClassifier;

    //free memory
    destroyDirectoryManager(directoryManager);

/*----------------------------------------------------------------------------*/
/*--------------------------------TEST----------------------------------------*/
/*----------------------------------------------------------------------------*/
    //Load test images
    directoryManager = loadDirectory("../processedData/test", 1);
    FeatureVector * wordHistogram;

    int predictedValue, actualValue, correctAnswers = 0, wrongAnswers = 0;

    //go through images
    for (int i = 0; i < (int)directoryManager->nfiles; ++i) {
          currentImage = readImage(directoryManager->files[fileIndex]->path);
          imageFeatureMatrix = computeFeatureVectorsImage(currentImage, patchSize);
          actualValue = directoryManager->files[fileIndex]->label;
          vectorWordHistogram = computeWordHistogram(imageFeatureMatrix, dictionary);
          predictedValue = findNearestCluster(vectorWordHistogram, clustersClassifier);

          if(actualValue == predictedValue)
              correctAnswers++;
          else
              wrongAnswers++;

          destroyFeatureVector(&vectorWordHistogram);
          destroyImage(&currentImage);
          destroyFeatureMatrix(&imageFeatureMatrix);
    }

    printf("Total: %d\n", correctAnswers+wrongAnswers);
    printf("Correct: %d Wrong:\n", correctAnswers , wrongAnswers);
    printf("Accuracy: %f\n", (float)correctAnswers/(float)wrongAnswers);

    //free memory
    destroyDirectoryManager(directoryManager);
    destroyFeatureMatrix();
    free(expectedLabels);
    free(predictedLabels);

    return 0;
}
