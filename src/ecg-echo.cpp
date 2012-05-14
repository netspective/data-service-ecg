#include "ccpp_ecg.h"
#include <ecg.h>
#define devid "deviceID"

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

std::stringstream temp,prtemp;

int echoSubscribeECG(string domainid,string deviceid,string loginfo,string logdata,string logconfpath)
{


	 
	 /*Initializing SimpleDDS library*/
	 DataService *simpledds;
	 ECGTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 ECGDataReader_var ecgReader;
    	 ReturnCode_t status;
	 int i=0;
	
	 /*Initializing Subscriber and DataWriter*/
	 simpledds = new OpenSpliceDataService(loginfo,logconfpath);
	 typesupport = new ECGTypeSupport();
    	 simpledds->logger->info(" ecg Subscriber Started %s",deviceid.c_str());
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
	
	 ecgReader = ECGDataReader::_narrow(content_reader);
   	 ECGSeq  ecgList;
     	 SampleInfoSeq     infoSeq;
	 int m_count=0;
	 simpledds->logger->info("ecg Subscriber For %s",deviceid.c_str());
	 simpledds->logger->info("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, TimeInSeconds, AmplitudeMillivolts, WaveformPeak, StandardDeviation, Signal");
	 /*Receiving Data from DDS */
	 while (1) 
	 {
         	status = ecgReader->take(
            	ecgList,
            	infoSeq,
            	LENGTH_UNLIMITED,
            	ANY_SAMPLE_STATE,
           	ANY_VIEW_STATE,
            	ANY_INSTANCE_STATE);
         	checkStatus(status, "take");
          	if (status == RETCODE_NO_DATA) 
		{
          		continue;
          	}
          	for (i = 0; i < ecgList.length(); i++) 
	  	{
			
			if(infoSeq[i].valid_data)
			{
			prtemp <<ecgList[i].deviceDomain<<COMMA;
		        prtemp <<ecgList[i].deviceID <<COMMA<<ecgList[i].timeOfMeasurement<<COMMA<< ecgList[i].timeInSeconds;
			prtemp <<COMMA<<ecgList[i].amplitudeMillivolts<<COMMA<<ecgList[i].waveformPeak;
			prtemp <<COMMA<<ecgList[i].standardDeviation<<COMMA<<ecgList[i].signalECG;
			simpledds->logger->info(prtemp.str().c_str());
			prtemp.str(CLEAN);
			cout << m_count<<"\n";
			m_count++;
			}
	  	}
		status = ecgReader->return_loan(ecgList, infoSeq);
       		checkStatus(status, "return_loan");
       
    	}
        /* We're done.  Delete everything */
	simpledds->logger->info("ecg Subscriber Ends");	
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;


}

