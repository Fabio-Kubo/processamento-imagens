#include "FL.h"
#include "flToIft.cpp"

/**
 * SAMPLERS:
 * 0 -> None
 * 1 -> Grid
 * 2 -> SuperPixel
 * 3 -> Random sampling
 *
 * FEATURE EXTRACTORS:
 * 0 -> Color Histogram
 * 1 -> HOG
 *
 * CLUSTERING:
 * 0 -> k-means
 *
 * CLASSIFIER:
 * 0 -> SVM
 * 1 -> k-means
 */
#define SAMPLER 1
#define FEATURE_EXTRACTOR 1
#define CLUSTERING 0
#define CLASSIFIER 0

// Super pixel sampling.
#define N_SUPER_PIXELS 30
#define SP_ALPHA 0.10f
#define SP_BETA 20
#define N_SP_ITERATIONS 20
#define N_SP_SMOOTH_ITERATIONS 0
#define SP_PATCH_SIZE 32
#define SP_MAX_SAMPLES 64

// Random sampling.
#define N_RANDOM_PATCHES 4
#define RANDOM_SEED 0

// Grid sampling.
#define GRID_PATCH_SIZE 64

// k-means Clustering.
#define N_VISUAL_WORDS 1000
#define N_K_MEANS_ITERATIONS 100
#define K_MEANS_TOLERANCE 0.001

// Color histogram.
#define CH_N_BINS 7

// HOG.
#define N_HOG_CELLS_PER_BLOCK 2
#define N_HOG_CELL_SIZE 32
#define HOG_BIN_SIZE 20

int main(int argc, char **argv) {
    //Caminhos onde esta o arquivo txt gerado pelo o script python "selec_samples2.py"
    //os caminhos vao mudar para cada pessoa
    char const* const fileName_createDict = "../train_paths.txt";
    char const* const fileName_createTrain = "../train_paths.txt";
    char const* const fileName_createTest = "../test_paths.txt";

    //cada posicao do vetor tem uma string para o caminho de uma imagem
    GVector* vectorSamplesUsed2CreateDict =  splitsLinesInTextFile(fileName_createDict);
    GVector* vectorSamplesUsed2TrainClassifier =  splitsLinesInTextFile(fileName_createTrain);
    GVector* vectorSamplesUsed2TestClassifier =  splitsLinesInTextFile(fileName_createTest);

    //apenas checkando se o vetor vazio. Caso o vetor esteja vazio, talvez seu caminho ate o arquivo
    //txt nao esteja correto
    if(vectorSamplesUsed2CreateDict == NULL){
        printf("error\n");
        return -1;
    }
    if(vectorSamplesUsed2CreateDict->size == 0){
        printf("no path found");
        return -1;
    }

    if(vectorSamplesUsed2TrainClassifier->size == 0){
        printf("no path found");
        return -1;
    }

    if(vectorSamplesUsed2TestClassifier->size == 0){
        printf("no path found");
        return -1;
    }

    //pipeline para a construncao do dicionario. Para mais detalhes olhe a imagem que esta
    //em data/bowArquiteturaImplementada.png

    //a estrutura bow manager e encarregada de fazer o processo do bow
    BagOfVisualWordsManager* bowManager = createBagOfVisualWordsManager();

    ////////////////////////////////////////////////////////////////////////
    //Passando os vetores que contem os caminhos das imagens para...
    bowManager->pathsToImages_dictionary = vectorSamplesUsed2CreateDict;//criar o dicionario
    bowManager->pathsToImages_train = vectorSamplesUsed2TrainClassifier;//treinar o classificador
    bowManager->pathsToImages_test = vectorSamplesUsed2TestClassifier;//testar o classificador
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
    // Metodo de sampling que vai ser usado para criar os patchs.
    // Se vc passar NULL aqui o estrutura do bow vai criar um vetor de tamanho 1
    // onde o unico elemento desse vetor vai ser a imagem.
    ArgumentList* samplingArguments = createArgumentList();

    printf("## SELECTED SAMPLER: ");
    switch (SAMPLER) {
        // NO sampling.
        case 0: {
            printf("None. ##\n");
            bowManager->imageSamplerFunction = NULL;
            break;
        }
        // Grid sampling.
        case 1: {
            printf("Grid. ##\n");
            bowManager->imageSamplerFunction = gridSamplingBow;
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, GRID_PATCH_SIZE);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, GRID_PATCH_SIZE);
            bowManager->argumentListOfSampler = samplingArguments;
            break;
        }
        // Super Pixel Sampling.
        case 2: {
            printf("Super Pixel Sampling. ##\n");
            bowManager->imageSamplerFunction = superPixelSamplingBow;
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, N_SUPER_PIXELS);
            ARGLIST_PUSH_BACK_AS(float, samplingArguments, SP_ALPHA);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, SP_BETA);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, N_SP_ITERATIONS);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, N_SP_SMOOTH_ITERATIONS);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, SP_PATCH_SIZE);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, SP_MAX_SAMPLES);
            bowManager->argumentListOfSampler = samplingArguments;
            break;
        }

        case 3: {
            printf("Random Sampling. ##\n");
            bowManager->imageSamplerFunction = randomSamplingBow;
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, N_RANDOM_PATCHES);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, GRID_PATCH_SIZE);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, GRID_PATCH_SIZE);
            ARGLIST_PUSH_BACK_AS(size_t, samplingArguments, RANDOM_SEED);
            bowManager->argumentListOfSampler = samplingArguments;
            break;
        }

        default:
            printf("invalid sampler function.\n");
            return -1;
    }

    /////////////////////////////////////////////////////////////////////////////
    //Essa função serve como um garbage collector para o metodo do sampling. Ao final de
    //cada iteracao, ela limpa da memoria os patchs gerados.
    //Se por acaso seu metodo de sampling nao gerar um vetor de imagens (GVector* de Image*),
    //voce pode passar NULL, porém fique consciente que vai ter um pouco de memory leak.
    //Ao final do programa seu sistema operional vai limpar toda a sujeira.
    bowManager->freeFunction2SamplerOutput = destroyImageVoidPointer;
    //bowManager->freeFunction2SamplerOutput = NULL;
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    printf("## SELECTED FEATURE EXTRACTOR: ");
    switch (FEATURE_EXTRACTOR) {
        // Color Histogram.
        case 0: {
            printf("Color Histogram. ##\n");
            bowManager->featureExtractorFunction = computeColorHistogramBow;
            ArgumentList* colorFeatureExtractorArguments = createArgumentList();
            ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,CH_N_BINS);
            ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,CH_N_BINS*CH_N_BINS*CH_N_BINS);
            bowManager->argumentListOfFeatureExtractor = colorFeatureExtractorArguments;
            break;
        }

        // HOG.
        case 1: {
            printf("HOG. ##\n");
            bowManager->featureExtractorFunction = computeHogDescriptorCustom;
            ArgumentList* hogFeatureExtractorArguments = createArgumentList();
            ARGLIST_PUSH_BACK_AS(size_t,hogFeatureExtractorArguments,N_HOG_CELLS_PER_BLOCK);
            ARGLIST_PUSH_BACK_AS(size_t,hogFeatureExtractorArguments,N_HOG_CELL_SIZE);
            ARGLIST_PUSH_BACK_AS(size_t,hogFeatureExtractorArguments,HOG_BIN_SIZE);
            bowManager->argumentListOfFeatureExtractor = hogFeatureExtractorArguments;
            break;
        }

        default: {
            printf("invalid feature extractor function.\n");
            return -1;
        }
    }



    //o meu metodo para fazer o histograma de cores recebe 2 parametros (exlcuindo vetor de entrada)
    //0 - vetor com as imagens dos patchs (esse argumento n'ao conta)
    //1 - numeros de bins por canal
    //2 - numero total de bins (bins por canal * numero de canais). Portanto, eu vou
    //criar uma argumentList e colocar dois parametros nela.
    //Note que o cabecalho geral para a funcao do extrator e
    //Matrix* MinhaFuncaoFeatureExtractor(GVector* outputSampler, BagOfVisualWordsManager* bagOfVisualWordsManager);
     //passando a lista de argumentos do feature extractor para o bow manager
    ///////////////////////////////////////

    ///////////////////////////////////////////////////////
    //Existem muitas maneiras de computar distancias entre pontos e vetores. A mais comum delas talvez
    //seja a distancia Euclidianda (norma l2). Neste exemplo eu vou usar a norma l1.
    //Quando vc implementar seu metodo de sampling, feature extraxtion, ou clustering, voce
    //pode usar essa funcao distancia.
    bowManager->distanceFunction = computeNormalizedL1Norm;
    bowManager->argumentListOfDistanceFunction = NULL;
    ////////////////////////////////////////////////////


    /////////////////////////////////////////////////////
    //Aqui precisamos definir qual funcao de clsutering vamos usar para encontrar as palavras
    //do dicionario. Eu optei de usar o kmeans clustering. Meu metodo do kmeans recebe 6 parametros,
    //desta forma eu preciso criar uma ArgumentList com 6 parametros.
    //Note que o cabecalho geral para a funcao de clustering e
    //typedef Matrix* minhaFuncaoDeClustering(Matrix* outputFeatureExtractor_allSamples, BagOfVisualWordsManager* bagOfVisualWordsManager);
    printf("## SELECTED CLUSTERING: ");
    switch (CLUSTERING) {
        // k-means
        case 0: {
            printf("K-means. ##\n");
            bowManager->clusteringFunction = kmeansClusteringBow;
            ArgumentList* clusteringMethodArguments = createArgumentList();
            ARGLIST_PUSH_BACK_AS(size_t, clusteringMethodArguments, N_VISUAL_WORDS); //number of words
            ARGLIST_PUSH_BACK_AS(size_t, clusteringMethodArguments, N_K_MEANS_ITERATIONS); //maximum number of iterations
            ARGLIST_PUSH_BACK_AS(double, clusteringMethodArguments, K_MEANS_TOLERANCE); //tolerance
            ARGLIST_PUSH_BACK_AS(int,clusteringMethodArguments,0); //seed
            ARGLIST_PUSH_BACK_AS(DistanceFunction,clusteringMethodArguments,computeNormalizedL1Norm); //seed
            ARGLIST_PUSH_BACK_AS(ArgumentList*,clusteringMethodArguments,NULL); //seed
            bowManager->argumentListOfClustering = clusteringMethodArguments;
            break;
        }

        default:
            printf("invalid clustering function\n");
    }

    ////////////
    //computa o dicionario
    computeDictionary(bowManager);
    /////////////

    //////////////////////
    //define a funcao para montar o histograma
    bowManager->mountHistogramFunction = computeCountHistogram_bow;
    bowManager->argumentListOfHistogramMounter = NULL;
    ///////////////////////


    /////////////////////////////////////////////////
    //criar um classificador e define os parametros
    //do classficiador. Em seguida, o bow manager recebe
    //o ponteiro do classificador. Desta forma o classificador
    //podera ser usado internamente dentro do bow manager.

    //knn
//    Knn_Classifier* classifierknn = createKnnClassifier();
//    classifierknn->k = 1;
//    classifierknn->nlabels = 100;
//    bowManager->classifier = (void*)classifierknn;
//    bowManager->fitFunction = knn_Classifier_fit;
//    bowManager->storeTrainData = false;
//    bowManager->predictFunction = knn_Classifier_predict;
//    bowManager->storePredictedData = false;
//    bowManager->freeFunctionClassifier = destroyKnnClassifierForVoidPointer;


    //"kmeans classifier"
    printf("## SELECTED CLASSIFIER: ");
    switch (CLASSIFIER) {
        // SVM.
        case 0: {
            printf("SVM. ##\n");
            SVM_Classifier* classifiersvm = createSVMClassifier();
            classifiersvm->param.kernel_type = RBF;
            classifiersvm->param.gamma = 3.5;
            bowManager->classifier = (void*)classifiersvm;
            bowManager->fitFunction = svm_Classifier_fit;
            bowManager->storeTrainData = false;
            bowManager->predictFunction = svm_Classifier_predict;
            bowManager->storePredictedData = false;
            bowManager->freeFunctionClassifier = destroySVMClassifierForVoidPointer;
            break;
        }

        // k-means.
        case 1: {
            printf("k-means. ##\n");
            Kmeans_Classifier* classifierkmeans = createKmeansClassifier();
            classifierkmeans->nlabels = 100;
            bowManager->classifier = (void*)classifierkmeans;
            bowManager->fitFunction = kmeans_Classifier_fit;
            bowManager->storeTrainData = false;
            bowManager->predictFunction = kmeans_Classifier_predict;
            bowManager->storePredictedData = false;
            bowManager->freeFunctionClassifier = destroyKmeansClassifierForVoidPointer;
            break;
        }

        default:
            printf("invalid classifier function.\n");
            return -1;
    }

    //////////////////////////////////////

    ///////
    //monta os histogramas, le os label e em seguida treina o classificador
    trainClassifier(bowManager);
    //////////

    /////////////////////////////////////////////////////
    //monta os histogramas e usa o classificador treinado para
    //classificar as amostras do conjunto de teste
    GVector* labelsPredicted = predictLabels(bowManager);
    //////////////////////////

    //////////////////////////
    //Le os true labels das imagens e checa com os labels predizidos pelo o classificador.
    //computa uma simples acuracia (numero de amostras rotuladas corretamente / numero de amostras do conjunto)
    GVector* trueLabels = createNullVector(bowManager->pathsToImages_test->size,sizeof(int));
    int hit = 0;
    //printf("file | predicted true\t\tcorrect\n");
    //char symbol;
    for (size_t index = 0; index < bowManager->pathsToImages_test->size; ++index) {
        //symbol = 'X';
        char * path = VECTOR_GET_ELEMENT_AS(char*,bowManager->pathsToImages_test,index);
        VECTOR_GET_ELEMENT_AS(int,trueLabels,index) = findTrueLabelInName(path);
        if(VECTOR_GET_ELEMENT_AS(int,trueLabels,index) == VECTOR_GET_ELEMENT_AS(int,labelsPredicted,index)){
            hit++;
            //symbol = 'O';
        }
        /*printf("%s | %d %d\t\t%c\n",
               path,
               VECTOR_GET_ELEMENT_AS(int,labelsPredicted,index),
               VECTOR_GET_ELEMENT_AS(int,trueLabels,index),symbol
        );*/
    }
    double acuracia = ((double)hit) / bowManager->pathsToImages_test->size;
    printf("acuracia: %f\n",acuracia);
    /////////////////////////////////////

    destroyBagOfVisualWordsManager(&bowManager);
    destroyVector(&trueLabels);
    destroyVector(&labelsPredicted);
    return 0;
}


