# vmMonitor
A C++ app to monitor VM utilization using libvirt

Build
-----
```
make clean
make all
```

Usage
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
