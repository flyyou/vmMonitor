#ifndef __VMM_H__
#define __VMM_H__

#include "vmm_topo.h"
#include "vmm_stats.h"

class vmmConfig {
public:
    int runTimeOpt;
    int runTime;
    int numThreadsOpt;
    int numThreads;
    int cpuThresholdOpt;
    int cpuThreshold;
    int memThresholdOpt;
    int memThreshold;
    int diskThresholdOpt;
    int diskThreshold;
    int monitorIntervalOpt;
    int monitorInterval;
    int numIterationsOpt;
    int numIterations;
    int nodeFileOpt;
    char nodeFile[256];
    vmmConfig();
};

class vmMonitor {
    class vmmConfig config;
    class vmmTopo topo;
    void checkCpuUtil(vmmStatsRec *rec, unsigned long long cpuUtil);
public:
    vmMonitor();
    int init(int argc, char *argv[]);
    void collect();
    void process();
    void enforce();
};

#endif
