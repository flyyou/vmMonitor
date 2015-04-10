#include "vmm_config.h"

vmmConfig::vmmConfig() {
    // Default config
    runTimeOpt = 0;
    numIterationsOpt = 0;
    numThreadsOpt = 0;
    monitorIntervalOpt = 0;
    cpuThresholdOpt = 1;
    diskThresholdOpt = 1;

    monitorInterval = 10;
    numIterations = 2;
    cpuThreshold = 20;
    diskThreshold = 20;
}

