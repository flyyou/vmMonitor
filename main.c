#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vmm.h"

int main(int argc, char *argv[]) {
    class vmMonitor vmm;

    vmm.init(argc, argv);
    vmm.collect();
    vmm.process();

    return 0;
}
