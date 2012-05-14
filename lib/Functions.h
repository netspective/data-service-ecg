#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
/*String literals*/

#define DOT "."
#define COMMA ", "
#define CLEAN ""
#define SEMI ":"
#define BUFFERSIZE 1024
#define TIMESTAMP "timeStamp"
#define DEVICEID "deviceID"
#define SYSTOLIC "systolic"
#define DIASTOLIC "diastolic"
#define PULSERATE "pulseRate"
#define SPo2 "SPO2"
#define TEMPERATURE "temperature"





using namespace std;

namespace po = boost::program_options;

bool parse_args_pub_ecg(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath,int &heartbeats,int &ecgsample,int &internalsample, float &amplitudenoise, float &heart_rate_mean,float &heart_rate_std,float &lowfreq,float &highfreq,float &lowfreqstd,float &highfreqstd,float &lfhfradio);

bool parse_args_sub(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logcatedata);




string alarmString(int value,int min,int max);
#endif


