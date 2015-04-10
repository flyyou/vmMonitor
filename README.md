# vmMonitor
vmMonitor monitors the private cloud for unused/underutilized VM's. It uses the CPU, memory  and disk usage (as viewed by the host machine) to determine resource utilization of VMs. The libvirt library is used to remotely connect to hosts to collect statistics. The program prints to stdout all VMs which have resource utilizations less that the configured thresholds.

## Build
-----
```
make clean
make all
```

## Usage
-----
```
vmmon -help
Usage: vmmon [options] [FILENAME]
       FILENAME
		File containing IP addresses or hostnames (one per line) of the nodes to be monitored.
       OPTIONS
		-t Run time in minutes. Specify either -t or -n.
		-x Number of processing threads (default=1)		 
		-c CPU threshold percentage (default=20)
		-m Memory threshold percentage (default=20)
		-d Disk threshold percentage (default=20)
		-i Monitor interval in secs (default=10)
		-n Number of iterations (default=1)
```
e.g.
```
vmmon -t 1 test-nodes-file
server: 127.0.0.1                      vmid: 14   cpu_util: 0%
server: 127.0.0.1                      vmid: 16   cpu_util: 0%
server: 127.0.0.1                      vmid: 17   cpu_util: 0%
server: 127.0.0.1                      vmid: 18   cpu_util: 0%
server: 127.0.0.1                      vmid: 19   cpu_util: 0%
server: ansari-ThinkPad-T61            vmid: 14   cpu_util: 0%
server: ansari-ThinkPad-T61            vmid: 16   cpu_util: 0%
server: ansari-ThinkPad-T61            vmid: 17   cpu_util: 0%
server: ansari-ThinkPad-T61            vmid: 18   cpu_util: 0%
server: ansari-ThinkPad-T61            vmid: 19   cpu_util: 0%
server: localhost                      vmid: 14   cpu_util: 0%
server: localhost                      vmid: 16   cpu_util: 0%
server: localhost                      vmid: 17   cpu_util: 0%
server: localhost                      vmid: 18   cpu_util: 0%

````



## Entity Relationship

```
                    +-----------+            +---------+
                    | vmMonitor |------------| vmmTopo |
                    +-----------+            +---------+
                          |
 +-----------+      +-----------+            +-------------+
 | vmmConfig |----- | vmmStats  |------------| vmmStatsRec |
 +-----------+      +-----------+            +-------------+
                          |
                    +------------+
                    | vmmLibvirt |
                    +------------+


```

## Resources
vmMonitor service
```
Attribute		Type		Required   Default	Description
-------------------------------------------------------------------------------
status                  String		Yes	   Stopped	Running or Stopped
```

Config (vmmConfig)
```
Attribute		Type		Required   Default	Description
-------------------------------------------------------------------------------
tenantId
runTime
numThreads
cpuThreshold
memThreshold
diskThreshold
monitorInterval
numIterations
```
Network Topology (vmmTopo)
```
Attribute	Type		Required   Default	Description
-------------------------------------------------------------------------------
nodeFile
```
Statistics Collector (vmmStats)
```
Attribute	Type		Required   Default	Description
-------------------------------------------------------------------------------
```
Statistics record (vmmStatsRec)
```
Attribute	Type		Required   Default	Description
-------------------------------------------------------------------------------
timestamp	Integer					Timestamp of stats record
node		String					Host IP address / hostname
vmID		Integer					VM Id
vmCpuUtil	Integer					Percent CPU utilization
```
Libvirt Driver (vmmLibvirt)
```
Attribute	Type		Required   Default	Description
-------------------------------------------------------------------------------
connectionType	String					qemu or tls
```

## API Operations
Base URL:

## ToDo
- Multi-process/thread implementation. The present implementation is single threaded. 
- Add memory and disk monitoring. The present implementation only supports CPU utlization monitoring.
- Rest API interface to the application.
