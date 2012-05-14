#include <boost/program_options.hpp>
#include <Functions.h>
#include <ecg.h>

int _heartbeats=256;
int _ecgsample=256;
int _internalsample=256;
float _amplitudenoise=0.0;
float _heart_rate_mean=60.0;
float _heart_rate_std=1.0;
float _lowfreq=0.1;
float _highfreq=0.25;
float _lowfreqstd=0.01;
float _highfreqstd=0.01;
float _lfhfradio=0.5;

int main(int argc, char* argv[]) 
{
	string _domainid,_deviceid,_loginfo,_logdata,_logconfpath;

	if (!parse_args_pub_ecg(argc, argv,_domainid,_deviceid,_loginfo,_logdata,_logconfpath,_heartbeats,_ecgsample,_internalsample,_amplitudenoise,_heart_rate_mean,_heart_rate_std,_lowfreq,_highfreq,_lowfreqstd, _highfreqstd,_lfhfradio))
	return 1;

	publishECG(_domainid,_deviceid,_loginfo,_logdata,_logconfpath,_heartbeats,_ecgsample,_internalsample,_amplitudenoise,_heart_rate_mean,_heart_rate_std,_lowfreq,_highfreq,_lowfreqstd, _highfreqstd,_lfhfradio);
	return 0;
}

