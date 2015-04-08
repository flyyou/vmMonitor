# vmMonitor
A C++ app to monitor VM utilization using libvirt

Build
-----
make clean
make all

Usage
-----
$ vmmon -help
Usage: vmmon [options] [FILENAME]
       FILENAME
		File containing IP addresses or hostnames (one per line) of the nodes to be monitored.
		If no file is specified, the localhost is monitored by default.
       OPTIONS
		-t Run time in minutes. Specify either -t or -n.
		-x Number of processing threads (default=1)		 
		-c CPU threshold percentage (default=20)
		-m Memory threshold percentage (default=20)
		-d Disk threshold percentage (default=20)
		-i Monitor interval in secs (default=10)
		-n Number of iterations (default=1)

e.g.
       vmmon -t 1 test-nodes-file
