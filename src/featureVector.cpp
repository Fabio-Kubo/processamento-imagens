//
// Created by deangeli on 3/27/17.
//
#include "featureVector.h"



FeatureVector* createFeatureVector(int size){
    FeatureVector* featureVector = (FeatureVector*)calloc(1,sizeof(FeatureVector));
    featureVector->size = size;
    featureVector->features = (float*)calloc((size_t)size, sizeof(float));
    return featureVector;
}
void destroyFeatureVector(FeatureVector** featureVector){
    free((*featureVector)->features);
    free((*featureVector));
    (*featureVector) = NULL;
}

FeatureVector* createRandomNormalizedFeatureVector(int size){
    FeatureVector* featureVector = (FeatureVector*)calloc(1,sizeof(FeatureVector));
    featureVector->size = size;
    featureVector->features = (float*)calloc((size_t)size, sizeof(float));
    for (int i = 0; i < size; ++i) {
        featureVector->features[i] = randomNormalized();
    }
    return featureVector;
}

void printFeatureVector(FeatureVector* featureVector){
    if(featureVector == NULL){
        printf("FeatureVector pointer is NULL\n");
        return;
    }
    for (int i = 0; i < featureVector->size; ++i) {
        printf("%f ",featureVector->features[i]);
    }
    printf("\n");
}

FeatureVector* mergeFeatureVectors(FeatureVector* vector1,FeatureVector* vector2){
    FeatureVector *mergedVector = NULL;
    if(vector1 == NULL || vector2 == NULL){
        printf("vector1 or/and vector2 are NULL\n");
        return mergedVector;
    }
    int mergedSize = vector1->size + vector2->size;
    mergedVector = createFeatureVector(mergedSize);
    for (int i = 0; i < vector1->size; ++i) {
        mergedVector->features[i] = vector1->features[i];
    }

    for (int i = 0; i < vector2->size; ++i) {
        mergedVector->features[i+vector1->size] = vector2->features[i];
    }
    return mergedVector;
}

FeatureVector* createFeatureVector(float* vec,int size){
    FeatureVector* featureVector = (FeatureVector*)calloc(1,sizeof(FeatureVector));
    featureVector->size = size;
    featureVector->features = (float*)calloc((size_t)size,sizeof(float));
    for (int i = 0; i < size; ++i) {
        featureVector->features[i] = vec[i];
    }
    return featureVector;
}


void wirteFeatureVectors(FeatureVector** vectors, int nVectors, char *filename){
    FILE *fp = fopen(filename,"w");
    for (int i = 0; i < nVectors; ++i) {
        FeatureVector* vec = vectors[i];
        for (int j = 0; j < vec->size; ++j) {
            fprintf(fp,"%f",vec->features[j]);
            if(!(j == vec->size-1)){
                fprintf(fp," ");
            }
        }
        fprintf(fp,"\n");
    }
}


float vectorDifference(FeatureVector* vector1,FeatureVector* vector2){
    if(vector1->size != vector2->size){
        printf("vectors mismatch dimensions\n");
        return -1.0;
    }
    float difference = 0;
    FeatureVector* vector = createFeatureVector(vector1->size);
    float diff;
    for (int i = 0; i < vector1->size; ++i) {
        diff = (vector1->features[i]-vector2->features[i]);
        if(diff < 0){
            diff *= -1;
        }
        vector->features[i] = diff;
        //vector->features[i] = (vector1->features[i]-vector2->features[i])*(vector1->features[i]-vector2->features[i]);
        //vector->features[i] = sqrtf(vector->features[i]);
        difference += vector->features[i];
    }
    destroyFeatureVector(&vector);
    return difference;
}

FeatureVector* copyFeatureVector(FeatureVector* featureVector){
    return createFeatureVector(featureVector->features,featureVector->size);
}



FeatureMatrix* createFeatureMatrix(){
    FeatureMatrix* featureMatrix = NULL;
    featureMatrix = (FeatureMatrix*)calloc(1,sizeof(FeatureMatrix));
    featureMatrix->nFeaturesVectors = 0;
    featureMatrix->featureVector = NULL;
    return featureMatrix;
}

FeatureMatrix* createFeatureMatrix(int nFeaturesVectors){
    FeatureMatrix* featureMatrix = NULL;
    featureMatrix = (FeatureMatrix*)calloc(1,sizeof(FeatureMatrix));
    featureMatrix->nFeaturesVectors = nFeaturesVectors;
    featureMatrix->featureVector = (FeatureVector**)calloc((size_t)nFeaturesVectors,sizeof(FeatureVector*));
    return featureMatrix;
}

FeatureMatrix* createFeatureMatrix(int nFeaturesVectors,int vectorSize){
    FeatureMatrix* featureMatrix = NULL;
    featureMatrix = (FeatureMatrix*)calloc(1,sizeof(FeatureMatrix));
    featureMatrix->nFeaturesVectors = nFeaturesVectors;
    featureMatrix->featureVector = (FeatureVector**)calloc((size_t)nFeaturesVectors,sizeof(FeatureVector*));
    for (int i = 0; i < vectorSize; ++i) {
        featureMatrix->featureVector[0] = createFeatureVector(vectorSize);
    }
    return featureMatrix;
}

void destroyFeatureMatrix(FeatureMatrix** featureMatrix){
    if(*featureMatrix == NULL){
        return ;
    }
    for (int i = 0; i < (*featureMatrix)->nFeaturesVectors; ++i) {
        printf("miaus %d\n",i);
        destroyFeatureVector( &((*featureMatrix)->featureVector[i]) );
    }
    free((*featureMatrix)->featureVector);
    free((*featureMatrix));
    *featureMatrix = NULL;
}

void wirteFeatureMatrix(FeatureMatrix* featureMatrix, char *filename){
    FILE *fp = fopen(filename,"w");
    for (int i = 0; i < featureMatrix->nFeaturesVectors; ++i) {
        FeatureVector* vec = featureMatrix->featureVector[i];
        for (int j = 0; j < vec->size; ++j) {
            fprintf(fp,"%f",vec->features[j]);
            if(!(j == vec->size-1)){
                fprintf(fp," ");
            }
        }
        fprintf(fp,"\n");
    }
}

void addNewLines(FeatureMatrix** featureMatrix, int numberNewLines){
    FeatureMatrix* aux = *featureMatrix;

//    int numberLines = aux->nFeaturesVectors+numberNewLines;
//    FeatureVector** copy = (FeatureVector**)calloc(numberLines,sizeof(FeatureVector*));
//    for (int i = 0; i < aux->nFeaturesVectors; ++i) {
//        copy[i] = aux->featureVector[i];
//    }
//    free(aux->featureVector);
//    aux->featureVector = copy;
//    aux->nFeaturesVectors = numberLines;

    int numberLines = aux->nFeaturesVectors+numberNewLines;
    FeatureVector** newRows = (FeatureVector**)realloc(aux->featureVector,(numberLines)*sizeof(FeatureVector*));
    aux->featureVector = newRows;
    //aux->featureVector = (FeatureVector**)realloc(aux->featureVector,(numberLines)*sizeof(FeatureVector*));
    aux->nFeaturesVectors = numberLines;
}

void printFeatureMatrix(FeatureMatrix* featureMatrix){
    if(featureMatrix == NULL){
        printf("FeatureMatrix pointer is NULL\n");
        return;
    }
    for (int i = 0; i < featureMatrix->nFeaturesVectors; ++i) {
        printFeatureVector(featureMatrix->featureVector[i]);
    }
    printf("\n");
}


//void sortAt(FeatureVector featureVector, int lastIndex){
//
//    for (int i = lastIndex-1; i >= 0 ; --i) {
//        if(featureVector.features[lastIndex] > featureVector.features[i]){
//            break;
//        }
//    }
//    //featureVector
//}