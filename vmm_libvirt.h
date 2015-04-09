#ifndef _VMM_LIBVIRT_H_
#define _VMM_LIBVIRT_H_
#include <libvirt/libvirt.h>

// VM Monitor : Libvirt driver

class vmmLibvirt {
    virConnectPtr conn;
public:
    int *domId;
    int numDomains;
    vmmLibvirt(char *uri);
    unsigned long long getDomainCpuTime(int index);
    unsigned long long getNodeCpuTime();
    int getDomainID(int **domID);
    virDomainPtr domainLookupByID(int domID);
};


#endif
