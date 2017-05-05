#include <vector>
#include "FL.h"
#include "bagOfVisualWords.h"

using namespace std;

int main(int argc, char **argv) {

    DirectoryManager* directoryManager;
    FeatureMatrix * featureMatrix;
    int patchSize, numberOfCluster, fileIndex, i;
    numberOfCluster = 6;
    patchSize = 50;

/*----------------------------------------------------------------------------*/
/*---------------------------Computing Dictionary-----------------------------*/
/*----------------------------------------------------------------------------*/

    printf("Computing words...\n");

    //Load dictionary images
    directoryManager = loadDirectory("../processedData/dictionary-creation", 1);

    //compute feature matrix
    featureMatrix = computeFeatureVectors(directoryManager, patchSize);

    //get the words
    FeatureMatrix *dictionary = kMeansClustering(featureMatrix, numberOfCluster);

    printf("Words computed...\n");
//    //free memory
    destroyDirectoryManager(&directoryManager);
    destroyFeatureMatrix(&featureMatrix);
/*----------------------------------------------------------------------------*/
/*--------------------------------TRAINING------------------------------------*/
/*----------------------------------------------------------------------------*/
    FeatureMatrix * imageFeatureMatrix, * matrixWordHistogram;
    Image* currentImage;

    printf("Training initializing...\n");
    //Load training images
    directoryManager = loadDirectory("../processedData/training", 1);

    int* labels = (int *)calloc((int)directoryManager->nfiles, sizeof(int));
    matrixWordHistogram = createFeatureMatrix((int)directoryManager->nfiles);
    vector< vector<int> > controleWordHistogramLabel(dictionary->nFeaturesVectors, vector<int>(0));

    //go through images
    for (fileIndex = 0; fileIndex < (int)directoryManager->nfiles; fileIndex++) {
        currentImage = readImage(directoryManager->files[fileIndex]->path);
        imageFeatureMatrix = computeFeatureVectorsImage(currentImage, patchSize);
        labels[i] = directoryManager->files[fileIndex]->label;

        controleWordHistogramLabel[labels[i]].push_back(i);
        matrixWordHistogram->featureVector[i] = computeWordHistogram(imageFeatureMatrix, dictionary);

        destroyImage(&currentImage);
        destroyFeatureMatrix(&imageFeatureMatrix);
    }

    //Find one cluster data point for each group
    //FeatureMatrix * clustersClassifier = computeClusters(matrixWordHistogram, labels,
    //  controleWordHistogramLabel, numberOfCluster);

    //free memory
    //destroyDirectoryManager(&directoryManager);

/*----------------------------------------------------------------------------*/
/*--------------------------------TEST----------------------------------------*/
/*----------------------------------------------------------------------------*/
    //Load test images
   /* directoryManager = loadDirectory("../processedData/test", 1);
    FeatureVector * vectorWordHistogram;

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
    printf("Correct: %d Wrong:%d\n", correctAnswers , wrongAnswers);
    printf("Accuracy: %f\n", (float)correctAnswers/(float)wrongAnswers);

    //free memory
    destroyDirectoryManager(&directoryManager);
*/
    return 0;
}
