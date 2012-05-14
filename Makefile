# Makefile 

# The top level directory
TOPDIR := $(shell pwd)

# The source directory locations
IDL_DIR=$(TOPDIR)/idl
CPP_DIR=$(TOPDIR)
CPP_LIB_DIR=$(TOPDIR)/lib

ifeq ($(strip $(PREFIX)),)

        TARGET_PATH=$(TOPDIR)
else
        TARGET_PATH=$(PREFIX)
endif


# The C/C++ compilers to use
#CPP=/usr/bin/g++ 
CPP=/usr/bin/g++
IDLPP= idlpp

# The C/C++ compiler flags
CPPFLAGS= -g -Wall -I/opt/mongo-cxx-driver-v1.8/mongo/client -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
LDFLAGS= -L/opt/mongo-cxx-driver-v1.8 -L/lib64 -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
LIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lsimpledds

# Boost Libraries is used by the  publisher/generator examples
BOOST_LIBS= -lboost_thread -lboost_program_options -lboost_system

# The target programs to build
TARGETS= \
	$(CPP_DIR)/bin/ecg-pub \
	$(CPP_DIR)/bin/ecg-sub-echo \
	$(CPP_DIR)/bin/lib

INSTALL_TARGETS= \
	ecg_install



# Build all targets
all: check_idlpp $(TARGETS)

install: $(INSTALL_TARGETS)

# Clean all files, including C++ generated from IDL
clean:
	rm -f /lib//libecg.so;  \
	rm -f /usr/include/ecg.h;  \
	rm -f /usr/include/Functions.h;  \
	rm -f $(CPP_DIR)/lib/gen/ecg/*;  \
	rm -f $(CPP_DIR)/*.a;  \
	rm -f $(CPP_DIR)/src/*.o;  \
	rm -f $(CPP_DIR)/lib/*.o; \
	rm -f $(TOPDIR)/bin/*
	rm -f $(TARGETS)

# Check that the idlpp compiler is in the current path
check_idlpp:
	@$(IDLPP) | grep Usage > /dev/null;


# Generate the ecg C++ source files from the IDL
$(CPP_DIR)/lib/gen/ecg/ecg.h:
	mkdir -p $(CPP_DIR)/lib/gen/ecg; \
	cd ${IDL_DIR}; $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/ecg ecg.idl; 

# Compile the ecg IDL-generated C++ files into libecg.a
$(CPP_DIR)/lib/gen/ecg/libecg.a: $(CPP_DIR)/lib/gen/ecg/ecg.h
	cd $(CPP_DIR)/lib/gen/ecg; \
	$(CPP) $(CPPFLAGS) -c ecg.cpp; \
        $(CPP) $(CPPFLAGS) -c ecgDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c ecgDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c ecgSplDcps.cpp; \
        ar cr libecg.a ecg.o ecgDcps.o ecgDcps_impl.o ecgSplDcps.o; \
        ranlib libecg.a

# Compile the Functions helper class
Functions.o: $(CPP_LIB_DIR)/Functions.cpp
	$(CPP) $(CPPFLAGS) -c $(CPP_LIB_DIR)/Functions.cpp


# Compile the ecgOximeterPublish binary
$(CPP_DIR)/bin/ecg-pub: $(CPP_DIR)/lib/gen/ecg/libecg.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/src; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/ecg -c $(CPP_DIR)/src/ecg-pub.cpp -o  $(CPP_DIR)/src/ecg-pub.o $(LDFLAGS) $(LIBS); 

# Compile the ecgOximeterSubscribe binary
$(CPP_DIR)/bin/ecg-sub-echo: $(CPP_DIR)/lib/gen/ecg/libecg.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/src; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/ecg -c $(CPP_DIR)/src/ecg-echo.cpp -o $(CPP_DIR)/src/ecg-echo.o $(LDFLAGS) $(LIBS);

# Creating Libraries
$(CPP_DIR)/bin/lib:
	cd $(CPP_DIR)/src; \
	ar cr $(CPP_DIR)/liblecg.a $(CPP_DIR)/src/ecg-pub.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/ecg/libecg.a \
	$(CPP_DIR)/src/ecg-echo.o ; \
        ranlib $(CPP_DIR)/liblecg.a; \
	
ecg_install:
	mkdir -p $(TARGET_PATH)/lib; \
        mkdir -p $(TARGET_PATH)/include; \
	cp $(CPP_DIR)/liblecg.a $(TARGET_PATH)/lib; \
	cp $(CPP_DIR)/liblecg.a /lib; \
        cp $(CPP_DIR)/lib/ecg.h $(TARGET_PATH)/include; \
        cp $(CPP_DIR)/lib/ecgsyn.cpp $(TARGET_PATH)/include; \
        cp $(CPP_DIR)/lib/ecg.h /usr/include; \
        cp $(CPP_DIR)/lib/ecgsyn.cpp /usr/include; \
        cp $(CPP_DIR)/lib/Functions.h /usr/include;

