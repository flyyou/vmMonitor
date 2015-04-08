#ifndef __VMM_STORE_H__
#define __VMM_STORE_H__
#include <stdio.h>

class vmmStats {
    FILE *fp;
public:
    vmmStats();
    vmmStats(char *file);
    int collect(const char *node, struct vmmStatsRec **stats);
    void write(class vmmStatsRec *rec, int num);
    ~vmmStats();
};

class vmmStatsRec {
public:
    int timestamp;
    char node[256];
    int vmID;
    unsigned long long nodeCpuTime;
    unsigned long long vmCpuTime;
    void copy(class vmmStatsRec *d);
};

#endif
