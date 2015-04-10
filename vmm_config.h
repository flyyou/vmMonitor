#ifndef __VMM_CONFIG_H__
#define __VMM_CONFIG_H__

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
#endif
