//
// Created by deangeli on 5/15/17.
//

#include "matrix.h"

Matrix* createMatrix(size_t nrows, size_t ncols, size_t dataSize_bytes){
    Matrix* matrix = (Matrix*)calloc(1,sizeof(Matrix));
    size_t nElements = nrows*ncols;
    matrix->numberColumns = ncols;
    matrix->numberRows = nrows;
    matrix->numberElements = nElements;
    matrix->matrixData = createNullVector(nElements,dataSize_bytes);
    return matrix;
}

Matrix* createMatrix(size_t nrows, size_t ncols){
    Matrix* matrix = (Matrix*)calloc(1,sizeof(Matrix));
    size_t nElements = nrows*ncols;
    matrix->numberColumns = ncols;
    matrix->numberRows = nrows;
    matrix->numberElements = nElements;
    matrix->matrixData = createNullVector(nElements,sizeof(double));
    return matrix;
}

Matrix* createMatrix(GVector* vector){
    Matrix* matrix = (Matrix*)calloc(1,sizeof(Matrix));
    size_t nElements = vector->size;
    matrix->numberColumns = vector->size;
    matrix->numberRows = 1;
    matrix->numberElements = nElements;
    matrix->matrixData = createNullVector(nElements,vector->elementSize);
    memcpy(matrix->matrixData->data, vector->data, matrix->matrixData->elementSize*matrix->numberElements);
    return matrix;
}

Matrix* copyMatrix(Matrix* matrix){
    Matrix* output = createMatrix(matrix->numberRows,matrix->numberColumns,matrix->matrixData->elementSize);
    output->numberColumns = matrix->numberColumns;
    output->numberRows = matrix->numberRows;
    output->numberElements = matrix->numberElements;
    memmove(output->matrixData->data, matrix->matrixData->data, matrix->matrixData->elementSize*matrix->numberElements);
    return output;
}
void reshapeMatrix(Matrix* matrix, size_t nrows, size_t ncols){
    if(nrows*ncols != matrix->numberElements){
        printf("[reshapeMatrix] desired shape not match");
        return;
    }
    matrix->numberRows = nrows;
    matrix->numberColumns = ncols;
}


//TODO: there is a bug here
void stackVerticallyMatricesInPlace(Matrix** matrix1,Matrix* matrix2){
    Matrix* aux = *matrix1;
    if(aux == NULL && matrix2 == NULL){
        return;
    }
    if(matrix2 == NULL){
        return;
    }

    if(aux == NULL){
        *matrix1 = copyMatrix(matrix2);
        return;
    }
    if(aux->numberColumns != matrix2->numberColumns){
        printf("[concatenateMatrices] matrices dimension mismatch\n");
        return;
    }
    if(aux->matrixData->elementSize != matrix2->matrixData->elementSize){
        printf("[concatenateMatrices] matrices have different data types\n");
        return;
    }
    size_t totalRows = aux->numberRows + matrix2->numberRows;
    size_t newTotal = totalRows*aux->numberColumns;
    void* safeCopy = realloc((*matrix1)->matrixData->data,(*matrix1)->matrixData->elementSize*newTotal);
    if(safeCopy == NULL){
        return;
    }
    size_t nbytes1 =  (*matrix1)->matrixData->elementSize * (*matrix1)->numberElements;
    size_t nbytes2 =  matrix2->matrixData->elementSize*matrix2->numberElements;
    (*matrix1)->matrixData->data = safeCopy;
    (*matrix1)->numberRows = totalRows;
    memcpy((unsigned char*)(*matrix1)->matrixData->data+nbytes1,
           (unsigned char*)matrix2->matrixData->data, nbytes2);
}

Matrix* stackVerticallyMatrices(Matrix* matrix1, Matrix* matrix2){

    if(matrix1 == NULL && matrix2 == NULL){
        return NULL;
    }
    if(matrix1 == NULL){
        return copyMatrix(matrix2);
    }

    if(matrix2 == NULL){
        return copyMatrix(matrix1);
    }

    if(matrix1->numberColumns != matrix2->numberColumns){
        printf("[stackVerticallyMatrices] matrices dimension mismatch\n");
        return NULL;
    }

    if(matrix1->matrixData->elementSize != matrix2->matrixData->elementSize){
        printf("[stackVerticallyMatrices] matrices have different data types\n");
        return NULL;
    }
    size_t totalRows = matrix1->numberRows + matrix2->numberRows;
    size_t nCols = matrix1->numberColumns;
    Matrix* output = createMatrix(totalRows,nCols,matrix1->matrixData->elementSize);

    //first matrix
    size_t nbytes1 =  matrix1->matrixData->elementSize*matrix1->numberElements;
    memcpy(output->matrixData->data, matrix1->matrixData->data, nbytes1);
    //second matrix
    size_t nbytes2 =  matrix2->matrixData->elementSize*matrix2->numberElements;
    memcpy((unsigned char*)output->matrixData->data+nbytes1,(unsigned char*)matrix2->matrixData->data, nbytes2);
    return output;
}

Matrix* stackVerticallyMatrices(Matrix* matrix1, GVector* vector){
    if(matrix1 == NULL && vector == NULL){
        return NULL;
    }
    if(vector == NULL){
        return copyMatrix(matrix1);
    }

    if(matrix1 == NULL){
        return createMatrix(vector);
    }



    if(matrix1->numberColumns != vector->size){
        printf("[stackVerticallyMatrices] matrices dimension mismatch\n");
        return NULL;
    }

    if(matrix1->matrixData->elementSize != vector->elementSize){
        printf("[stackVerticallyMatrices] matrices have different data types\n");
        return NULL;
    }
    size_t totalRows = matrix1->numberRows + 1;
    size_t nCols = matrix1->numberColumns;
    Matrix* output = createMatrix(totalRows,nCols,matrix1->matrixData->elementSize);

    //first matrix
    size_t nbytes1 =  matrix1->matrixData->elementSize*matrix1->numberElements;
    memcpy(output->matrixData->data, matrix1->matrixData->data, nbytes1);
    //second matrix
    size_t nbytes2 =  vector->elementSize*vector->size;
    memcpy((unsigned char*)output->matrixData->data+nbytes1,(unsigned char*)vector->data, nbytes2);
    return output;
}



void destroyMatrix(Matrix **pMatrix){
    if ( (*pMatrix) == NULL || pMatrix == NULL){
        return;
    }
    destroyVector(&((*pMatrix)->matrixData));
    free(*pMatrix);
    *pMatrix= NULL;
}

void swapMatrixRows(Matrix* matrix, size_t index1, size_t index2){
    unsigned char* auxBuffer = (unsigned char*)calloc(matrix->matrixData->elementSize*matrix->numberRows,sizeof(unsigned char));
    unsigned char* row1 = (unsigned char*)matrix->matrixData->data + (index1 * matrix->matrixData->elementSize * matrix->numberColumns);
    unsigned char* row2 = (unsigned char*)matrix->matrixData->data + (index2 * matrix->matrixData->elementSize * matrix->numberColumns);
    memmove(auxBuffer,row1,matrix->matrixData->elementSize*matrix->numberColumns);
    memmove(row1,row2,matrix->matrixData->elementSize*matrix->numberColumns);
    memmove(row2,auxBuffer,matrix->matrixData->elementSize*matrix->numberColumns);
    free(auxBuffer);
}

void removeMatrixRow(Matrix* matrix, size_t rowIndex){
    if(rowIndex == matrix->numberRows-1){
        matrix->numberRows -= 1;
        matrix->matrixData->size -= matrix->numberColumns;
        return;
    }

    unsigned char* rowAddress = (unsigned char*)matrix->matrixData->data + (rowIndex * matrix->matrixData->elementSize * matrix->numberColumns);
    unsigned char* nextRowAddress = (unsigned char*)matrix->matrixData->data + ( (rowIndex+1) * matrix->matrixData->elementSize * matrix->numberColumns);
    size_t totalBytes2Move = ((matrix->numberRows)-(rowIndex+1))*matrix->matrixData->elementSize * matrix->numberColumns;
    memmove(rowAddress,nextRowAddress,totalBytes2Move);
    matrix->numberRows -= 1;
    matrix->matrixData->size -= matrix->numberColumns;
}

Matrix* getMatrixRows(Matrix* matrix, int startRow,int endRow){
    int delta = endRow-startRow;
    if(delta < 0){
        printf("[getMatrixRows] star row is greather or equal than end row\n");
        return NULL;
    }
    size_t shift_bytes = (delta*matrix->matrixData->elementSize*matrix->numberColumns);
    Matrix* outputMatrix = createMatrix(delta,matrix->numberColumns,matrix->matrixData->elementSize);
    unsigned char* source = (unsigned char*)matrix->matrixData->data + (startRow*matrix->matrixData->elementSize*matrix->numberColumns);
    memmove(outputMatrix->matrixData->data,source,shift_bytes);
    return outputMatrix;
}

Matrix* getMatrixRows(Matrix* matrix, int* indices,int n){
    if(n <= 0){
        printf("[getMatrixRows] vector of indices is empty\n");
        return NULL;
    }
    Matrix* output = createMatrix(n,matrix->numberColumns,matrix->matrixData->elementSize);
    int index;
    for (int i = 0; i < n; ++i) {
        index = indices[i];
        copyMatrixRow(output,matrix,i,index);
    }
    return output;
}

void destroyMatrixVoidPointer(void* matrix){
    Matrix** aux =  (Matrix**) matrix;
    destroyMatrix(aux);
    aux = NULL;
}



