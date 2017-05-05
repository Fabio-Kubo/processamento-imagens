/*
 *Created by Deangeli Gomes Neves
 *
 * This software may be freely redistributed under the terms
 * of the MIT license.
 *
 */

#ifndef _BAGOFVISUALWORDS_H_
#define _BAGOFVISUALWORDS_H_

#include <vector>
#include "common.h"
#include "featureVector.h"
#include "file.h"
#include "image.h"
#include "histogram.h"

using namespace std;

typedef struct _bagOfVisualWords {
    int patchSize;
    DirectoryManager* directoryManager;
    FeatureMatrix* vocabulary;
} BagOfVisualWords;

FeatureMatrix* computeFeatureVectors(Image* imagePack, int patchSize);
FeatureMatrix* computeFeatureVectors(DirectoryManager* directoryManager, int patchSize);
FeatureMatrix* kMeansClustering(FeatureMatrix* featureMatrix, int numberOfCluster);
FeatureVector * computeWordHistogram(FeatureMatrix * imageFeatureMatrix, FeatureMatrix * dictionary);
FeatureMatrix * computeFeatureVectorsImage(Image* image, int patchSize);
FeatureMatrix * computeClusters(FeatureMatrix* matrixWordHistogram, int * labels, vector< vector<int> > controleWordHistogramLabel, int numberOfCluster);


#endif //LIBFL_BAGOFVISUALWORDS_H
