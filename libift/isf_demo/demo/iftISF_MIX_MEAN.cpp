#include "ift.h"

iftImage *iftCompute_ISF_MIX_MEAN_Superpixels(iftImage *img, int nsuperpixels, float alpha, float beta, int niters, int smooth_niters, int *nseeds, int *finalniters) {
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

  *nseeds = iftNumberOfElements(seeds);

  iftDestroyImage(&mask1);
  iftDestroyMImage(&mimg);

  /* Superpixel segmentation */
  *finalniters = iftIGraphISF_Mean(igraph,seeds,alpha,beta,niters);

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

int main(int argc, char *argv[])
{
  iftImage  *img, *label;
  iftImage  *gt_borders=NULL, *gt_regions=NULL, *border=NULL;
  iftAdjRel *A;
  int        niters, nseeds;
  char       labelfilename[256];
  char       borderfilename[256];
  iftColor   RGB, YCbCr;
  int        normvalue;

  if (argc!=8)
    iftError("Usage: iftISF_MIX_MEAN <image.[pgm,ppm,png]> <nsamples> <alpha (e.g., [0.005-0.2])> <beta (e.g., 12)> <niters (e.g., 10)> <smooth niters (e.g., 2)> <output_image>","main");

  printf("%s\n", argv[1]);

  img  = iftReadImageByExt(argv[1]);

  normvalue =  iftNormalizationValue(iftMaximumValue(img));

  label     = iftCompute_ISF_MIX_MEAN_Superpixels(img, atoi(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), &(nseeds), &(niters));

  border  = iftBorderImage(label);

  /* Write superpixel segmentation */
  RGB.val[0] = 0;
  RGB.val[1] = normvalue;
  RGB.val[2] = normvalue;
  A = iftCircular(0.0);
  YCbCr      = iftRGBtoYCbCr(RGB, normvalue);
  sprintf(labelfilename, "%s.pgm", argv[7]);
  iftWriteImageP2(label,labelfilename);
  iftDrawBorders(img,border,A,YCbCr,A);
  sprintf(borderfilename, "%s.png", argv[7]);
  iftWriteImageByExt(img, borderfilename);

  /* Print number of iterations and superpixels */
  printf("Number of iterations = %d \n", niters);
  printf("Number of superpixels = %d \n", nseeds);


  /* Free */
  iftDestroyImage(&img);
  iftDestroyImage(&label);
  iftDestroyImage(&border);
  iftDestroyAdjRel(&A);

  return(0);
}
