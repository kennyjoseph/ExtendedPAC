#include "SMS.h"
#include "Globals.h"
#include "Agent.h"
#include "Log.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <hash_map>
using namespace std;

SMS::SMS(Agent*send, vector<Agent*> receive, int min, double reason, double bounces):
Interaction(send,receive,min,Globals::StartupCost_sms, Globals::Cost_Per_Min_sms,
	Globals::Cost_Per_Additional_Person_sms, reason, bounces,Globals::alphaSMS){

	double avgFr = 0;
	vector<Agent*>::iterator receiversEnd = receivers.end();
	for(vector<Agent*>::iterator it = receivers.begin(); it != receiversEnd; it++){
		avgFr += (*it)->friendliness;
	}

	avgFr /= receivers.size();
	benefit = getBenefit(sender->friendliness,avgFr);

}


void 
SMS::send(bool isReply){
	Interaction::send(isReply);
}

void
SMS::completeInteraction(){

	Log::getInstance()->printInteraction(shared_from_this());

	vector<Agent*>::iterator receiversEnd = receivers.end();

	//uses originalPayoff
	
	sender->updateNeighbors(getPayoff(false), receivers);
	
	//uses other...
	currPayoff /= receivers.size();
	for(vector<Agent*>::iterator it = receivers.begin(); it != receiversEnd; it++){
		Agent* receiver = *it;

		shared_ptr<Interaction> toPushOnStack(new SMS(sender,receivers,startMinute, importance,bounces+1));
		receiver->interactionPQ.push(toPushOnStack);

		receiver->numER++;
		sender->totSMSPay+= getPayoff(false);
		sender->totOut += getPayoff(false);
		receiver->totIn += getPayoff(true);
		receiver->totSMSPay+=getPayoff(true);
	}
	sender->numE += receivers.size();
	sender->currInteraction = 0;
}


void 
SMS::interrupt(){
	interruptedCount++;
	sender->interactionPQ.push(shared_from_this());
}
