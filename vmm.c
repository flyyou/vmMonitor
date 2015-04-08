#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vmm.h"
#include "vmm_libvirt.h"
#include "vmm_topo.h"
#include "vmm_stats.h"

vmMonitor::vmMonitor() {
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

int vmMonitor::init(int argc, char *argv[]) {
    extern char *optarg;
    extern int optind, optopt;
    int optchar;

    while( ( optchar = getopt( argc, argv, "c:d:m:n:p:t:x:lh" ) ) != -1 ) {
        switch( optchar ) {
            case 't':
                runTimeOpt = 1;
                runTime = atoi(optarg) * 60;
                break;
            case 'x':
                numThreadsOpt = 1;
                numThreads = atoi(optarg);
                break;
            case 'c':
                cpuThresholdOpt = 1;
                cpuThreshold = atoi(optarg);
                break;
            case 'm':
                memThresholdOpt = 1;
                memThreshold = atoi(optarg);
                break;
            case 'd':
                diskThresholdOpt = 1;
                diskThreshold = atoi(optarg);
                break;
            case 'i':
                monitorIntervalOpt = 1;
                monitorInterval = atoi(optarg);
                break;
            case 'n':
                numIterationsOpt = 1;
                numIterations = atoi(optarg);
                break;
            case 'h':
            default:
                goto usage;
        }
    }

    if (optind < argc) {
        nodeFileOpt = 1;
        strcpy(nodeFile, argv[optind]);
    }

    if (!numIterationsOpt) {
        if (runTimeOpt) {
            numIterations = runTime / monitorInterval;
        } else {
            numIterationsOpt = 1;
        }
    }

    topo.init(nodeFile);
        
    return 1;

usage:
    printf("Usage: vmmon [options] [FILENAME]\n"
           "       FILENAME\n"
           "\t\tFile containing IP addresses or hostnames (one per line) of the nodes to be monitored.\n"
           "       OPTIONS\n"
          "\t\t-t Run time in minutes. Specify either -t or -n.\n"
          "\t\t-x Number of processing threads (default=1)\t\t \n"
          "\t\t-c CPU threshold percentage (default=20)\n"
          "\t\t-m Memory threshold percentage (default=20)\n"
          "\t\t-d Disk threshold percentage (default=20)\n"
          "\t\t-i Monitor interval in secs (default=10)\n"
          "\t\t-n Number of iterations (default=1)\n");
    exit(0);
}

void vmMonitor::collect() {
    int i, j, k;
    class vmmStats stats;

    for (i=0; i<numIterations; i++) {
        char *nodes[GET_NODES_BATCH_SIZE];
        int numNodes;
        FILE *fp1 = 0;

        if (numNodes = topo.getFirstNodes(nodes)) {
            do {
                for (j=0; j<numNodes; j++) { 
                    class vmmStatsRec *statsRec;
                    int numStats;

                    numStats = stats.collect(nodes[j], &statsRec);
                    stats.write(statsRec, numStats);

                    free(statsRec);
                }
                topo.freeNodes(nodes, numNodes);
            } while (numNodes = topo.getNextNodes(nodes));
        }
        if (i < numIterations-1)
            sleep(monitorInterval);
    }
    return;
}

void vmMonitor::process() {
    FILE *fp;
    class vmmStatsRec start, curr, prev;
    unsigned long long cpuUtil;

    system("sort stats > stats.tmp");

    fp = fopen("stats.tmp", "r");

    if (fp) {
        int ret;

        ret = fscanf(fp, "%s %d %llu %llu", start.node, &start.vmID, &start.nodeCpuTime, &start.vmCpuTime);
        if(ret > 0) {
            start.copy(&prev);
            while (fscanf(fp, "%s %d %llu %llu", curr.node, &curr.vmID, &curr.nodeCpuTime, &curr.vmCpuTime) > 0) {
                if ((strcmp(prev.node, curr.node)) || (curr.vmID != prev.vmID)) {
                    cpuUtil = (100 * (prev.vmCpuTime - start.vmCpuTime)) / (prev.nodeCpuTime - start.nodeCpuTime);
                    printf("\nserver=%s vm-id=%d cpu-util=%llu%%\n", start.node, start.vmID, cpuUtil);
                    curr.copy(&start);
                }
                curr.copy(&prev);
            }
            cpuUtil = (100 * (prev.vmCpuTime - start.vmCpuTime)) / (prev.nodeCpuTime - start.nodeCpuTime);
            printf("\nserver=%s vm-id=%d cpu-util=%llu%%\n", start.node, start.vmID, cpuUtil);
        }
    }
}

void vmMonitor::enforce() {
}
