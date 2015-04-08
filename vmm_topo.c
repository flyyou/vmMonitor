#include <string.h>
#include <stdlib.h>

#include "vmm_topo.h"

const int GET_NODES_BATCH_SIZE = 10;

void vmmTopo::init(char *file) {
    strcpy(nodeFile, file);
    fp = fopen(nodeFile, "r");
}

int vmmTopo::getFirstNodes(char *nodes[]) {
    rewind(fp);
    return(getNodes(nodes));
}

int vmmTopo::getNextNodes(char *nodes[]) {
    return(getNodes(nodes));
}

int vmmTopo::getNodes(char *nodes[]) {
    char strBuf[256];
    int num = 0;

    while ((fgets(strBuf, 256, fp)) && (num < GET_NODES_BATCH_SIZE)) {
        char *pos;

        if ((pos=strchr(strBuf, '\n')) != NULL)
            *pos = '\0';

        nodes[num] = (char *)malloc(sizeof(char)*strlen(strBuf)+1);
        strncpy(nodes[num++], strBuf, strlen(strBuf)+1);
    }

    return num; 
}

void vmmTopo::freeNodes(char *node[], int num) {
    int i;

    for (i=0; i<num; i++) {
        free(node[i]);
    }
}
