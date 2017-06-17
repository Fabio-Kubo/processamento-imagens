#include "FL.h"
#include "ift.h"

iftImage* imageToIft(Image* image) {
    iftImage *newImage = iftCreateColorImage(image->nx, image->ny, image->nz);

    for (int x = 0; x < image->nx; x++) {
        for (int y = 0; y < image->ny; y++) {
            iftImgCrElem2D(newImage, x, y) = imageValCh(image, x, y, 0);
            iftImgElem2D(newImage, x, y) = imageValCh(image, x, y, 1);
            iftImgCbElem2D(newImage, x, y) = imageValCh(image, x, y, 2);
        }
    }

    return newImage;
}


GVector* iftImageToVector(iftImage *img, int numberSuperPixels) {
    GVector* vector_images = createNullVector(numberSuperPixels, sizeof(iftImage*));
    /*int k = 0;
    for (size_t y = 0; y <= (size_t)img->ny-patchSizeY; y +=patchSizeY) {
        for (size_t x = 0; x <= (size_t)img->nx-patchSizeX; x += patchSizeX) {
            VECTOR_GET_ELEMENT_AS(Image*,vector_images,k) = extractSubImage(img, x, y,patchSizeX, patchSizeY, true);
            k++;
        }
    }*/
    return vector_images;
}

iftImage *computeSuperPixels(iftImage *img, int nsuperpixels, float alpha, float beta, int niters, int smooth_niters) {
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


  if (iftIsColorImage(img)){
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

  iftDestroyImage(&mask1);
  iftDestroyMImage(&mimg);

  /* Smooth regions in the label map of igraph */
  if (smooth_niters > 0){
    iftIGraphSetWeightForRegionSmoothing(igraph, img);
    iftIGraphSmoothRegions(igraph, smooth_niters);
  }
  /* Get superpixel image */
  label   = iftIGraphLabel(igraph);

  iftDestroyImage(&seeds);
  iftDestroyIGraph(&igraph);
  iftDestroyAdjRel(&A);


  return label;
}

GVector* superPixelSamplingBow(Image* image, BagOfVisualWordsManager* bagOfVisualWordsManager) {
    ArgumentList* argumentList = bagOfVisualWordsManager->argumentListOfSampler;

    if(argumentList->length != 5){
        printf("[superPixelSampling] invalid argument list");
        return NULL;
    }

    size_t numberSuperPixels = ARGLIST_GET_ELEMENT_AS(size_t ,argumentList,0);
    size_t alpha = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,1);
    size_t beta = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,2);
    size_t numberIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,3);
    size_t numberSmoothIterations = ARGLIST_GET_ELEMENT_AS(size_t,argumentList,4);

    iftImage *ift = imageToIft(image);
    iftImage *superPixels = computeSuperPixels(ift, numberSuperPixels, alpha, beta, numberIterations, numberSmoothIterations);

    GVector *samplingVector = iftImageToVector(superPixels, numberSuperPixels);

    iftDestroyImage(&ift);
    iftDestroyImage(&superPixels);

    return samplingVector;
}
