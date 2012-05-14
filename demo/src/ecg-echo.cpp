#include <boost/program_options.hpp>
#include <Functions.h>
#include <ecg.h>

string _domainid,_deviceid,_loginfo,_logdata,_logconfpath;

int main(int argc, char* argv[]) 
{
	if (!parse_args_sub(argc, argv,_domainid,_deviceid,_loginfo,_logdata,_logconfpath))
    	return 1;

	echoSubscribeECG(_domainid,_deviceid,_loginfo,_logdata,_logconfpath);
	return 0;
}

