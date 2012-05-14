#include "ccpp_ecg.h"
#include <ecg.h>
#define devid "deviceID"
#include <fstream>
#include "ecgsyn.cpp"
using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

char buf[BUFFERSIZE], c;
stringstream prtemp,datacommand;


int publishECG(string domainid,string deviceid,string loginfo,string logdata,string logconfpath,int heartbeats,int ecgsample,int internalsample, float amplitudenoise, float heart_rate_mean,float heart_rate_std,float lowfreq,float highfreq,float lowfreqstd,float highfreqstd,float lfhfradio)
{
	heartbeats=256;                 
	ecgsample=256;               
	internalsample=256;
	amplitudenoise=0.0;          
	heart_rate_mean=60.0;        
	heart_rate_std=1.0;          
	lowfreq=0.1;             
	highfreq=0.25;           
	lowfreqstd=0.01;         
	highfreqstd=0.01;        
	lfhfradio=0.5; 
	int flag;
	

	
	/*Initializing SimpleDDS library*/
	DataService *simpledds;
	ECGTypeSupport_var typesupport;
	DataWriter_ptr writer;
	ECGDataWriter_var ecgWriter;


	simpledds = new OpenSpliceDataService(loginfo,logconfpath);
	typesupport = new ECGTypeSupport();
	writer = simpledds->publish(typesupport);
	ecgWriter = ECGDataWriter::_narrow(writer);

	simpledds->logger->info(" ecg Publisher Started %s",deviceid.c_str());

	/*Sending Commmand to The Data Generator*/
	datacommand<<"ECG"<<SEMI<<heartbeats<<SEMI<<ecgsample<<SEMI<<internalsample<<SEMI<<amplitudenoise;
	datacommand<<SEMI<<heart_rate_mean<<SEMI<<heart_rate_std<<SEMI<<lowfreq<<SEMI<<highfreq<<SEMI<<lowfreqstd<<SEMI<<highfreqstd<<SEMI<<lfhfradio;
	strcpy(buf,datacommand.str().c_str());

	flag=0;
	ECG data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	simpledds->logger->info("ecg Started Publishing Data In DDS");
	simpledds->logger->info("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, TimeInSeconds, AmplitudeMillivolts, WaveformPeak, StandardDeviation, Signal");	
	long count=0;
	int m_count=0;
	std::stringstream ecgfile,rrfile,rrpcfile;
	int loop_count=2; // maximum 3
	while (1) 
	{
		
			
		int heartbeats[3]={100, 105, 110};
		int ecgsample[3]={100, 105, 110};
		int internalsample[3]={100, 105, 110};
		float amplitudenoise=0;
		float heart_rate_mean=60;
		float heart_rate_std=1;
		float lowfreq=0.1;
		float highfreq=0.25;
		float lowfreqstd=0.01;
		float highfreqstd=0.01;
		float lfhfradio=0.5;
		time_t rawtime;
		time ( &rawtime );
		ecgfile<<rawtime;
		rrfile<<rawtime<<"rr";
		rrpcfile<<rawtime<<"rrpc";
		cout<<"Going to gerenting for : "<<heartbeats[loop_count]<<"\n";
		dorun(ecgfile.str().c_str(),rrfile.str().c_str(),rrpcfile.str().c_str(),heartbeats[loop_count],ecgsample[loop_count],internalsample[loop_count],amplitudenoise,heart_rate_mean,heart_rate_std,lowfreq,highfreq,lowfreqstd,highfreqstd,lfhfradio);
		string ecg,rr,rrpc;
		ifstream ifs(ecgfile.str().c_str());
		ifstream ifrr(rrfile.str().c_str());
		ifstream ifrrpc(rrpcfile.str().c_str());
		while(!ifrr.eof())
		{
			if(!ifs.eof())
			{
				if(!ifrrpc.eof())
				{
					fflush(stdout);
					stringstream ecgtemp;
					ecgtemp<<time(0)<<":";
					getline( ifs, ecg );
					for(int i = 0; i < ecg.length(); i++)
					{
						if( isspace(ecg[i]) )
						ecg[i] = ':';
					}	
					ecgtemp <<ecg<<":";
					getline( ifrr, rr );
					ecgtemp <<rr <<":";
					getline( ifrrpc, rrpc );
					ecgtemp<<rrpc;
					if((strcmp(ecg.c_str(),"")!=0)&&(strcmp(rr.c_str(),"")!=0)&&(strcmp(rrpc.c_str(),"")!=0))
					//send(client, ecgtemp.str().c_str(),1024, MSG_NOSIGNAL);
					{
						strcpy(buf,ecgtemp.str().c_str());
						string datasplit[7];
						datasplit[0] = strtok (buf,SEMI);
						datasplit[1] = strtok (NULL,SEMI);
						datasplit[2] = strtok (NULL,SEMI);
						datasplit[3] = strtok (NULL,SEMI);
						datasplit[4] = strtok (NULL,SEMI);
						datasplit[5] = strtok (NULL,SEMI);
			
						prtemp<<domainid<<COMMA<<deviceid<<COMMA;
			
						data.timeOfMeasurement = atol(datasplit[0].c_str());
						prtemp<<data.timeOfMeasurement<<COMMA;
					
						data.timeInSeconds = (double)atof(datasplit[1].c_str());		
						prtemp<<data.timeInSeconds<<COMMA;
		
		               		        data.amplitudeMillivolts = (double)atof(datasplit[2].c_str());   
		                       		prtemp<<data.amplitudeMillivolts<<COMMA;
					
						data.waveformPeak = (short)atoi(datasplit[3].c_str());   
		                        	prtemp<<data.waveformPeak<<COMMA;
							
						data.standardDeviation = (double)atof(datasplit[4].c_str());
						prtemp<<data.standardDeviation<<COMMA;


						data.signalECG = (double)atof(datasplit[5].c_str());
						prtemp<<data.signalECG;
						
						ecgWriter->write(data, NULL);

						simpledds->logger->info("%s %d",prtemp.str().c_str(),count++);
						prtemp.str(CLEAN);
						fflush(stdout);
					}



					usleep(9999);
					ecgtemp.str("");
				
				}
			}
		}
		
                if(loop_count==2)
                	loop_count=0;
                else
                	loop_count++;
		ifs.close();
		ifrr.close();
		ifrrpc.close();
		remove(ecgfile.str().c_str());
		remove(rrfile.str().c_str());
		remove(rrpcfile.str().c_str());
		ecgfile.str("");
		rrfile.str("");
		rrpcfile.str("");

	}
	
	/*Deleting SimpleDDS Instance*/
	simpledds->logger->info(" ecg Publisher Ends");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;
}

