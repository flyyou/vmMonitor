CC = gcc
CXX = g++

INCLUDES =
LIBS = -lvirt

CFLAGS = -g

ALL_PROGRAMS = vmmon
OBJS = main.o \
       vmm.o \
       vmm_libvirt.o \
       vmm_topo.o \
       vmm_stats.o \
       vmm_config.o

default::
	@echo "You must say what you want to build.  For example:"
	@echo "make all - build all programs"
	@echo "make progname - build the 'progname' program"
	@echo "Permitted values of progname include:"
	@echo "$(ALL_PROGRAMS)"

all:: $(ALL_PROGRAMS)

.c.o:
	$(CXX) -c $(CFLAGS) $(INCLUDES) $<

vmmon:: $(OBJS) 
	$(CXX) -o $@ $(OBJS) $(LIBS)

clean::
	rm -f $(ALL_PROGRAMS) *.o
