#include "PaCGenerator.h"
#include "Analysis.h"
#include "Globals.h"
#include "Log.h"
#include "BasicProperties.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;
int main(int argc, char * argv[]){

	//This is, unfortunately, really lazy. 
	// needs to become a full-fledged command line tool ASAP
	if(argc < 11){
		cout << "Usage: Simulation [outputDirectory] "<<
			"[numAgents] [numDays]" <<
			"[startup_cost_phone] [startup_cost_sms]"<<
			"[cpm_phone] [cpm_sms]"<<
			"[alpha_phone] [alpha_sms] [power_function]"<<
			"[seed- default time(0)]" << endl;
		exit(-1);
	} 

	string allOutName = string(argv[1]) + "/all_sim.csv";
	string smsOutName = string(argv[1]) + "/sms_sim.csv";
	string phoneOutName = string(argv[1]) + "/phone_sim.csv";
	string analysisOutputName = string(argv[1]) + "/sim_combined_out.csv";
	string idsFileName = string(argv[1]) + "/ids.txt";
	
	unsigned int seed = 0;
	if(argc==12){
		stringstream ss;
		ss << argv[11];
		ss >> seed;
	} else{
		seed = time(0);
	}

	///write out parameters to file
	ofstream paramsFile(string(argv[1])+"/params.csv");
	paramsFile << "NumAgents,"<<argv[2]<<
				  "\nDays,"<<argv[3]<<
				  "\nStartupCost_Phone,"<<argv[4]<<
				  "\nStartupCost_SMS," <<argv[5]<<
				  "\nCPM_Phone,"<<argv[6]<<
				  "\nCPM_sms,"<<argv[7]<<
				  "\nAlpha_Phone,"<<argv[8]<<
				  "\nAlpha_SMS,"<<argv[9]<<
				  "\nPower_Function,"<<argv[10]<<
				  "\nSeed,"<<seed<<endl;
	paramsFile.close();

	Globals::NUM_AGENTS = atoi(argv[2]);
	Globals::DAYS = atoi(argv[3]);
	Globals::StartupCost_phone=atof(argv[4]);
	Globals::StartupCost_sms=atof(argv[5]);
	Globals::Cost_Per_Min_phone=atof(argv[6]);
	Globals::Cost_Per_Min_sms = atof(argv[7]);
	Globals::alphaPhone = atof(argv[8]);
	Globals::alphaSMS = atof(argv[9]);
	Globals::choiceExponent = atof(argv[10]);

	Log::initializeLog(allOutName,smsOutName,phoneOutName);
	PaCGenerator gen(allOutName, smsOutName, phoneOutName, idsFileName);
	gen.run();

	//Analysis analysis;
	
	//analysis.analyze(string(argv[1]),
	//	analysisOutputName,
	//	allOutName,
	//	smsOutName,
	//	phoneOutName,
	//	idsFileName);
}