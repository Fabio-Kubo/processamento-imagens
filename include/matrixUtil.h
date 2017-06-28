//
// Created by deangeli on 5/19/17.
//

#ifndef LIBFL_MATRIXUTIL_H
#define LIBFL_MATRIXUTIL_H

#include "matrix.h"
#include "distanceFunctions.h"


inline double computeDistanceBetweenRows(Matrix* matrix1, Matrix* matrix2,
                                         size_t indexRow_Source ,size_t indexRow_Target,
                                         DistanceFunction distanceFunction,
                                         ArgumentList* argumentList){

    size_t nCols = matrix2->numberColumns;
    float *vec_source = ((float*)matrix1->matrixData->data) + (indexRow_Source*matrix1->numberColumns);
    float *vec_target = ((float*)matrix2->matrixData->data) + (indexRow_Target*matrix2->numberColumns);

    return distanceFunction(vec_source,vec_target,nCols,argumentList);
}


inline size_t findNearestRow(Matrix* source, Matrix* target,
                             size_t indexRow_Target,
                             DistanceFunction distanceFunction = computeNormalizedL1Norm,
                             ArgumentList* argumentList = NULL){

    double minDistance = DBL_MAX;
    double currentDistance = 0;
    size_t indexMin = 0;
    for (size_t i = 0; i < source->numberRows; ++i) {
        currentDistance = computeDistanceBetweenRows(source,target,i,indexRow_Target,distanceFunction,argumentList);
        if(currentDistance < minDistance){
            minDistance = currentDistance;
            indexMin = i;
        }
    }
    return indexMin;
}

inline double* computeAllDistancesBetweenRowAndMatrix(Matrix* source, Matrix* target,
                                                     size_t indexRow_Target,
                                                     DistanceFunction distanceFunction = computeNormalizedL1Norm,
                                                     ArgumentList* argumentList = NULL){
    double* distances = (double*)calloc(source->numberRows,sizeof(double));
    for (size_t i = 0; i < source->numberRows; ++i) {
        distances[i] = computeDistanceBetweenRows(source,target,i,indexRow_Target,distanceFunction,argumentList);
    }
    return distances;
}

inline double* myInsertionSort(double* vector, size_t n){
    double* auxVector = (double*)calloc(n,sizeof(double));
    double aux;

    for (size_t i = 0; i < n; ++i) {
        auxVector[i] = vector[i];
        for (size_t j = i; j > 0; --j) {

            if(auxVector[j-1] > auxVector[j]){
                aux = auxVector[j];
                auxVector[j] = auxVector[j-1];
                auxVector[j-1] =  aux;
            }else{
                break;
            }

        }
    }
    return auxVector;
}

//option = 0 : vertical
//option = 1 : horizontal;
inline Matrix* computeMatrixMean(Matrix* matrix, int option = 0){
    Matrix* output = NULL;
    if(option == 0){
        output = createMatrix(1,matrix->numberColumns,matrix->numberElements);
        for (int i = 0; i < matrix->numberRows; ++i) {
            for (int j = 0; j < matrix->numberColumns; ++j) {
                MATRIX_GET_ELEMENT_PO_AS(float,output,0,j) += MATRIX_GET_ELEMENT_PO_AS(float,matrix,i,j);
            }
        }

        for (int j = 0; j < matrix->numberColumns; ++j) {
            MATRIX_GET_ELEMENT_PO_AS(float,output,0,j) /= matrix->numberColumns;
        }

    }else if(option == 1){
        output = createMatrix(matrix->numberRows,1,matrix->numberElements);
        for (int i = 0; i < matrix->numberRows; ++i) {
            for (int j = 0; j < matrix->numberColumns; ++j) {
                MATRIX_GET_ELEMENT_PO_AS(float,output,i,0) += MATRIX_GET_ELEMENT_PO_AS(float,matrix,i,j);
            }
        }

        for (int j = 0; j < matrix->numberColumns; ++j) {
            MATRIX_GET_ELEMENT_PO_AS(float,output,0,j) /= matrix->numberColumns;
        }


    }

    return output;
}


#endif //LIBFL_MATRIXUTIL_H
