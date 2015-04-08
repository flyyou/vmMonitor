#ifndef _VMM_LIBVIRT_H_
#define _VMM_LIBVIRT_H_
#include <libvirt/libvirt.h>

// VM Monitor : Libvirt driver

int vmmGetDomains(virConnectPtr conn,  int **domID);
unsigned long long vmmGetCPUTime(virDomainPtr dom);

#endif
