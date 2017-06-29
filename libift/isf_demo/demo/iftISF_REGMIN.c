#include "ift.h"

void iftForceMinima(iftLabeledSet **S, iftImage *regmin, iftAdjRel *A)
{
  iftLabeledSet *aux = *S, *newS=NULL;
  int i;
  int l = 1;

  while (aux != NULL){
    int      p   = aux->elem;
    iftVoxel u   = iftGetVoxelCoord(regmin,p);
    int min_dist = IFT_INFINITY_INT, qmin = IFT_NIL;
    for (i=0; i < A->n; i++) {
      iftVoxel v = iftGetAdjacentVoxel(A,u,i);
      if (iftValidVoxel(regmin,v)){
  int q = iftGetVoxelIndex(regmin,v);
  if ((regmin->val[q]!=0)&& 
      (!iftLabeledSetHasElement(newS,q))){
    int dist = iftSquaredVoxelDistance(u,v);
    if (dist < min_dist){
      min_dist = dist;
      qmin     = q;
    }
  }
      }
    }
    if (qmin != IFT_NIL){
      iftInsertLabeledSet(&newS,qmin,l);
      l++;
    }
    aux = aux->next;
  }

  iftDestroyLabeledSet(S);
  (*S)=newS;

}

iftImage *iftCompute_ISF_REGMIN_Superpixels(iftImage *orig_img, int nsuperpixels, int area, int *nseeds) {
  iftImage       *img[2], *label;
  iftAdjRel      *A, *B, *C;
  iftMImage      *mimg;
  iftImage       *mask, *seed; 
  iftLabeledSet  *S=NULL;
  iftImageForest *fst;
  int p, l;

  if (iftIs3DImage(orig_img)){
    iftError("It is not extended to 3D yet","main");
  }
  
  /* Set ajacency relations */
  A      = iftCircular(1.5);
  B      = iftCircular(3.0);
  C      = iftCircular(sqrtf(orig_img->n/(float)nsuperpixels));

  img[0]   = iftMedianFilter(orig_img,B);

  if (iftIsColorImage(orig_img)){
    /* RGB to Lab conversion */
    mimg   = iftImageToMImage(img[0],LABNorm_CSPACE);
  } else {
    mimg   = iftImageToMImage(img[0],GRAY_CSPACE);
  } 
  
  
  iftImage *aux   = iftImageBasins(img[0],A);
  if (area > 0){
    img[1]          = iftFastAreaClose(aux, area);
    iftDestroyImage(&aux);
  } else {
    img[1] = aux;
  }
  
  /* Get regional minimas */
  iftImage *regmin = iftRegionalMinima(img[1]);

  mask  = iftSelectImageDomain(mimg->xsize,mimg->ysize,mimg->zsize);
  /* Seed sampling */
  seed = iftGridSampling(mimg,mask,nsuperpixels);

  /* Compute ISF_REGMIN superpixels */
  S=NULL;
  l=1;
  for (p=0; p < seed->n; p++) 
    if (seed->val[p]!=0){
      iftInsertLabeledSet(&S,p,l);
      l++;
    }

  iftForceMinima(&S,regmin,C);

  iftDestroyAdjRel(&B);
  B    = iftCircular(1.5);
  fst  = iftCreateImageForest(img[1], B);

  iftDiffWatershed(fst, S, NULL);
  
  *nseeds = iftMaximumValue(fst->label);

  label = iftCopyImage(fst->label);

  iftDestroyImage(&regmin);
  iftDestroyAdjRel(&A);
  iftDestroyAdjRel(&B);
  iftDestroyAdjRel(&C);
  iftDestroyLabeledSet(&S);
  iftDestroyImage(&mask);
  iftDestroyImage(&seed);
  iftDestroyMImage(&mimg);

  iftDestroyImageForest(&fst);
  iftDestroyImage(&img[0]);
  iftDestroyImage(&img[1]);

  return label;
}

int main(int argc, char *argv[]) 
{
  iftImage  *img, *label;
  iftImage  *border=NULL;
  iftAdjRel *A;
  int        nseeds;
  char       labelfilename[256];
  char       borderfilename[256];
  iftColor   RGB, YCbCr;
  int        normvalue;

  if (argc!=5)
    iftError("Usage: iftISF_REGMIN <image.[pgm,ppm,scn,png]> <nsamples> <area_close (e.g 50)> <output_image>","main");
  
  img  = iftReadImageByExt(argv[1]);

  normvalue =  iftNormalizationValue(iftMaximumValue(img)); 
  
  label = iftCompute_ISF_REGMIN_Superpixels(img, atoi(argv[2]), atoi(argv[3]), &nseeds);
  
  border  = iftBorderImage(label);
  
  /* Write superpixel segmentation */
  RGB.val[0] = 0;
  RGB.val[1] = normvalue;
  RGB.val[2] = normvalue;
  A = iftCircular(0.0);    
  YCbCr      = iftRGBtoYCbCr(RGB, normvalue);
  sprintf(labelfilename, "%s.pgm", argv[4]);
  iftWriteImageP2(label,labelfilename);
  iftDrawBorders(img,border,A,YCbCr,A);
  sprintf(borderfilename, "%s.png", argv[4]);
  iftWriteImageByExt(img, borderfilename);

  /* Print number of iterations and superpixels */
  printf("Number of superpixels = %d \n", nseeds);
  
  /* Free */
  iftDestroyImage(&img);
  iftDestroyImage(&label);
  iftDestroyImage(&border);
  iftDestroyAdjRel(&A);

  return(0);
}
