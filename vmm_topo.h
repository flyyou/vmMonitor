#ifndef __VMM_NODES_H__
#define __VMM_NODES_H__
#include <stdio.h>

extern const int GET_NODES_BATCH_SIZE;

class vmmTopo {
    FILE *fp;
    char nodeFile[256];
    int getNodes(char *node[]);
public:
    void init(char *nodefile);
    int getFirstNodes(char *nodes[]);
    int getNextNodes(char *nodes[]);
    void freeNodes(char *node[], int num);
};

#endif
