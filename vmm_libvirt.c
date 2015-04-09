#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vmm_libvirt.h"

vmmLibvirt::vmmLibvirt(char *uri) {
    conn = virConnectOpen(uri);
    if (conn == NULL) {
        printf("Failed to open connection to qemu://system\n");
        exit;
    }
    numDomains = virConnectNumOfDomains(conn);
    domId = (int *)malloc(sizeof(int) * numDomains);
    numDomains = virConnectListDomains(conn, domId, numDomains);
}

unsigned long long vmmLibvirt::getDomainCpuTime(int index) {
    int nparams;
    virTypedParameterPtr params = NULL;
    unsigned long long cpuTime = 0;
    int i;
    virDomainPtr dom;

    dom = virDomainLookupByID(conn, domId[index]);
    nparams = virDomainGetCPUStats(dom, NULL, 0, -1, 1, 0);
    if (nparams != 0) {
        if ((params = (virTypedParameterPtr)malloc(sizeof(virTypedParameter) * nparams))) {
            memset(params, 0, sizeof(virTypedParameter) * nparams);
            if (virDomainGetCPUStats(dom, params, nparams, -1, 1, 0) != -1) {
                for(i = 0; i < nparams; i++) {
                    if (strcmp(params[i].field, "cpu_time") == 0) {
                        cpuTime = params[i].value.ul;
                    }
                }
            }
        }
    }

    if (params)
        free(params);

    return cpuTime;
}

unsigned long long vmmLibvirt::getNodeCpuTime() {
    unsigned long long nodeCpuTime = 0;
    int nparams = 0;
    virNodeCPUStatsPtr params;

    if (virNodeGetCPUStats(conn, VIR_NODE_CPU_STATS_ALL_CPUS, NULL, &nparams, 0) == 0 && nparams != 0) {
        if (params = (virNodeCPUStatsPtr)malloc(sizeof(virNodeCPUStats) * nparams)) {
            memset(params, 0, sizeof(virNodeCPUStats) * nparams);
            if (!virNodeGetCPUStats(conn, VIR_NODE_CPU_STATS_ALL_CPUS, params, &nparams, 0)) {
                for (int i=0; i<nparams; i++) {
                    nodeCpuTime += params[i].value;
                }
            }
        }
    }
    return nodeCpuTime;
}

virDomainPtr vmmLibvirt::domainLookupByID(int index) {
    return(virDomainLookupByID(conn, domId[index]));
}

int vmmLibvirt::getDomainID(int **domId) {
    int numDomains;

    numDomains = virConnectNumOfDomains(conn);
    *domId = (int *)malloc(sizeof(int) * numDomains);
    numDomains = virConnectListDomains(conn, *domId, numDomains);
    return numDomains;
}
