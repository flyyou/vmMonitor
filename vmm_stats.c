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
    virDomainPtr dom;
    char str[MAX_URI_LEN];
    vmmStatsRec *s;
    int numStats = 0;
    unsigned long long nodeCpuTime;
    int index;

    //no_verify=1 disables client side verification of node's cert
    snprintf(str, MAX_URI_LEN, "qemu+tls://%s/system?no_verify=1", node);

    vmmLibvirt libvirt = vmmLibvirt(str);
    
    nodeCpuTime = libvirt.getNodeCpuTime();
    
    *stats = (vmmStatsRec *)malloc(sizeof(vmmStatsRec) * libvirt.numDomains);
    for (index = 0, s = *stats; index < libvirt.numDomains; index++, s++) {
        strcpy(s->node, node);
        s->vmID = libvirt.domId[index];
        s->nodeCpuTime = nodeCpuTime;
        s->vmCpuTime = libvirt.getDomainCpuTime(index);
        // Get mem and disk stats
        numStats++;
    }

error:

    return numStats;
}

void vmmStatsRec::copy(class vmmStatsRec *rec) {
    strcpy(rec->node, node);
    rec->vmID = vmID;
    rec->nodeCpuTime = nodeCpuTime;
    rec->vmCpuTime = vmCpuTime;
}
