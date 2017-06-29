#include "FL.h"
#include "ift.h"

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

    /*printf("%d\n", newImage->n);
    printf("%d\n", newImage->xsize);
    printf("%d\n", newImage->ysize);
    printf("%d\n", newImage->zsize);
    printf("%f\n", newImage->dx);
    printf("%f\n", newImage->dy);
    printf("%f\n", newImage->dz);*/

    /*printf("%d\n", image->numberPixels);
    printf("%d\n", image->nx);
    printf("%d\n", image->ny);
    printf("%d\n", image->nz);
    printf("%f\n", image->dx);
    printf("%f\n", image->dy);
    printf("%f\n", image->dz);*/

    /*for (int y = 0; y < image->ny; y++) {
      for (int x = 0; x < image->nx; x++) {
        printf("pixel %d: ", y*image->nx + x);
        imageValCh(image, x, y, 0) = iftGetRGB(newImage, y*newImage->xsize + x, 256).val[0];
        imageValCh(image, x, y, 1) = iftGetRGB(newImage, y*newImage->xsize + x, 256).val[1];
        imageValCh(image, x, y, 2) = iftGetRGB(newImage, y*newImage->xsize + x, 256).val[2];
      }
    }*/

    /*for (int y = 0; y < newImage->ysize; y++) {
      for (int x = 0; x < newImage->xsize; x++) {
        printf("pixel %d: ", y*newImage->xsize + x);
        printf("%d ", iftGetRGB(newImage, y*newImage->xsize + x, 256).val[0]);
        printf("%d ", iftGetRGB(newImage, y*newImage->xsize + x, 256).val[1]);
        printf("%d\n", iftGetRGB(newImage, y*newImage->xsize + x, 256).val[2]);
      }
    }

    printf("end\n");*/

    return newImage;
}


GVector* iftImageToVector(Image *img, iftImage *labels, int numberSuperPixels, int patchSize, int maxSamples) {
    GVector* vector_images = createNullVector(maxSamples, sizeof(iftImage*));
    iftIntArray *allLabels = iftGetLabels(labels);
    iftBoundingBox border;

    if (maxSamples == 0) {
      maxSamples = INT_MAX;
    }

    int countLabels, countAdjacents, stop = 0;
    unsigned int k = 0;
    for (int y = 0; y < labels->ysize && !stop; y++) {
        for (int x = 0; x < labels->xsize && !stop; x++) {
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

            //int label = labels->val[y*labels->xsize + x];
            /*imageValCh(img, x, y, 0) = label;
            imageValCh(img, x, y, 1) = label;
            imageValCh(img, x, y, 2) = label;*/


            for (unsigned int i = 0; i < allLabels->n; i++) {
              countLabels = iftCountObjectSpelsFromBoundingBox(labels, allLabels->val[i], border);
              if (countLabels > 0) {
                countAdjacents++;
              }
            }
            if (countAdjacents >= 3) {
              //printf("pixel %d: ", y*labels->xsize + x);
              //printf("poi: %d\n", countAdjacents);

              for (int i = x - patchSize; i <= x + patchSize; i++) {
                if (i < 0) continue; if (i >= img->nx) continue;
                for (int j = y - patchSize; j <= y + patchSize; j++) {
                  if (j < 0) continue; if (j >= img->ny) continue;
                  imageValCh(img, i, j, 0) = 0;
                  imageValCh(img, i, j, 1) = 255;
                  imageValCh(img, i, j, 2) = 255;
                }
              }

              for (int i = border.begin.x; i <= border.end.x; i++) {
                for (int j = border.begin.y; j <= border.end.y; j++) {
                  imageValCh(img, i, j, 0) = 255;
                  imageValCh(img, i, j, 1) = 0;
                  imageValCh(img, i, j, 2) = 255;
                }
              }

              imageValCh(img, x, y, 0) = 255;
              imageValCh(img, x, y, 1) = 0;
              imageValCh(img, x, y, 2) = 0;

              Image *subImage = extractSubImage(img, x, y,patchSize, patchSize, true);
              subImage->imageROI.coordinateX = x - patchSize / 2;
              subImage->imageROI.coordinateY = y - patchSize / 2;
              subImage->imageROI.coordinateZ = 0;
              subImage->imageROI.size_x = patchSize;
              subImage->imageROI.size_y = patchSize;
              subImage->imageROI.size_z = 1;
              //printf("x:%d  y:%d \n", x, y);
              //printf("patchSize: %d\n", patchSize/2);
              //printf("Cx:%f Cy:%f\n", subImage->imageROI.coordinateX, subImage->imageROI.coordinateY);
              VECTOR_GET_ELEMENT_AS(Image*,vector_images,k) = subImage;
              k++;

              if (k >= vector_images->size) stop = 1;
            }
        }
        //writeImagePNG(img,"poi.png");
    }
    resizeVector(vector_images, k - 1);
    return vector_images;
}

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

    /*iftAdjRel *A = iftCircular(0.0);
    iftColor rgb;
    rgb.val[0] = 0;
    rgb.val[1] = 255;
    rgb.val[2] = 255;
    iftColor YCbCr = iftRGBtoYCbCr(rgb, 255);
    iftImage *border  = iftBorderImage(superPixels);
    iftDrawBorders(ift,border,A,YCbCr,A);
    for (int y = 0; y < image->ny; y++) {
      for (int x = 0; x < image->nx; x++) {
        imageValCh(image, x, y, 0) = iftGetRGB(ift, y*ift->xsize + x, 256).val[0];
        imageValCh(image, x, y, 1) = iftGetRGB(ift, y*ift->xsize + x, 256).val[1];
        imageValCh(image, x, y, 2) = iftGetRGB(ift, y*ift->xsize + x, 256).val[2];
      }
    }
    writeImagePNG(image,"border.png");*/

    iftDestroyImage(&ift);
    iftDestroyImage(&superPixels);

    return samplingVector;
}
