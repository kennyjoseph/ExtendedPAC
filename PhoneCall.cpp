#include "PhoneCall.h"
#include "Agent.h"
#include "Globals.h"
#include "Log.h"
#include <iostream>

PhoneCall::PhoneCall(Agent*send, vector<Agent*> receive, int min, double reason, double bounces):
Interaction(send,receive,min,Globals::StartupCost_phone, Globals::Cost_Per_Min_phone,
	Globals::Cost_Per_Additional_Person_phone, reason, bounces, Globals::alphaPhone){
	
	 //We only want one phone call ... if we have multiple receivers, we ignore them, at least for now
	receiver = receive[0];

	benefit = getBenefit(sender->friendliness,receiver->friendliness);
}

void 
PhoneCall::send(bool isReply){
	Interaction::send(isReply);

	//If they are currently on a call
	if(receiver->currInteraction){
		vector<Agent*> send;
		send.push_back(sender);
		receiver->interactionPQ.push(shared_ptr<PhoneCall>(new PhoneCall(receiver,send,startMinute+1,importance,bounces+1)));	
		completeInteraction();
	} else {
		//otherwise, he answers this call
		receiver->currInteraction = shared_from_this();
	}
	
}

void
PhoneCall::completeInteraction(){
	Log::getInstance()->printInteraction(shared_from_this());
	bool hadRecv = receiver->currInteraction && (receiver->currInteraction->sender->id == sender->id);

	sender->updateNeighbor(getPayoff(false),receiver);
	if(hadRecv){
		receiver->updateNeighbor(getPayoff(true),sender);
		receiver->currInteraction = 0;
		sender->idsConversedWith.insert(receiver->id);
		receiver->idsConversedWith.insert(sender->id);
	}

	sender->numP++;
	receiver->numPR++;
	sender->totPhonePay+= getPayoff(true);
	receiver->totPhonePay+=getPayoff(false);
	sender->lastPhonePayoff = getPayoff(true);
	
	sender->currInteraction = 0;		
}

void 
PhoneCall::interrupt(){
	interruptedCount++;
	sender->interactionPQ.push(shared_from_this());
	sender->currInteraction = 0;
	receiver->currInteraction = 0;
}
