#include "FL.h"
#include "bagOfVisualWords.h"

int main(int argc, char **argv) {

    DirectoryManager* directoryManager;

    //Load dictionary images
    directoryManager = loadDirectory("../processedData/dictionary-creation", 1);
    printf("reading:\n");
    for (int i = 0; i < (int)directoryManager->nfiles; ++i) {
        printf("%s\n",directoryManager->files[i]->path);
    }

    destroyDirectoryManager(directoryManager);

    //Load training images
    directoryManager = loadDirectory("../processedData/training", 1);
    printf("reading:\n");
    for (int i = 0; i < (int)directoryManager->nfiles; ++i) {
        printf("%s\n",directoryManager->files[i]->path);
    }

    destroyDirectoryManager(directoryManager);

    //Load test images
    directoryManager = loadDirectory("../processedData/test", 1);
    printf("reading:\n");
    for (int i = 0; i < (int)directoryManager->nfiles; ++i) {
        printf("%s\n",directoryManager->files[i]->path);
    }

    destroyDirectoryManager(directoryManager);

    return 0;
}
