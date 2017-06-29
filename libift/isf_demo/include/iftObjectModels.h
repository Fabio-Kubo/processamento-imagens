#ifndef IFT_OBJECTMODELS_H_
#define IFT_OBJECTMODELS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dirent.h>
#include "iftCommon.h"
#include "iftDataSet.h"
#include "iftFile.h"
#include "iftFImage.h"
#include "iftIGraph.h"
#include "iftImage.h"
#include "iftMatrix.h"
#include "iftRegistration.h"
#include "iftRepresentation.h"
#include "iftString.h"


/**
 * @brief Flag to indicate a Classification Algorithm
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 */
typedef enum {
    IFT_NONE, IFT_OPF, IFT_LOGREG
} iftClassificationAlg;


/**
 * @brief Indicates the Policy of Object/Label Processing.
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ObjectModels
 */
typedef enum {
    /** Process each object/label one by one separately */
    IFT_ONE_BY_ONE,
    /** Process all objects/labels at once */
    IFT_ALL_AT_ONCE
} iftObjProcPolicy;


/**
 * Extra (plus) features of our Object Model for a single Object or for multiple objects at once.
 * @author Samuel Martins
 * @date Aug 28, 2016
 * @ingroup ObjectModel
 */
typedef struct ift_obj_model_plus {
    /** Factor used to smooth the training and testing images */
    float smooth_factor;
    /** Texture Classifier (optional) */
    void *clf;
    /** Indicates which is the considered texture classifier */
    iftClassificationAlg clf_alg;
} iftObjModelPlus;


/**
 * @brief Struct to represent a Multi-Object Model (SOSM or Fuzzy).
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 */
typedef struct ift_obj_model {
    /** Prior Prob. Map (Model) */
    iftFImage *model;
    /** Label Map used to identify the label of each voxel of the Prior Prob. Map */
    iftImage *label_map;
    /** Reference Image (required for SOSM) */
    iftImage *ref_img;
    /** Array of Extra (Plus) features, one for each Object Model.
     * For the Multi-Objects case, the array has only one position ([0]) with its extra features */
    iftObjModelPlus **plus;
    /** Labels of the Multi-Object Model */
    iftIntArray *labels;
    /** Policy of Object Processing/Segmentation */
    iftObjProcPolicy seg_pol;
    /** Image Depth (in bits): 8, 12, 16, ... */
    int img_depth;
} iftObjModel;


/**
 * @brief Creates/Allocates a Multi-Object Model.
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 * @return An allocated Multi-Object Model.
 */
iftObjModel *iftCreateObjModel();


/**
 * @brief Destroys a Multi-Object Model.
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 */
void iftDestroyObjModel(iftObjModel **obj_model);


/**
 * @brief Writes a Multi-Object Model
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 */
void iftWriteObjModel(const iftObjModel *obj_model, const char *path);


/**
 * @brief Reads a Multi-Object Model
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 */
iftObjModel *iftReadObjModel(const char *path);


/**
 * @brief Builds a Multi Statistical Object Shapel Model from a set of Registered Ground Truths.
 * @author Samuel Martins
 * @date Aug 21, 2016
 * @ingroup ObjectModel
 *  
 * @param  label_set     Set with the pathnames of the Registered Ground Truths to be used to build the SOSM.
 * @param  labels        Array of Labels which will be constructed the simple models.
 * @param  ref_img       Reference Image where all Label Images were registered.
 * @param  seg_pol       Object Segmentation Policy: ONE_BY_ONE or ALL_AT_ONCE.
 * @return               Multi Statistical Object Shape Model.
 */
iftObjModel *iftBuildSOSM(const iftFileSet *label_set, const iftIntArray *labels, const iftImage *ref_img,
                          iftObjProcPolicy seg_pol);


iftObjModel *iftBuildSOSM_A(const iftFileSet *label_set, const iftIntArray *labels, const iftImage *ref_img,
                          iftObjProcPolicy seg_pol, const iftFileSet *markers_paths,
                          iftClassificationAlg clf_alg, float smooth_factor);


/**
 * @brief Finds the Seeds for delineation of the Background and of each Target Object from a Multi-Object Model.
 * @author Samuel Martins
 * @date Jun 2, 2016
 * 
 * @param  model                   Prior Probability Map (Model).
 * @param  model_label_map         Label map from the model.
 * @param  e_radius                Erosion Radius that used to find the Inner Seeds (Certain Region Border of the Target Object).
 * @param  d_radius                Radius Dilation that is used to find the Outer Seeds (Certain Region Border of the Background).
 * @param  out_bg_certainty_region Returns by Reference (if != NULL) all voxels of the certainty regions (probability of 1)
 *                                 of the background.
 * @return                         Returns the seeds for each target object including the background.
 */
iftLabeledSet *iftFindModelSeeds(const iftFImage *model, const iftImage *model_label_map,
                                 double e_radius, double d_radius, iftSet **out_bg_certainty_region);


iftLabeledSet *iftFindFilteredModelSeeds(const iftFImage *model, const iftImage *model_label_map,
                                         const iftImage *clf_mask, double e_radius, double d_radius,
                                         iftSet **out_bg_certainty_region);

iftImage *iftSegmentBySOSM(iftImage *test_img, const iftObjModel *obj_model, iftGradientAlg grad_alg,
                           float beta, float e_radius, float d_radius);


iftImage *iftSegmentBySOSM_A(iftImage *test_img, const iftObjModel *obj_model, iftGradientAlg grad_alg,
                           float alpha, float beta, float e_radius, float d_radius);



/**
 * @brief Registers an Object Model to a Coordinate Space.
 *
 * This functions register the Reference Image from the Object Model to a new Coordinate Space (Fixed Image). \n
 * The resulting Deformation Fields are then used to map the model and label map from the Object Model. \n
 * The Fixed Image becomes is also stored in the Object Model as the new Reference Image.
 * All process is done in place.
 * 
 * @param obj_model           Object Model to be registered. All registration is done in place.
 * @param fixed_img           Coordinate Space (Fixed Image) where the object model will be registered. 
 * @param affine_params_path  Pathname from the Affine Parameter File.
 * @param bspline_params_path Pathname from the BSpline Parameter File.
 */
void iftRegisterObjModelByElastix(iftObjModel *obj_model, const iftImage *fixed_img,
                                  const char *affine_params_path, const char *bspline_params_path);













//========================================================================================================
/**
 * @brief Strategy of the Optimization Function: Minimizing or Maximizing it
 * @author Samuel Martins
 * @date May 2, 2016
 * @ingroup ObjectModels
 */
typedef enum {
    IFT_MINIMIZATION,
    IFT_MAXIMIZATION
} iftOptStrategy;


/**
 * @brief Structure used for storing common data for object recognition functionals.
 * @author Thiago Spina
 * @author Samuel Martins
 * @date May 2, 2016
 * @ingroup ObjectModels
 */
typedef  struct ift_OM_recog_alg_data {
    /** Optimization Strategy used by the Score Functions.Non-zero for maximization and 0 for minimization */
    iftOptStrategy opt_str;
    /** Pointer to custom data for each recognition algorithm */
    void *custom_data;
    /** Expected Best Score for the Optimization Strategy @ref opt_str, which will be used by the Score Function */
    float best_score;
    /** Expected Worst Score for the Optimization Strategy @ref opt_str, which will be used by the Score Function */
    float worst_score;
} iftOMRecogAlgData;


/**
 * @brief Structure used for storing common data for delineation algorithms.
 * @author Thiago Spina
 * @author Samuel Martins
 * @date May 2, 2016
 * @ingroup ObjectModels
 */
typedef struct ift_OM_delin_alg_data {
    /** The original image to be delineated */
    iftImage *orig;
    /** Pointer to custom data for each delineation algorithm */
    void *custom_data;
} iftOMDelinAlgData;


/**
 * @brief The delineation custom data structure that stores information required for Watershed-based delineation.
 */
typedef struct ift_OM_delin_alg_watershed_data {
    /** Voxels on the object's boundary (used to determine the boundary between the objects) */
    iftBMap *border_processed;
    /** Region in which Watershed is allowed to run (optional) */
    iftBMap *mask;
    /** MST of the original image */
    iftIGraph *MST;
    /** Forest used by watershed */
    iftImageForest *fst;
} iftOMDelinAlgWatershedCustomData;


/**
 * @brief Structure for performing object search and segmentation via MSPS using the seeds computed by an object model (e.g.,
 * fuzzy or statistical).
 */
typedef struct iftObjectModelProb {
    /**
     * A generic pointer to the original model so that we may do post-processing whenever
     * necessary (e.g., the recognition algorithm may need all of the internal
     * object voxels to be labeled accordingly, which requires the original OM)
     */
    void *model;
    /** Stores the displacement vector for all objects and the background seeds. */
    iftAdjRel **seed_adj;
    /**  Stores the uncertainty region displacement vectors. */
    iftAdjRel *uncertainty_adj;
    /** The labels of each object represented in the object model including the background */
    iftIntArray *labels;
    /** Delineation Data used to Segment the Images */
    iftOMDelinAlgData *delineation_data;
    /** Recognition Data used to Segment the Images */
    iftOMRecogAlgData *recognition_data;
    /** Search region for the Optimization Algoritgm */
    iftBMap *search_region;

    /**
     * Function called a single time before starting the optimization via MSPS for initializing the basic
     * data used for delineation (e.g., setting the cost of all voxels to -infinity for IFT-SC).
     */
    void (*iftDelinAlgInit)(iftOMDelinAlgData *data);
    /**
     * @details Function called just before delineation. It may be used to convert the seed displacements to actual
     * seed voxels in the target image domain, for delineation algorithms such as IFT-SC. In this case, this function
     * considers that all object seed and uncertainty regio displacements represented in the object model refer to
     * a common center, regardless of their labels.
     *
     * @date Feb 10, 2016
     *
     * @param prob        A pointer to the this OM problem.
     * @param cur_pos     The current position of the model, which is common to all adjacency relations.
     * @param S           Returns the seed set in the target image's coordinates if not NULL.
     * @param Uncertain   Returns the unceratinty region voxels in the target image's coordinates, if not NULL.
     */
    void (*iftDelinAlgIterationSetup)(struct iftObjectModelProb *prob, iftVoxel *cur_pos, iftLabeledSet **S,
                                      iftSet **Uncertain);
    /**
     * Delineation algorithm.
     *
     * @param data        A pointer to the data required by the delineation algorithm.
     * @param seeds       The seed set from the object model placed at the target image's coordinates.
     * @param uncertainty The uncertainty region voxels placed on the target image's coordinates.
     * @param objects     It MUST be a pointer to a valid seed set that will be set with all of the voxels that belong
     *                    to the object according to the delineation algorithm.
     * @param borders     An OPTIONAL parameter that returns the delineation boundary for algorithms that do so
     *
     */
    void (*iftDelinAlgorithm)(iftOMDelinAlgData *data, iftLabeledSet *seeds, iftSet *uncertainty,
                              iftLabeledSet **objects, iftLabeledSet **borders);
    /**
     * Function called after delineation for performing post-processing when required by the recognition algorithm.
     * Post-processing may fill the whole in segmentation caused by running the delineation algorithm only inside
     * the uncertainty region, for instance.
     */
    void (*iftDelinAlgPostProcessing)(void *model, iftOMRecogAlgData *data,
                                      iftLabeledSet **processed, iftLabeledSet **borders);
    /**
     * Function called after delineation and recognition were performed for resetting exclusively the voxels processed
     * by delineation, to avoid resetting unnecessary data.
     */
    void (*iftDelinAlgReset)(iftOMDelinAlgData *data, iftLabeledSet *processed);

    /**
     * @details Function called just before delineation that may be used to initialize the recognition algorithm for the
     * current iteration.
     *
     * @date Feb 18, 2016
     *
     * @param prob        A pointer to the this OM problem.
     * @param cur_pos     The current position of the model, which is common to all adjacency relations.
     */
    void (*iftRecogAlgIterationSetup)(struct iftObjectModelProb *prob, iftVoxel *cur_pos);

    /**
     * @brief The Recognition algorihm.
     * @param prob        A pointer to the this OM problem.
     * @return The object recognition score
     */
    float (*iftRecogAlgorithm)(struct iftObjectModelProb *prob, iftLabeledSet *processed, iftLabeledSet *borders);
} iftObjectModelProb;


/**
 * @brief Allocates a generic iftOMDelinAlgData record containing.
 *
 * This function basically facilitates the generic allocation of the data contained in it. It should be used by functions
 * that create delineation data for specific algorithms.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param fst The image forest record required by the seeded watershed transform.
 * @param custom_data The custom data specific for a given delineation algorithm.
 * @return The allocated data structure for iftOMDelinAlgData.
 *
 * @warning The delineation structure should be deleted with a specific destructor, since <custom_data> may represent
 * distinct data.
 */
iftOMDelinAlgData *iftCreateOMDelinAlgData(iftImage *orig, void *custom_data);


/**
 * @brief Allocates an iftOMDelinAlgData record containing the necessary data for performing seeded
 * watershed delineation for a given Object Model.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param fst The image forest record required by the seeded watershed transform.
 * @return The allocated data structure for iftOMDelinAlgData.
 */
iftOMDelinAlgData *iftCreateOMDelinAlgWatershedData(iftImage *orig, iftImageForest *fst);


/**
 * @brief Destroys the data allocated with iftCreateOMDelinAlgWatershedData and nullifies the input pointer.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data The address of the pointer to data structure iftOMDelinAlgData
 */
void iftDestroyOMDelinAlgWatershedData(iftOMDelinAlgData **data);


/**
 * @brief Allocates an iftOMRecogAlgData record containing the necessary data for performing recognition.
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 * @ingroup ObjectModels
 *
 * This function basically facilitates the generic allocation of the data contained in it. \n
 * It should be used by functions that create delineation data for specific algorithms.
 *
 * @param custom_data The custom data specific for a given recognition algorithm.
 * @param opt_str The Optimization Strategy that will be used for the Recognition Algorithm (Score Function). *
 * @return The allocated Recognition Algorithm Data.
 *
 * @warning Since the function does not copy the input custom data (it only points to it),
 * the resulting Recognition Algorithm Data should be deleted with a specific destructor if necessary
 * in order to also deallocate its custom data.
 */
iftOMRecogAlgData *iftCreateOMRecogAlgData(void *custom_data, iftOptStrategy opt_str);


/**
 * @brief Allocates an iftOMRecogAlgData record containing the necessary data for performing recognition.
 *
 * It computes the mean cut on the object's boundary as a recognition score, which we expect to be maximum when the
 * delineation result is accurate.
 *
 * @param fst The forest produced by IFT-watershed.
 * @return The allocated record.
 * 
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 */
iftOMRecogAlgData *iftCreateOMRecogAlgDataMeanCut(iftImageForest *fst);


/**
 * @brief Deallocates the data used by the recognition algorithm.
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data A pointer to the data required by the recognition algorithm.
 */
void iftDestroyOMRecogDataMeanCut(iftOMRecogAlgData **data);


/**
 * @brief Allocates the data structure iftObjectModelProb.
 *
 * One has to set the fields related to the delineation and recognition algorithms manually if s/he is not
 * interested in using Watershed + mean cut.
 *
 * @warning No parameter is copyied, they are only assigned.
 *
 * @author Thiago V. Spina
 * @author Samuel Martins
 * @date May 17, 2016
 *
 * @param model                      A pointer to the model.
 * @param seed_adj          The OM's seed set for each object and the background. They will be copied by the model.
 * @param uncertainty_adj   The uncertainty region of the model. It will be copied by the model.
 * @param labels           The labels for each adjacency relation in seed_adj representing the objects. It will be copied by the model.
 * @param nlabels          The number of labels represented by the Object Model
 * @param delineation_data A pointer to the data required by the delineation algorithm (by default we use the Watershed
 *                         transform). The model does not take ownership of the data, since only the calling function
 *                         knows how to delete it.
 * @param recognition_data A pointer to the data required by the recognition algorithm (by default we use the Watershed
 *                         transform's mean cut). The model does not take ownership of the data, since only the calling function
 *                         knows how to delete it.
 * @param search_region    The search region of the image where the model is allowed to perform optimization. The model
 *                         takes ownership of it and is responsible for deleting it.
 * @return                 The allocated data
 */
iftObjectModelProb *iftCreateObjectModelProb(void *model, iftAdjRel **ALabels, iftAdjRel *AUncertain,
                                             iftIntArray *labels, iftOMDelinAlgData *delineation_data,
                                             iftOMRecogAlgData *recognition_data, iftBMap *search_region);


/**
 * @brief Deallocates the iftObjectModelProb pointer and sets it to NULL.
 *
 * Except for the adjacency relations, all of the remaining data that was passed in
 * iftCreateObjectModelProb remains unchanged since they are NOT owned by iftObjectModelProb.
 *
 * @author Thiago V. Spina
 * @author Samuel Martins
 * @date May 17, 2016
 */
void iftDestroyObjectModelProb(iftObjectModelProb **prob);

///////////////////////////// OLD CODES TO BE REMOVED //////////////////////////////////


typedef struct ift_ref_space {
    int xsize;
    int ysize;
    int zsize;
    float dx;
    float dy;
    float dz;
    int depth;
    iftVoxel model_center;
} iftRefSpace;


typedef struct ift_fuzzy_model_extended {
  iftFImage *model;
  iftAdjRel *AIn;
  iftAdjRel *AOut;
  iftAdjRel *AUncertain;
  iftVoxel  start;
  iftVoxel  end;
  float     mean;
  float     deviation;
  int       mode;
  iftRefSpace *ref_info;
  int obj_label;
} iftFuzzyModelExtended;

typedef struct ift_statistic_model_simple {
  iftFImage *model;
  float     mean;
  float     deviation;
  int       mode;
} iftStatisticModelSimple;

iftImage             *iftCropObjectLoose(iftImage *obj, int xLSize, int yLSize, int zLSize);
iftRefSpace *iftModelDomain(const iftFileSet *label_paths, int obj_label, int *xsize, int *ysize, int *zsize);
void iftModelDomainProbAtlas(const iftFileSet *label_paths, int obj_label, int *xsize, int *ysize, int *zsize);
int                  iftHasUncertain6Adjacent(iftFImage *model, int i);
// void iftModelAdjacencies(iftFImage *model, iftAdjRel **AIn, iftAdjRel **AOut, iftAdjRel **AUncertain);
void iftModelAdjacencies(iftFImage *model, iftAdjRel **AIn, iftAdjRel **AOut, iftAdjRel **AUncertain,
                         double e_radius, double d_radius);
// void                 iftFuzzyByAveraging(iftFImage *model, fileList *imageFiles);
// void                 iftFuzzyByDistanceTransform(iftFImage *model, fileList *imageFiles);
void                 iftComputeFuzzyModel(char *dirname, iftFImage *model, char option);
void                 iftWriteFuzzyModelExtended(const iftFuzzyModelExtended *fuzzyModel, const char *filename);
iftFuzzyModelExtended *iftReadFuzzyModelExtended(const char *filename);
void                 iftModelPositionsFuzzyExtended(iftFileSet *img_paths, iftFileSet *label_paths,
                                                    iftFuzzyModelExtended *fuzzyModel);
void                 iftModelPositionsProbAtlas(iftFileSet *img_paths, iftFileSet *label_paths,
                                                    iftFuzzyModelExtended *fuzzyModel);
void	             iftWriteStatisticModelSimple(iftStatisticModelSimple *statisticModel, char *filename);
void                 iftDestroyStatisticModelSimple(iftStatisticModelSimple **statisticModel);
iftStatisticModelSimple *iftReadStatisticModelSimple(char *filename);
void                 iftDestroyFuzzyModelExtended(iftFuzzyModelExtended **fuzzyModel);
void                 iftComputeHistogramParameters(char *dirImages, char *dirInstances, float *meanP, int *modeP, float *sdP);
// void                 iftFuzzyByAveragingNoGCCentering(iftFImage *model, fileList *imageFiles);
void                 iftModelIntensitiesFuzzyExtended(iftFileSet *img_paths, iftFileSet *label_paths,
                                                      iftFuzzyModelExtended *fuzzyModel);
void                 iftFuzzyByAveragingDilated(iftFImage *model, iftFileSet *img_paths, int obj_label);
iftStatisticModelSimple *iftMakeStatisticModelSimple(char *imagesDirectory, char *segmentationsDirectory);
iftImage             *iftSegmentWithStatisticModelSimple(char *imageRoute, char *modelRoute);
iftImage             *iftAPosterioriGradientOld(iftImage *image, iftFuzzyModelExtended *model,
                                                float alpha);
iftImage             *iftAPosterioriGradientWithClassicSoam(iftImage *image, iftStatisticModelSimple *model);


/**
@brief It finds the affine transformation (translation and rotation)
by PCA that aligns a given object (binary image) with its principal
axes and returns the transformation matrix. Use iftTransformImage to
apply the transformation to any image.
@author Alexandre Falcao
@date   Nov 17th 2015
@param  A binary image with the object for alignment
@return An image transformation matrix
**/

iftMatrix *iftObjectAlignMatrixByPCA(iftImage *bin);



/**
 * @brief Computes the intensity mode and deviation of all voxels in a given set of image files, constrained to the
 * corresponding binary masks.
 *
 * @author Thiago Vallin Spina
 * @date Jan 23, 2016
 *
 * @param img_files The set of image files whose voxel intensities will be considered.
 * @param mask_files The set of binary image masks that constrain the computation of the voxel intensities.
 * @param intensities_mode The pointer to a variable that will be assigned the mode of the intensities.
 * @param intensities_mode_deviation The pointer to a variable that will be assigned the deviation to the mode.
 */
void iftImageSetVoxelIntensityStatistics(const iftFileSet *img_files, const iftFileSet *mask_files,
                                         double *intensities_mode, double *intensities_mode_deviation,
                                         int normalization_value);

iftFImage *iftEnhanceImageByGaussian(iftImage *image, double value, double deviation);

iftImage *iftGaussianEnhancedImageGradient(iftImage *image, iftAdjRel *A, double value, double deviation, double alpha);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Delineates an image with an object model by calling the functions that place seeds and then delineate the objects.
 *
 * Multiple objects may be segmented simultaneously if supported by the selected delineation algorithm. In this case,
 * this function calls the iteration setup function to place the seeds on the test image. This function also supports
 * multiple distinct centers, one for each object of interest if necessary.
 *
 *
 * @param prob The object model problem.
 * @param cur_pos An array of object centers, one for each object of interest or a single center common to all.
 * @param objects     The object and background voxels after delineation occurs. This parameter is only set if a non-NULL
 *                    address is passed.
 * @param borders     The voxels on the border between objects and background. This parameter is only set if a non-NULL
 *                    address is passed.
 *
 */
void iftMSPSDelineateObjectWithModel(iftObjectModelProb *prob, iftVoxel *cur_pos, iftLabeledSet **objects,
                                     iftLabeledSet **borders);

/**
 * @brief Initializes an MSPS structure with the necessary data for the object model search by translating a single center.
 *
 * @author Thiago Vallin Spina
 * @date Feb 16, 2016
 *
 * @param prob The object model problem.
 * @param nscales The number pof MSPS scales
 * @param min_disp The minimum displacement used in the first scale.
 * @param scale_disp_increment The the increment used to compute the x/y/z displacement for higher scales.
 * @return The initialized MSPS data structure.
 */
iftMSPS *iftMSPSObjectModelSearchByTranslationSingleCenterInit(iftObjectModelProb *prob, int nscales,
                                                               int min_disp, int scale_disp_increment);

/**
 * @brief Performs the MSPS optimization to search for the object via translation.
 *
 * @param msps The initialized MSPS data structure.
 * @param start The first voxel where the object search should start from.
 * @param best_pos Returns the best position for the model.
 * @return The best score obtained during the search.
 */
float iftMSPSObjectModelSearchByTranslationSingleCenterOptimization(iftMSPS *msps, iftVoxel start, iftVoxel *best_pos);


/** Functions for Watershed-based MSPS object search **/





/**
 * @brief Initializes the data for IFT-watershed by setting the optimum-path costs as -infinity to all voxels.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data A pointer to the data required by the Watershed delineation algorithm.
 */
void iftOMDelinWatershedInit(iftOMDelinAlgData *data);

/**
 * @brief Prepares the delineation data of the fuzzy object model for watershed-based segmentation.
 *
 * The OM's seeds are converted to seed sets for Watershed.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param prob       A pointer to the object model problem to allow the seed placement algorithm to get its delineation data and other info.
 * @param seed_adj    The OM's seed set for each object and the background.
 * @param uncertainty_adj The uncertainty region of the model.
 * @param labels     The labels for each adjacency relation in seed_adj representing the objects.
 * @param nlabels    The number of labels represented by the Object Model
 * @param cur_pos    The current position of the center of the model.
 * @param S          The labeled seed set returned by the function to be used in Watershed. Note: it initially sets *S
 *                   to NULL.
 * @param Uncertain  The uncertainty region set returned by the function to be used in Watershed. Note: it initially
 *                   sets *Uncertain to NULL.
 */
void iftOMDelinWatershedIterationSetupSingleCenter(iftObjectModelProb *prob, iftVoxel *cur_pos, iftLabeledSet **S,
                                                   iftSet **Uncertain);

/**
 * @brief Performs IFT-watershed delineation and returns the object voxels and borders if the parameters are set to
 * non-NULL addresses.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data        A pointer to the data required by the Watershed delineation algorithm.
 * @param seeds       The object and background seeds for the Watershed delineation algorithm.
 * @param uncertainty The uncertainty region voxels.
 * @param objects     The object and background voxels after delineation occurs. This parameter is only set if a non-NULL
 *                    address is passed.
 * @param borders     The voxels on the border between objects and background. This parameter is only set if a non-NULL
 *                    address is passed.
 */
void iftOMDelinWatershed(iftOMDelinAlgData *data, iftLabeledSet *seeds,
                         iftSet *uncertainty, iftLabeledSet **objects, iftLabeledSet **borders);


/**
 * @brief Performs IFT-watershed delineation on a MST graph.
 *
 * This function returns the object voxels and borders if the parameters are set to non-NULL addresses.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data        A pointer to the data required by the Watershed delineation algorithm.
 * @param seeds       The object and background seeds for the Watershed delineation algorithm.
 * @param uncertainty The uncertainty region voxels.
 * @param objects     The object and background voxels after delineation occurs. This parameter is only set if a non-NULL
 *                    address is passed.
 * @param borders     The voxels on the border between objects and background. This parameter is only set if a non-NULL
 *                    address is passed.
 *
 * @warning The MST field in the custom delineation data must have been computed prior to calling this function.
 */
void iftOMDelinWatershedOnMST(iftOMDelinAlgData *data, iftLabeledSet *seeds,
                              iftSet *uncertainty, iftLabeledSet **objects, iftLabeledSet **borders);
/**
 * @brief Resets the delineation data under the fuzzy object model (i.e., resets the processed voxel costs to -infinity).
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data      A pointer to the data required by the Watershed delineation algorithm.
 * @param processed The set of processed voxels by IFT-watershed.
 */
void iftOMDelinWatershedReset(iftOMDelinAlgData *data, iftLabeledSet *processed);


/**
 * @brief Performs recognition by computing the mean cut of the watershed transform. The function evaluates the border
 * voxels to this end.
 *
 * @author Thiago V. Spina
 * @date Oct 26, 2015
 *
 * @param data      A pointer to the data required by the recognition algorithm.
 * @param processed The set of object/background voxels that were delineated by IFT-watershed. This is not used by this function.
 * @param borders   The set of voxels on the border between object and background.
 * @return The mean cut.
 */
float iftOMRecogAlgWatershedMeanCut(iftObjectModelProb *prob, iftLabeledSet *processed,
                                    iftLabeledSet *borders);

#ifdef __cplusplus
}
#endif

#endif


