# Makefile 

# The top level directory
TOPDIR := $(shell pwd)

# The source directory locations
IDL_DIR=$(TOPDIR)/idl
CPP_DIR=$(TOPDIR)
CPP_LIB_DIR=$(TOPDIR)/lib
ecg_LIB=$(TOPDIR)/../lib

# The C/C++ compilers to use
#CPP=/usr/bin/g++ 
CPP=/usr/bin/g++
IDLPP= idlpp

# The C/C++ compiler flags
CPPFLAGS= -g -Wredundant-decls -Wall -I/opt/mongo-cxx-driver-v1.8/mongo/client -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
LDFLAGS= -L/opt/mongo-cxx-driver-v1.8 -L/lib64 -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
LIBS=  -lboost_thread -lboost_program_options -lboost_system -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  /lib/libsimpledds.a -llecg /lib/liblogger.a

# Boost Libraries is used by the  publisher/generator examples
#BOOST_LIBS= -lboost_thread -lboost_program_options -lboost_system

# The target programs to build
TARGETS= \
	$(CPP_DIR)/bin/ecg-pub \
	$(CPP_DIR)/bin/ecg-echo 

all: $(TARGETS)

clean: 
	rm -f $(TOPDIR)/bin/*; \
	rm -f $(TOPDIR)/src/*.o;


# Compile the ecgPublish binary
$(CPP_DIR)/bin/ecg-pub:
	cd $(CPP_DIR)/src; \
	$(CPP) $(CPPFLAGS) -c $(CPP_DIR)/src/ecg-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -o $(TOPDIR)/bin/ecg-pub $(CPP_DIR)/src/ecg-pub.o /lib/liblecg.a $(ecg_LIB)/gen/ecg/libecg.a $(LIBS);

# Compile the ecgPublish binary
$(CPP_DIR)/bin/ecg-echo:
	cd $(CPP_DIR)/src; \
        $(CPP) $(CPPFLAGS) -c $(CPP_DIR)/src/ecg-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -o $(TOPDIR)/bin/ecg-echo $(CPP_DIR)/src/ecg-echo.o /lib/liblecg.a $(ecg_LIB)/gen/ecg/libecg.a $(LIBS);
