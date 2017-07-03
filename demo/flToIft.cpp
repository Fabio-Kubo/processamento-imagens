#include "FL.h"
#include "ift.h"


/**
 * Converte uma imagem da estrutura Image para uma iftImage
 */
iftImage* imageToIft(Image* image) {
    iftImage *newImage = iftCreateColorImage(image->nx, image->ny, image->nz);
    for (int y = 0; y < image->ny; y++) {
        for (int x = 0; x < image->nx; x++) {
            iftSetRGB(
                newImage,
                image->ny*y + x,
                imageValCh(image, x, y, 0),
                imageValCh(image, x, y, 1),
                imageValCh(image, x, y, 2),
                image->scalingFactor
            );
        }
    }

    return newImage;
}

/**
 * Converte uma imagem de labels na estrutura iftImage para um vetor de samples.
 */
GVector* iftImageToVector(Image *img, iftImage *labels, int numberSuperPixels, int patchSize, int maxSamples) {
    if (0 == maxSamples) {
      maxSamples =(int) labels->ysize * labels->xsize;
    }
    GVector* vector_images = createNullVector(maxSamples, sizeof(iftImage*));
    iftIntArray *allLabels = iftGetLabels(labels);
    iftBoundingBox border;
    int countLabels, countAdjacents, stop = 0;
    unsigned int k = 0;
    for (int y = 0; y < labels->ysize && !stop; y++) {
        for (int x = 0; x < labels->xsize && !stop; x++) {
            // Para cada pixel (x,y), cria uma vizinhança 3x3 ao redor deste.
            countAdjacents = 0;
            border.begin.x = x - 1;
            border.begin.y = y - 1;
            border.begin.z = 0;
            border.end.x = x + 1;
            border.end.y = y + 1;
            border.end.z = 0;
            if (x == 0)
              border.begin.x = 0;
            if (y == 0)
              border.begin.y = 0;
            if (x >= labels->xsize - 1)
              border.end.x = labels->xsize - 1;
            if (y >= labels->ysize - 1)
              border.end.y = labels->ysize - 1;

            // Conta a quantidade de cada label na vizinhança do pixel.
            for (unsigned int i = 0; i < allLabels->n; i++) {
              countLabels = iftCountObjectSpelsFromBoundingBox(labels, allLabels->val[i], border);
              if (countLabels > 0) {
                countAdjacents++;
              }
            }
            // Se a quantidade de labels na vizinhança for maior ou igual a 3,
            // extraimos um sample centralizado naquele pixel.
            if (countAdjacents >= 3) {
              Image *subImage = extractSubImage(img, x, y,patchSize, patchSize, true);
              subImage->imageROI.coordinateX = x - patchSize / 2;
              subImage->imageROI.coordinateY = y - patchSize / 2;
              subImage->imageROI.coordinateZ = 0;
              subImage->imageROI.size_x = patchSize;
              subImage->imageROI.size_y = patchSize;
              subImage->imageROI.size_z = 1;
              VECTOR_GET_ELEMENT_AS(Image*,vector_images,k) = subImage;
              k++;

              if (k >= vector_images->size) stop = 1;
            }
        }
    }
    // Caso a quantidade de samples extraidas seja menor que o tamanho do vetor,
    // temos que reduzir o vetor para aquele tamanho para evitar problemas.
    resizeVector(vector_images, k - 1);
    return vector_images;
}

/**
 * Utiliza a biblioteca ift para extrair uma imagem de labels representando
 * os superpixels da imagem dada.
 */
iftImage *computeSuperPixels(iftImage *img, int nsuperpixels, float alpha, float beta, int niters, int smooth_niters, int patch_size) {
  iftImage  *mask1, *seeds, *label;
  iftMImage *mimg;
  iftAdjRel *A;
  iftIGraph *igraph;

  /* Set adjacency relation */
  if (iftIs3DImage(img)){
    A      = iftSpheric(1.0);
  } else {
    A      = iftCircular(1.0);
  }

  if (iftIsColorImage(img)) {
    /* RGB to Lab conversion */
    mimg   = iftImageToMImage(img,LABNorm_CSPACE);
  } else {
    mimg   = iftImageToMImage(img,GRAY_CSPACE);
  }

  mask1  = iftSelectImageDomain(mimg->xsize,mimg->ysize,mimg->zsize);

  /* Minima of a basins manifold in that domain */
  igraph = iftImplicitIGraph(mimg,mask1,A);

  /* Seed sampling for ISF */
  seeds   = iftAltMixedSampling(mimg,mask1,nsuperpixels);

  iftNumberOfElements(seeds);

  iftDestroyImage(&mask1);
  iftDestroyMImage(&mimg);

  /* Superpixel segmentation */
  iftIGraphISF_Mean(igraph,seeds,alpha,beta,niters);

  /* Smooth regions in the label map of igraph */
  if (smooth_niters > 0){
    iftIGraphSetWeightForRegionSmoothing(igraph, img);
    iftIGraphSmoothRegions(igraph, smooth_niters);
  }

  label   = iftIGraphLabel(igraph);

  iftDestroyImage(&seeds);
  iftDestroyIGraph(&igraph);
  iftDestroyAdjRel(&A);

  return label;
}

GVector* superPixelSamplingBow(Image* image, BagOfVisualWordsManager* bagOfVisualWordsManager) {
    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfSampler;

    if(argumentList->length != 7) {
        printf("[superPixelSampling] invalid argument list");
        return NULL;
    }

    size_t numberSuperPixels = ARGLIST_GET_ELEMENT_AS(size_t ,argumentList,0);
    float alpha = ARGLIST_GET_ELEMENT_AS(float,argumentList,1);
    size_t beta = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,2);
    size_t numberIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,3);
    size_t numberSmoothIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,4);
    size_t patchSize = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,5);
    size_t maxSamples = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,6);
    iftImage *ift = imageToIft(image);
    iftImage *superPixels = computeSuperPixels(ift, numberSuperPixels, alpha, beta, numberIterations, numberSmoothIterations, patchSize);
    GVector *samplingVector = iftImageToVector(image, superPixels, numberSuperPixels, patchSize, maxSamples);

    iftDestroyImage(&ift);
    iftDestroyImage(&superPixels);

    return samplingVector;
}
