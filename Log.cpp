#include "Log.h"
#include "Agent.h"
#include "Interaction.h"
#include "PhoneCall.h"
#include "SMS.h"
#include <string>
#include <iostream>
using namespace std;

Log* Log::pInstance = 0;

Log::Log(string allOutFileName, string smsOutFileName, string phoneOutFileName, string attributesFileName):
	allOut(allOutFileName),smsOut(smsOutFileName),
	phoneCallOut(phoneOutFileName),numInteractions(0),
	agentAttributesOut(attributesFileName){

	string header = "Sender,Receiver,Time,Type,callLength,day,";
	smsOut << header << endl;
	phoneCallOut<<header << endl;

	allOut << header << "type,senderType,RecvType,weight"<<endl;
	agentAttributesOut << "day,agent_id,capital,friendliness,numNeighbors,qSize,avgPayoff,expPayoffFromStrangers,"<<
		"NumPhone,NumSMS,NumPhoneR,NumSMSR,TotPhone,TotSMS,TimesRec,ProbReset,ExpPhone,ExpSMS,TotOut,TotIn,ExpOut,ExpIn,Asks,Reply,Init"<<endl;
}

void 
Log::initializeLog(string allOutFN, string smsFN, string phoneFN){
	pInstance = new Log(allOutFN,smsFN,phoneFN,"attributes.csv");
}

Log* Log::getInstance(){
  if(pInstance == 0){
	cout << "Please initialize log first... sorry.."<<endl;
	exit(-1);
  }
  return pInstance;
}

 void writeInteraction(ofstream& out, shared_ptr<Interaction> interaction, const string& medium, const string& addToOut){
	 	vector<Agent*>::iterator receiversEnd = interaction->receivers.end();
	 for(vector<Agent*>::iterator it = interaction->receivers.begin(); it != receiversEnd; it++){
		out << interaction->sender->id << "," << 
			(*it)->id << "," <<
			interaction->startMinute*60 << "," << 
			medium  << "," << 
			interaction->callLength << ","  <<
			interaction->startMinute/Globals::MINUTES_IN_DAY << "," << 
			addToOut << endl;
		
		//<<"," << interaction->sender->capital << "," << interaction->sender->numNeighbors << ","
		//<< interaction->receiver->capital << "," << interaction->receiver->numNeighbors;
		
	 }
	 //cout << interaction->importance << " \t" << interaction->sender->interactionPQ.size() << endl;
}

void 
Log::printInteraction(shared_ptr<Interaction> interaction){
	
	numInteractions++;
	int day = interaction->startMinute % Globals::MINUTES_IN_DAY;

	if(dynamic_pointer_cast<PhoneCall>(interaction)){
		writeInteraction(allOut,interaction, "Voice","Agent,Agent,1");
		writeInteraction(phoneCallOut,interaction,"Voice","");
	} else if(dynamic_pointer_cast<SMS>(interaction)){
		writeInteraction(allOut,interaction, "SMS","Agent,Agent,1");
		writeInteraction(smsOut,interaction,"SMS","");
	}
}


void 
Log::printAgentAttributes(vector<Agent>& agents, int day){
	for(vector<Agent>::iterator it = agents.begin(); it != agents.end(); it++){
		agentAttributesOut  << day << "," << it->id << "," << it->capital << "," << it->friendliness << "," <<
			it->neighbors.size() << "," << it->interactionPQ.size()  << "," << it->avgPayoff << "," << it->expPayoffFromStrangers ;//<< ",";
	/*	if(it->neighbors.size() != 0){
			agentAttributesOut << it->relationships[it->neighbors.front()->id].lastPayoff  << "," <<
			it->relationships[it->neighbors.front()->id].numInteractions  << "," <<
			it->relationships[it->neighbors.front()->id].numSends << "," <<
			it->relationships[it->neighbors.back()->id].lastPayoff  << "," <<
			it->relationships[it->neighbors.back()->id].numInteractions << "," <<
			it->relationships[it->neighbors.back()->id].numSends;
		}*/
		agentAttributesOut << "," << it->numP << "," << it->numE << "," << it->numPR << "," << it->numER << "," << it->totPhonePay << "," << it->totSMSPay << "," << it->timesRec << 
			"," << it->probOfReset << "," << it->totPhonePay / (1+it->numP+it->numPR) << "," << it->totSMSPay/(1+it->numE+it->numER) << 
			"," << it->totOut << "," << it->totIn << "," << it->totOut/(it->numE+1) << "," << it->totIn/(it->numER+1) << "," << it->asksForRec << "," << it->numReply << "," << it->numInit << endl;
	}
}




void 
Log::printMessage(const std::string& message){
	allOut << message << endl;
}

void 
Log::printMessageWithAgent(const std::string& message, Agent* agent){
	allOut << message << " AGENT: " << agent->id << endl;
}

void 
Log::printMessageWithValue(const std::string& message, int val){
	allOut << message << val << endl;
}

 void 
Log::printMessageWithValue(const std::string& message, double val){
	allOut << message << val << endl;
}

