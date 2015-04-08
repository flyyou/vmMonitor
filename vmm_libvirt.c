#include <stdlib.h>
#include <string.h>

#include "vmm_libvirt.h"

int vmmGetDomains(virConnectPtr conn,  int **domID) {
    int numDomains;

    numDomains = virConnectNumOfDomains(conn);
    *domID = (int *)malloc(sizeof(int) * numDomains);
    numDomains = virConnectListDomains(conn, *domID, numDomains);
    return numDomains;
}

unsigned long long vmmGetCPUTime(virDomainPtr dom) {
    int nparams;
    virTypedParameterPtr params = NULL;
    unsigned long long cpuTime = 0;
    int i;

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
