#include <stdlib.h>
#include <string.h>

#include "vmm_stats.h"
#include "vmm_libvirt.h"

vmmStats::vmmStats() {
    fp = fopen("stats", "w");
}

vmmStats::vmmStats(char *file) {
    fp = fopen(file, "w");
}

void vmmStats::write(struct vmmStatsRec *rec, int num) {
    int i;

    for (i = 0; i < num; i++) {
        fprintf(fp, "%s %d %llu %llu\n", rec[i].node, rec[i].vmID,
                                    rec[i].nodeCpuTime, rec[i].vmCpuTime);
    }

}

vmmStats::~vmmStats() {
    fclose(fp);
}

#define MAX_URI_LEN 256

int vmmStats::collect(const char *node, struct vmmStatsRec **stats) {        
    virConnectPtr conn;
    int numDomains;
    int *domID;
    virDomainPtr dom;
    char str[MAX_URI_LEN];
    int i;
    struct vmmStatsRec *s;
    virNodeCPUStatsPtr params;
    int nparams = 0;
    int numStats = 0;
    unsigned long long nodeCpuTime = 0;

    //no_verify=1 disables client side verification of node's cert
    snprintf(str, MAX_URI_LEN, "qemu+tls://%s/system?no_verify=1", node);
    conn = virConnectOpen(str);
    if (conn == NULL) {
        printf("Failed to open connection to qemu://system\n");
        return 1;
    }
    
    //multi-thread?
    //virConnectRef(conn); 

    numDomains = vmmGetDomains(conn, &domID);
    if (!numDomains)
        return 0;

    if (virNodeGetCPUStats(conn, VIR_NODE_CPU_STATS_ALL_CPUS, NULL, &nparams, 0) == 0 && nparams != 0) {
        if (params = (virNodeCPUStatsPtr)malloc(sizeof(virNodeCPUStats) * nparams)) {
            memset(params, 0, sizeof(virNodeCPUStats) * nparams);
            if (!virNodeGetCPUStats(conn, VIR_NODE_CPU_STATS_ALL_CPUS, params, &nparams, 0)) {
                for (i=0; i<nparams; i++) {
                    nodeCpuTime += params[i].value;
                }
            }
        }
    }
    
    *stats = (struct vmmStatsRec *)malloc(sizeof(struct vmmStatsRec) * numDomains);

    for (i = 0, s = *stats; i < numDomains; i++, s++) {
        dom = virDomainLookupByID(conn, domID[i]);
        if (dom == NULL) {
            printf("Failed to lookup domain %d\n", domID[i]);
            continue;
        }
        strcpy(s->node, node);
        s->vmID = domID[i];
        s->nodeCpuTime = nodeCpuTime;
        s->vmCpuTime = vmmGetCPUTime(dom);
        //s->vmCpuUtil = (s->vmCpuTime / s->nodeCpuTime) * 100;
        numStats++;
    }

error:
    free(domID);
    virConnectClose(conn);

    return numStats;
}

void vmmStatsRec::copy(class vmmStatsRec *rec) {
    strcpy(rec->node, node);
    rec->vmID = vmID;
    rec->nodeCpuTime = nodeCpuTime;
    rec->vmCpuTime = vmCpuTime;
}
