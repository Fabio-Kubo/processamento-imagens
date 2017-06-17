/**
 * @file
 * @brief Parameter Optimization utilities
 * @author Peixinho
 */

#ifndef IFT_PARAMOPTIMIZER_H
#define IFT_PARAMOPTIMIZER_H

#include "ift.h"

typedef struct ift_param_optimizer {
    int n;
    iftDblArray ** paramsSpace;
    double *params;
    double score;
} iftParamOptimizer;

typedef double (*iftClassifierFitnessFunc)   (void*, iftDataSet*, double *params);
typedef double (*iftDescriptorFitnessFunc)   (void*, iftFileSet*, double *params);

/**
 * @brief Creates a iftParamOptimizer for a problem with n parameters.
 * @author Peixinho
 */
iftParamOptimizer* iftCreateParamOptimizer(int nparams);

/**
 * @brief Destroys the iftParamOptimizer object.
 * @author Peixinho
 */
void iftDestroyParamOptimizer(iftParamOptimizer** opt);

//TODO: change all the void* problem in a params dictionary

/*
 * @brief Searchs for the best score parameters among all the possible values.
 * @param opt Space search params
 * @param f Fitness function, evaluates the classifier given the specified parameters.
 * @param problem Extra parameters to an specific problem
 * @author Peixinho
 * */
void iftGridSearch(iftParamOptimizer* opt, iftDFitnessFunc f, void* problem);

/*
 * @brief Searchs for the best classifier parameters in a given dataset using the iftGridSearch() method.
 * @param opt Space search params
 * @param f Fitness function, evaluates the classifier given the specified parameters.
 * @param dataset The train and test dataset, with thre proper TRAIN/TEST split.
 * @param problem Extra parameters to an specific problem
 * @author Peixinho
 * */
void iftGridSearchClassifier(iftParamOptimizer* opt, iftClassifierFitnessFunc f, iftDataSet* dataset, iftSampler* sampler, void* problem);

/*
 * @brief Searchs for the best descriptor parameters in a given fileset using the iftGridSearch() method.
 * @param opt Space search params
 * @param f Fitness function, evaluates the descriptor given the specified parameters.
 * @param dataset The fileset to be used in descriptor evaluation.
 * @param problem Extra parameters to an specific problem
 * @author Peixinho
 * */
void iftGridSearchDescriptor(iftParamOptimizer*, iftDescriptorFitnessFunc f, iftFileSet* dataset, void* problem);

/*
 * @brief Searchs for the best score parameters using random parameters trials.
 * @param opt Space search params
 * @param f Fitness function, evaluates the classifier given the specified parameters.
 * @ntrials The number of random trials.
 * @param problem Extra parameters to an specific problem
 * @author Peixinho
 * */
void iftRandomSearch(iftParamOptimizer* opt, iftDFitnessFunc f, int ntrials, void*);

void iftRandomSearchClassifier(iftParamOptimizer*, iftClassifierFitnessFunc, iftDataSet* dataset, iftSampler* sampler, int ntrials, void*);
void iftRandomSearchDescriptor(iftParamOptimizer*, iftDescriptorFitnessFunc fitnessFunc, iftFileSet*, int ntrials, void*);

/**
 * @brief Read iftParamOptimizer from disk.
 * The file must contain a mandatory key named "params", describing all the parameters (in order) to be optimized. Followed of the search space for each parameter.
 * @author Peixinho
 * @date April, 2016
 */
iftParamOptimizer* iftReadParamOptimizer(const char *filename);

/**
 * @brief Read iftParamOptimizer from dict.
 * The dict must contain a mandatory key named "params", describing all the parameters (in order) to be optimized. Followed of the search space for each parameter.
 * @author Peixinho
 * @date April, 2016
 */
iftParamOptimizer *iftCreateParamOptimizerFromDict(const iftDict* dict);

#endif //IFT_PARAMOPTIMIZER_H
