#include "FL.h"

int main(int argc, char **argv) {
    int nPatches = 16, nClusters;
    // Cria uma lista com todas as imagems das quais vamos extrair os descritores.
    DirectoryManager* directoryManager = loadFilesFromDirBySuffix("../data/objects", "ppm");
    // Cria uma matriz de todos os descritores a partir das imagens dadas.
    FeatureMatrix *featureMatrix = computeFeatureVectors(directoryManager, nPatches);

    nClusters = (int) featureMatrix->nFeaturesVectors / 10;
    printf("Gerado %d descritores.\n", featureMatrix->nFeaturesVectors);
    printf("Rodando k-means com %d clusters.\n", nClusters);

    // Gera um dicionario de palavras visuais utilizando implementaçao k-means.
    FeatureMatrix *dictionary = kMeansClustering(featureMatrix, nClusters);

    destroyFeatureMatrix(&dictionary);
    destroyFeatureMatrix(&featureMatrix);
    destroyDirectoryManager(&directoryManager);
}
