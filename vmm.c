#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vmm.h"
#include "vmm_libvirt.h"
#include "vmm_topo.h"
#include "vmm_stats.h"

vmMonitor::vmMonitor() {
}

int vmMonitor::init(int argc, char *argv[]) {
    extern char *optarg;
    extern int optind, optopt;
    int optchar;

    while( ( optchar = getopt( argc, argv, "c:d:m:n:p:t:x:lh" ) ) != -1 ) {
        switch( optchar ) {
            case 't':
                config.runTimeOpt = 1;
                config.runTime = atoi(optarg) * 60;
                break;
            case 'x':
                config.numThreadsOpt = 1;
                config.numThreads = atoi(optarg);
                break;
            case 'c':
                config.cpuThresholdOpt = 1;
                config.cpuThreshold = atoi(optarg);
                break;
            case 'm':
                config.memThresholdOpt = 1;
                config.memThreshold = atoi(optarg);
                break;
            case 'd':
                config.diskThresholdOpt = 1;
                config.diskThreshold = atoi(optarg);
                break;
            case 'i':
                config.monitorIntervalOpt = 1;
                config.monitorInterval = atoi(optarg);
                break;
            case 'n':
                config.numIterationsOpt = 1;
                config.numIterations = atoi(optarg);
                break;
            case 'h':
            default:
                goto usage;
        }
    }

    if (optind >= argc)
        goto usage;

        config.nodeFileOpt = 1;
        strcpy(config.nodeFile, argv[optind]);

    if (!config.numIterationsOpt) {
        if (config.runTimeOpt) {
            config.numIterations = config.runTime / config.monitorInterval;
        } else {
            config.numIterationsOpt = 1;
        }
    }

    topo.init(config.nodeFile);
        
    return 1;

usage:
    printf("Usage: vmmon [options] FILENAME\n"
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

    for (i=0; i<config.numIterations; i++) {
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
        if (i < config.numIterations-1)
            sleep(config.monitorInterval);
    }
    return;
}

void vmMonitor::process() {
    FILE *fp;
    class vmmStatsRec start, curr, prev;
    unsigned long long cpuUtil;

    system("sort .stats > .stats.sorted");

    fp = fopen(".stats.sorted", "r");

    if (fp) {
        int ret;

        ret = fscanf(fp, "%s %d %llu %llu", start.node, &start.vmID, &start.nodeCpuTime, &start.vmCpuTime);
        if(ret > 0) {
            start.copy(&prev);
            while (fscanf(fp, "%s %d %llu %llu", curr.node, &curr.vmID, &curr.nodeCpuTime, &curr.vmCpuTime) > 0) {
                if ((strcmp(prev.node, curr.node)) || (curr.vmID != prev.vmID)) {
                    cpuUtil = (100 * (prev.vmCpuTime - start.vmCpuTime)) / (prev.nodeCpuTime - start.nodeCpuTime);
                    if (cpuUtil < config.cpuThreshold)
                        printf("server: %-30s vmid: %-4d cpu_util: %llu%%\n", start.node, start.vmID, cpuUtil);
                    curr.copy(&start);
                }
                curr.copy(&prev);
            }
        }
    }
}

void vmMonitor::enforce() {
}
