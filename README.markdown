# ECG Library

ECG library is intended to greatly simplify the use of ECG device simulation with DDS. As such, ECG library have functions that reduces by several times the line of code that need to be written for a creating ECG Simulation DDS application. 

# Prerequisite

[Setting Up Logging Service](https://github.com/netspective/commons-logging-cpp/blob/master/README.md)

# Compilation and Installation
Clone the library from the github

	git clone git@github.com:netspective/data-service-ecg.git
	
	cd data-service-ecg/

Compiling
	
	make

Installing
	
	make install 

	or 

	make install PREFIX=<installation path>



## Simplified Functions
Following are ECG functions and their descriptions

Note: Refer the demo programs for reference

#### Running Publisher 

SYNOPSIS

	int publishECG(string deviceid,string domainid,string loginfo,string logdata,string logconfpath);

DESCRIPTION

	The publish function used to publish the ECG data in the DDS. 

ARGUMENTS

	domain             Device Domain 

	device-id          Device ID - for device identification

	log-info           Log information category

	lod-data           Log data file 

	log4cpp-conf       Log configuration and format specification file


#### Echo Subscribers

SYNOPSIS

	int echoSubscribeECG(string deviceid,string domainid,string loginfo,string logdata,string logconfpath);

DESCRIPTION

This function is used to subscribe and join the ECG topic. Log4cpp config is used to maintain the logs and thier path.
 
ARGUMENTS

	domain             Device Domain 

	device-id          Device ID - for device identification

	log-info           Log information category

	lod-data           Log data file 

	log4cpp-conf       Log configuration and format specification file
