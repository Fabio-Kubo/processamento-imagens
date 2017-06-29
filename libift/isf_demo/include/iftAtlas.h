/**
 * @file iftAtlas.h
 * @brief Definitions and functions about Atlas-based models.
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ProbAtlas
 *
 * @note Programs:
 * * @ref iftBuildClassicProbAtlas.c = Builds a Classic Probabilistic Atlas from a set of Images and their Labels.
 */

#ifndef IFT_ATLAS_H
#define IFT_ATLAS_H

#ifdef __cplusplus
extern "C" {
#endif


#include "iftAdjacency.h"
#include "iftBMap.h"
#include "iftCommon.h"
#include "iftDict.h"
#include "iftFImage.h"
#include "iftImage.h"
#include "iftJson.h"
#include "iftObjectModels.h"
#include "iftSegmentation.h"
#include "iftSeeds.h"


/** Padding added around the Cropped Prob. Atlas to enable dilation */
#define IFT_PROB_ATLAS_PADDING 21


/**
 * @brief Indicates the Segmentation Algorithm used for Prob. Atlases.
 * @author Samuel Martins
 * @date May 4, 2016
 * @ingroup ProbAtlas
 * 
 * @note More information about these algorithms can be found in:
 * @url http://scitation.aip.org/content/aapm/journal/medphys/43/1/10.1118/1.4938577
 */
typedef enum {
    /** Simple Thresholding over the Prior Probability Map */
    IFT_CLASSICAL_ATLAS_SEG,
    /** Segmentation using IFT-SC on the best (high score) location on Test Image found using MSPS optimizer */
    IFT_MSPS_ATLAS_SEG
} iftAtlasSegAlg;


/**
 * @brief Options of Tie-Break for Label Collision, that is, when Two or More Objects of several 
 * Label images are in a same coordinate or it has several labels resulting of segmentations.
 * It is only used if the All labels are processed At Once (IFT_PROCESS_ALL).
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ObjectModels
 */
typedef enum {
    /** The object with lowest volume is chosen. */
    IFT_LOWEST_VOLUME_OBJ,
    /** The object with highest volume is chosen. */
    IFT_HIGHEST_VOLUME_OBJ,
    /** A random object is chosen. */
    IFT_RANDOM_OBJ
} iftLabelTieBreakPolicy;


/**
 * @brief Parameters of a Gaussian.
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ObjectModels
 */
typedef struct ift_gauss_params {
    /** Mean Parameter of the Gaussian */
    double mean;
    /** Standard Deviation Parameter around the mean for Gaussian Computation */
    double stdev;
} iftGaussParams;


/**
 * @brief Seeds of the Target Objects (inner) and Background (outer), obtained from the Models,
 * that are required for Seed-based Delineation Algorithms.
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ObjectModels
 */
typedef struct ift_model_seeds {
    /** Array of Inner Model (Target Object) Seeds. For only one Object Model, there will be only one
    instance. */
    iftAdjRel **in;
    /** Outter Model (Background) Seeds (union of the outer seeds of all objects, if the
    Label Proc. Policy is IFT_ALL_AT_ONCE) */
    iftAdjRel *out;
} iftModelSeeds;


/**
 * @brief Information about the Reference Space/Image where the model was created.
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ProbAtlas
 */
typedef struct ift_ref_space_info {
    /**
     * Reference Space/Image Domain
     * @{
     */
    int xsize;
    int ysize;
    int zsize;
    /** @} */
    /**
     * Voxel Sizes from the Reference Space/Image
     * @{
     */
    // Voxel Sizes
    float dx;
    float dy;
    float dz;
    /** @} */
    /** Image Depth */
    int depth;
    /** Coordinate in the Reference Image Domain from the Central Point each Model (Prior Prob Atlas) */ 
    iftVoxel *model_centers;
} iftRefSpaceInfo;


/**
 * @brief Information about the Training Images used to build the Prob. Atlas.
 * @author Samuel Martins
 * @date May 1, 2016
 * @ingroup ProbAtlas
 */
typedef struct ift_model_train_imgs_info {
    /** Number of Training Label Image used to build the Prob. Atlas */
    size_t n_imgs;
    /** Mean volumes for each label (total of n_objs) computed from all training label images */
    iftDblArray *mean_vols;
} iftModelTrainImgsInfo;


/**
 * @brief Probabilistic Atlas for Multiple Objects.
 * @author Samuel Martins
 * @date Mar 31, 2016
 * @ingroup ProbAtlas
 *
 * @warning It is expected that the Label Images used to construct the Prob. Atlas Multi Label has labels
 * in the field <b>labels</b>.
 * @warning The background has always label 0.
 *
 * For each object <b>labels[i]</b> (total of <b>n_objs</b>), it will be constructed a single Prob. Atlas
 * for it whose maps, seeds, etc, are stored at index [i] of the corresponding fields. \n
 * The Merge of ALL single Prob. Atlas is stored at index [n_objs].
 */
typedef struct ift_prob_atlas {
    /** Policy to untie the Tie-Break due to the Label Overlapping */
    iftLabelTieBreakPolicy tie_break_pol;
    /** Prior Probabilistic Maps, one for each object. Index [n_objs] stores the Combined Prob. Atlas */
    iftFImage **models;
    /** Label Map used to identify the label of each voxel of the Combined Prior Prob. Map */
    iftImage *label_map;
    /** Information about the Reference Space/Image where the model was created */
    iftRefSpaceInfo *ref_info;
    /** Information about the Training Images used to build the Prob. Atlas */
    iftModelTrainImgsInfo *train_info;
    /** Seeds of the Background and Target Objects, one for each object, that are required in
    Seed-based Delineation Algorithms. Index [n_objs] stores the Combined Prob. Atlas*/
    iftModelSeeds **seeds;
    /** Region of Uncertainty of the Models */
    iftAdjRel **unc_regions;
    /** Parameters to Gaussian PDF Estimation of each Object Model that are required to compute P(x / v in O), i.e.,
     * the Conditional Probability of a voxel v has brightness x given it belongs to the object O.
     * It for the Delineation Algorithms in the Bayes' Theorem. */
    iftGaussParams **pdf;
    /** Search Regions of the Models required for Optimization Algorithms during delineation. */
    iftBMap **search_regions;
    /** Number of Objects */
    size_t n_objs;
    /** The labels of each object represented in the object model (total of n_objs labels). */
    int *labels;
} iftProbAtlas;


/**
 * @brief Struct to represent a Simple Prob. Atlas for Multiples Objects, which only corresponds to
 * a prior prob. map (model) and their Label Map.
 * @author Samuel Martins
 * @date Jun 29, 2016
 * @ingroup ProbAtlas
 */
typedef struct ift_simple_prob_atlas {
    /** Prior Prob. Map (Model) */
    iftFImage *model;
    /** Label Map used to identify the label of each voxel of the Combined Prior Prob. Map */
    iftImage *label_map;
} iftSimpleProbAtlas;


/**
 * @brief Generic Struct that stores the Parameters of an Atlas-based Segmentation Algorithm.
 * @author Samuel Martins
 * @date May 9, 2016
 * @ingroup ProbAtlas
 */
typedef struct ift_atlas_seg_params {
    /** Chosen Atlas-based Delineation Algorithm. */
    iftAtlasSegAlg alg;
    /** Dictionary with the Atlas-based Segmentation Parameters. */
    iftDict *params;
} iftAtlasSegParams;


/**
 * @brief MSPS Problem for Probabilistic Atlas
 * @author Samuel Martins
 * @date May 17, 2016
 * @ingroup ProbAtlas
 */
typedef struct ift_prob_atlas_problem {
    /** Inner Seeds used in the MSPS optimization */
    iftAdjRel *inner_seeds;
    /** Outer Seeds used in the MSPS optimization */
    iftAdjRel *outer_seeds;
    /** Uncertainty Region used in the MSPS optimization */
    iftAdjRel *unc_region;
    /** Search Region used in the MSPS optimization */
    iftBMap *search_region;
    /** Number of Objects */
    size_t n_objs;
    /** The labels of each object represented in the object model (total of n_objs labels). */
    int *labels;
} iftProbAtlasProblem;


/**
 * @brief Destroys/Deallocates Model Seeds.
 * @author Samuel Martins
 * @date Apr 4, 2016
 * @ingroup ObjectModels
 */
void iftDestroyModelSeeds(iftModelSeeds **seeds, int n_objs);


/**
 * @brief Destroys a Probabilistic Atlas.
 * @author Samuel Martins
 * @date Apr 4, 2016
 * @ingroup ProbAtlas
 */
void iftDestroyProbAtlas(iftProbAtlas **atlas);


/**
 * @brief Read a Prob. Atlas.
 * @author Samuel Martins
 * @date Apr 12, 2016
 * @ingroup ProbAtlas
 */
iftProbAtlas *iftReadProbAtlas(const char *path, ...);


/**
 * @brief Write a Probabilist Atlas.
 * @author Samuel Martins
 * @date Apr 7, 2016
 * @ingroup ProbAtlas
 *
 * The pathname could be actually a string format, similarly to printf, which would be used to build
 * the atlas pathname. \n
 * Ex: \n
 * ```
 * char prefix[] = "Prob";
 * char suffix[] = "Atlas";
 * iftWriteProbAtlas(atlas, "%s_%s.tar.bz2", prefix, suffix);
 * ```
 * 
 * @param atlas Probabilist Atlas to be written.
 * @param path Pathname from the Prob. Atlas or format, such as "%s_%s.tar.bz2", that could be built like printf, for example.
 * @param ... Vargs used to build the path.
 * 
 */
void iftWriteProbAtlas(const iftProbAtlas *atlas, const char *path, ...);


/**
 * @brief Create a MSPS Problem using the gradient of an image to be segmented and the Probabilistic Atlas.
 * @author Samuel Martins
 * @date May 17, 2016
 * @ingroup Atlas
 *  
 * @param  grad  Gradient of the Image to be segmented.
 * @param  atlas Probabilistic Atlas that will segment the target image.
 * @return       Prob. Atlas MSPS Problem.
 */
// iftProbAtlasProblem *iftCreateProbAtlasProblem(iftImage *grad, const iftProbAtlas *atlas);


/**
 * @brief Compute Prior Probability Map for the Single (Combined) Prior Prob. Model.
 * It also returns the Label Map and Model Center Voxel if the passed references are != NULL.
 * @author Samuel Martins
 * @date May 1, 2016
 * @ingroup ProbAtlas
 *
 * @param tie_break_pol Policy to untie the Label Tie-Breaks
 * @param models Array of Models (Prior Prob. Maps) with <n_objs> elements
 * @param n_objs Number of Objects
 * @param labels Array of the considered labels with <n_objs> elements
 * @param ref_info Reference Space Info elements
 * @param mean_vols Array with the mean volume of each considered object
 * @param out_label_map Return by Reference (if it is != NULL) the corresponding Label Map
 * @param out_model_center_rd Return by reference (if it is != NULL) the corresponding Model Center on the Ref. Image Domain.
 * @return The Single Combined Model (Prior Prob. Map)
 */
iftFImage *iftCompCombinedProbPriorMap(iftLabelTieBreakPolicy tie_break_pol, iftFImage **models,
                                 int n_objs, int *labels, iftRefSpaceInfo *ref_info,
                                 double *mean_vols, iftImage **out_label_map, iftVoxel *out_model_center_rd);


/**
 * @brief Compute Prior Probability Maps for all objects from a set of Images and also
 * compute a Single (Combined) Prior Prob. Map with all these maps.
 * The function also gets the Reference Space Information and Training Images Information, saving them
 * in the Prob. Atlas structure.
 * @author Samuel Martins
 * @date May 10, 2016
 * @ingroup ProbAtlas
 *
 * Firstly, averages all Label Images in their Domains.
 * Then, extracts the model from the Bounding box resulting of the averaging. \n
 * Finally, it expands the cropped model by adding zero frames around it.
 * The Information of the Reference Space and Training Images Information are also returned stored into the Prob. Atlas. \n
 *
 * @note The Merge of All Prior Prob. Atlas is stored at index [n_objs] of each corresponding field.
 * 
 * @param atlas Probabilistic Atlas where the Prior Prob. Maps will be saved.
 * @param label_set Set of Label Images.
 *
 * @warning All Label Images must have the same domain and voxel sizes (Ref. Image). There is no
 * checking for that.
 */
void iftCompPriorProbMap(iftProbAtlas *atlas, const iftFileSet *label_set);


/**
 * @brief Finds the Seeds of the Background and Target Object for each Object Model and for the Single
 * (Combined) Object Model, saving them in the Prob. Atlas.
 * @author Samuel Martins
 * @date Apr 5, 2016
 * @ingroup ObjectModel
 *
 * @note Such are required for Seed-based Delineation Algorithms.
 * 
 * @param atlas Probabilistic Atlas where the Prior Prob. Maps will be saved.
 * @param e_radius Erosion Radius that used to find the Inner Seeds (Certain Region Border of the Target Object).
 * @param d_radius Radius Dilation that is used to find the Outer Seeds (Certain Region Border of the Background).
 */
void iftFindProbAtlasSeeds(iftProbAtlas *atlas, double e_radius, double d_radius);


/**
 * @brief Computes the Parameters of a Gaussian for each Object Model and for the Single
 * (Combined) Object Model, saving them in the Prob. Atlas.
 * @author Samuel Martins
 * @date Apr 27, 2016
 * @ingroup ProbAtlas
 * 
 * Computes the Parameters of a Gaussian for each Object Model from a set of Gray Images and their Labels Images in order
 * to estimate the probability of a voxel <b>v</b> to exhibit a certain gray value <b>x</b>,
 * given that it belongs to a given object Object <b>o</b>. \n
 * Apply a weighted average, based on the mean volume of each object, over the gaussian params of
 * each Object Model in order to obtain the ones for the Single (Combined) Object Model. \n
 * This Gaussian will be used during Prob. Atlas Construction. \n
 *
 * @param atlas Probabilistic Atlas where the Prior Prob. Maps will be saved.
 * @param  img_set   Set of Input (Gray) Images.
 * @param  label_set Set of the corresponding Label Images from the Input Images.
 */
void iftCompGaussParamsFromImgSets(iftProbAtlas *atlas, const iftFileSet *img_set,
                                   const iftFileSet *label_set);


/**
 * @brief Find the Search Region of each Object Model and of the Single
 * (Combined) Object Model, saving them in the Prob. Atlas.
 * from a Set of Label Images.
 * @author Samuel Martins
 * @date Apr 28, 2016
 * @ingroup ProbAtlas
 *
 * The Search Region is basically a Bounding Box that contain the Center Voxel from the Min.
 * Bounding Box of a given Object (Object Model) or of all ones (Single merged Object Model)
 * considering all Label Images. \n
 * The returned BMap has the value 1 on the coordinates of this Bounding Box.
 * 
 * @param atlas Probabilistic Atlas where the Prior Prob. Maps will be saved.
 * @param  label_set Set of Label Images.
 * 
 * @warning Be sure all Label Image Set have the same domain of the Reference Space.
 * @note There is no domain verification between the Ref. Space and the Label Images for reasons of efficiency.
 */
void iftFindProbAtlasSearchRegion(iftProbAtlas *atlas, const iftFileSet *label_set);


/**
 * @brief Builds a Multi Label Probabilistic Atlas from a set of Images and their Label Images.
 * @author Samuel Martins
 * @date Apr 1, 2016
 * @ingroup ProbAtlas
 *
 * This functions builds a Probabilistic Atlas for Multi Objects (total of <b>n_objs</b> objects)
 * from a set of Images and their Label Images. \n
 * The Multi Label Probabilistic Atlas can be constructed according to Two Labels Processing Policies: \n
 * 1) IFT_ONE_BY_ONE = Each Object <b>i</b> has its own Prob. Model and the other fields (gauss params, seeds, etc)
 * at the index [i] \n
 * 
 * 2) IFT_ALL_AT_ONCE = All labels are fused in ONLY ONE Prior Prob. Map and have ONLY ONE instance of the other
 * fields (gauss params, seeds, etc) at index [0]. \n
 * Label Overlapping Ties are solved during Prob. Atlas Construction, according to an user-defined
 * Label Tie-Break Policy) \n\n
 * 
 * The Model Search Regions can be obtained with a Larger Set of Label Images, if it is passed (!= NULL). \n
 * The Gaussian Parameters of the Models can be also obtained with a Larger Set of Gray Images and
 * their Label Images, if they are passed (!= NULL). \n
 * 
 * @warning Be sure that each Image have the corresponding Label Image and vice-versa.
 * @warning Be sure that all (Gray) Images have the given Image Depth <b>img_depth</b>.
 *
 * @param label_pol Label Proc. Policy.
 * @param tie_break_pol Tie-Break Policy for Label Overlapping.
 * @param img_set Set with the pathnames of the Images to be used to build the Prob. Atlas.
 * @param label_set Set with the pathnames of the Label Images to be used to build the Prob. Atlas.
 * @param larger_img_set Directory or CSV file with a larger set of Images that will be used
 *                         to determine the Search Region.
 * @param larger_label_set Directory or CSV file with a larger set of Label Images that will be used
 *                           to determine the Search Region.
 * @param labels Array of Labels.
 * @param img_depth Depth (in bits) from the Input (Gray) Images.
 * @param e_radius Erosion Radius that used to find the Inner Seeds (Certain Region Border of the Target Object).
 * @param d_radius Radius Dilation that is used to find the Outer Seeds (Certain Region Border of the Background).
 */
iftProbAtlas *iftBuildProbAtlas(iftLabelTieBreakPolicy tie_break_pol,
                                const iftFileSet *img_set, const iftFileSet *label_set,
                                const iftFileSet *larger_img_set, const iftFileSet *larger_label_set,
                                iftIntArray *labels, int img_depth, double e_radius, double d_radius);


/**
 * @brief Put the Model on the Ref. Image Domain.
 * @author Samuel Martins
 * @date Apr 14, 2016
 * @ingroup ProbAtlas
 *
 * @param model Prior Prob. Map to be put in the Reference Image Domain.
 * @param ref_info Reference Space Information used.
 * @param iftVoxel Center of the input model on the Reference Image Domain.
 *
 * @note The additional rectangular frames are removed before putting the model on the Ref Domain.
 */
iftFImage *iftModelOnRefDomain(const iftFImage *model, const iftRefSpaceInfo *ref_info,
                              iftVoxel model_center);


/**
 * @brief Put the Label Map on the Ref. Image Domain.
 * @author Samuel Martins
 * @date May 1, 2016
 * @ingroup ProbAtlas
 *
 * @param label_map Label Map to be put in the Reference Image Domain.
 * @param ref_info Reference Space Information used.
 * @param iftVoxel Center of the input model on the Reference Image Domain.
 *
 * @note The additional rectangular frames are removed before putting the label_map on the Ref Domain.
 */
iftImage *iftLabelMapOnRefDomain(const iftImage *label_map, const iftRefSpaceInfo *ref_info,
                                 iftVoxel model_center);


/**
 * @brief Crop the Model (that is an Image) and return its center voxel on the original image.
 * @author Samuel Martins
 * @date May 1, 2016
 * @ingroup ProbAtlas
 * 
 * @param  mapped_model Model Image on an Original Coordinate Space.
 * @param  center       Reference for the Center Voxel from the Cropped Model on the Original Image Domain.
 * @return              Cropped Model Image.
 */
iftImage *iftCropModelImg(const iftImage *mapped_model, iftVoxel *center);


/**
 * @brief Setup the Parameters of the Classical Atlas-based Segmentation.
 * @author Samuel Martins
 * @date May 9, 2016
 * @ingroup ProbAtlas
 *
 * It applies a simple Thresholding over the Prior Probability Map.
 *
 * @note @url http://scitation.aip.org/content/aapm/journal/medphys/43/1/10.1118/1.4938577
 * 
 * @param seg_alg Atlas-based Segmentation Algorithm.
 * @param thres Threshold used for the Classical Atlas-based Segmentation.
 * @return The Atlas Segmentation Parameters.
 */
iftAtlasSegParams *iftSetupClassicalAtlasSeg(iftAtlasSegAlg seg_alg, float thres);


/**
 * @brief Setup the Parameters of the IFT-MSPS Atlas-based Segmentation.
 * @author Samuel Martins
 * @date May 17, 2016
 * @ingroup ProbAtlas
 *
 * Actually, no specific parameter is allocated to this method.
 * 
 * @param seg_alg Atlas-based Segmentation Algorithm.
 * @return The Atlas Segmentation Parameters.
 */
iftAtlasSegParams *iftSetupIFTMSPSAtlasSeg(iftAtlasSegAlg seg_alg, float alpha);


/**
 * @brief Destroys Atlas-based Segmentation Parameters.
 * @author Samuel Martins
 * @date May 9, 2016
 * @ingroup ProbAtlas
 */
void iftDestroyAtlasSegParams(iftAtlasSegParams **seg_params);


/**
 * @brief Segment a Test Image with a Prob. Atlas using a given Atlas-based Segmentation Algorithms.
 * @author Samuel Martins
 * @date May 9, 2016
 * @ingroup ProbAtlas
 * @note An example is @ref iftSegmentByProbAtlas.c
 *
 * Available Atlas-based Segmentation Algorithms: \n
 * * Classical - Thresholding over the Prior Probability Map
 * * A Posteriori - Thresholding over the Posterior Probability Map
 * * IFT-MSPS - Segmentation using IFT-SC on the best (high score) location on Test Image found
 *
 * @note Use the Setup Parameter function for the required Segmentation Algorithm before calling this function. 
 * 
 * @param test_img Test Image to be segmented
 * @param atlas Probabilistic Atlas that will segment the Test Image. 
 * @param seg_alg Chosen Segmentation Algorithm.
 * @return The Segmented Image.
 *
 * @sa iftSetupClassicalAtlasSeg()
 */
iftImage *iftSegmentByProbAtlas(iftImage *grad_img, iftProbAtlas *atlas,
                                const iftAtlasSegParams *seg_params, iftObjProcPolicy seg_pol,
                                iftLabelTieBreakPolicy tie_break_pol);


/////////////////// SIMPLE ATLAS FUNCTIONS //////////////////////////
/**
 * @brief Creates/Allocates a Simple Prob. Atlas.
 * @author Samuel Martins
 * @date Jun 28, 2016
 * @ingroup ProbAtlas
 * @return The created Simple Prob. Atlas.
 */
iftSimpleProbAtlas *iftCreateSimpleProbAtlas();


/**
 * @brief Destroys a Simple Prob. Atlas.
 * @author Samuel Martins
 * @date Jun 28, 2016
 * @ingroup ProbAtlas
 */
void iftDestroySimpleProbAtlas(iftSimpleProbAtlas **satlas);


/**
 * @brief Reads a Simple Prob. Atlas
 * @author Samuel Martins
 * @date Jun 28, 2016
 * @ingroup ProbAtlas
 */
iftSimpleProbAtlas *iftReadSimpleProbAtlas(const char *path);


/**
 * @brief Writes a Simple Prob. Atlas
 * @author Samuel Martins
 * @date Jun 28, 2016
 * @ingroup ProbAtlas
 */
void iftWriteSimpleProbAtlas(const iftSimpleProbAtlas *satlas, const char *path);


/**
 * @brief Builds a Simple Prob. Atlas from a set of Ground Truths.
 * @author Samuel Martins
 * @date Jun 28, 2016
 * @ingroup ProbAtlas
 *  
 * @param  label_set     Set with the pathnames of the Images to be used to build the Prob. Atlas.
 * @param  labels        Array of Labels which will be constructed the simple models.
 * @return               Simple Prob. Atlas.
 */
iftSimpleProbAtlas *iftBuildSimpleProbAtlas(const iftFileSet *label_set, const iftIntArray *labels);






/**
 * @brief Filters the Seeds by binary object mask.
 * @author Samuel Martins
 * @date Jun 4, 2016
 *
 * The labeled seeds can be from several different objects and the background. \n
 * The Binary Object Mask is a combination of all objects, and it could be generate by a classifier. \n\n
 * 
 * For example, we have seeds for Bg, Cerebellum, Left and Right Hemispheres. \n
 * Our bin object mask, that could be generate by a texture classifier, shows if the voxels were
 * classified as an object (any one of them) or background. \n\n
 * 
 * The rule for filtering is: \n
 * (1) If it is a seed of background, keep it; \n
 * (2) If it is a seed of any object and the object map confirms that it is from an object, keep it.
 * 
 * @param  seeds    Labeled seeds to be filtered.
 * @param  bin_mask Binary Object Mask, generated by another mechanism, such as texture classification,
 *                  that shows if the seeds are background or object.
 * @return          The filtered seeds for delineation.
 */
iftLabeledSet *iftFilterSeedsByBinObjectMask(const iftLabeledSet *seeds, const iftImage *bin_mask);


/**
 * @brief Finds the best Filtered Multi-Object Model Seeds by MSPS to delineate a Test Image using IFT-Watershed delineation.
 * @author Samuel Martins
 * @date Jun 4, 2016
 *
 * The filtering is done by using a bin object mask like the function @ref iftFilterSeedsByBinObjectMask(). \n
 * MSPS slightly shifts the seeds around their original position. \n
 * Candidate objects are delineated by the IFT-watershed and the mean gradient value along their
 * boundary is taken as score for maximization.
 * 
 * @param  grad_img                Gradient of the Test Image to be delineated.
 * @param  model                   Prior Probability Map (Model).
 * @param  model_label_map         Label map from the model.
 * @param  bin_mask                Binary Object Mask, generated by another mechanism, such as texture classification,
 *                                 that shows if the seeds are background or object.
 * @param  e_radius                Erosion Radius that used to find the Inner Seeds (Certain Region Border of the Target Object).
 * @param  d_radius                Radius Dilation that is used to find the Outer Seeds (Certain Region Border of the Background).
 * @param  out_bg_certainty_region Returns by Reference (if != NULL) all voxels of the certainty regions (probability of 1)
 *                                 of the background considering the shift of the MSPS.
 * @return                         The best Filtered Model Seeds.
 */
iftLabeledSet *iftFindBestFilteredModelSeedsByMSPS(const iftImage *grad_img, const iftFImage *model, const iftImage *model_label_map,
                                                   const iftImage *model_grad, float alpha, const iftImage *bin_mask,
                                                   double e_radius, double d_radius, iftSet **out_bg_certainty_region);


#ifdef __cplusplus
}
#endif

#endif // IFT_ATLAS_H