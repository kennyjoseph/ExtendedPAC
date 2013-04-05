
#include "InteractionFactory.h"
#include "Globals.h"
#include "Agent.h"
#include "Interaction.h"
#include "SMS.h"
#include "PhoneCall.h"
#include "AgentVector.h"
#include "Relationship.h"
#include <cmath>
#include <memory>
#include <iostream>
#include <hash_map>
using namespace std;

InteractionFactory::InteractionFactory(){
}

InteractionFactory::~InteractionFactory(){
}

shared_ptr<Interaction>
InteractionFactory::getInteraction(Agent* sender, int minute,shared_ptr<Interaction> interactionToReplyTo){

	if(sender->capital < Globals::MIN_INTERACTION_COST){
		return 0;
	}
	int numToRecv = 1;
	int bounces = 0;

	InteractionType chosenType = LAST;

	vector<Agent*> receivers;
	double importance = (double)rand() / RAND_MAX;

	if(!interactionToReplyTo){
		int tieNumber = sender->findPartner(numToRecv,receivers);
		if(!receivers.size()){
			return 0;
		}

		double expPhone = (sender->numP + sender->numPR == 0) ? 0 :
				sender->totPhonePay/(sender->numP+sender->numPR);
		double expSMS = (sender->numE + sender->numER == 0) ?  0:
				sender->totSMSPay/(sender->numE+sender->numER);
		double probPhone = expPhone/(expPhone+expSMS);

		if(expPhone < 0 && expSMS >= 0){
			probPhone =0;  
		} else if(expSMS < 0 && expPhone >= 0){
			probPhone = 1;
		} else if(expPhone ==0 && expSMS ==0 ){
			probPhone = .5;
		}
		else if(expPhone < 0 & expSMS < 0){
			probPhone = 1-probPhone;
		}
		
		if(tieNumber == 1){}
		else if(probPhone < .5){
			probPhone = pow(probPhone, pow((double)tieNumber,Globals::choiceExponent));
		} else {
			probPhone = pow(probPhone, 1/pow((double)tieNumber,Globals::choiceExponent));
		}
		//cout << expPhone << " " << expSMS << endl;
		if((double)rand()/RAND_MAX < probPhone){
			chosenType=PHONE_ENUM;
		} else {
			chosenType=SMS_ENUM;
		}
	} else {
		bounces = interactionToReplyTo->bounces+1;

		if(dynamic_pointer_cast<SMS>(interactionToReplyTo)){
			chosenType = SMS_ENUM;
		} else if(dynamic_pointer_cast<PhoneCall>(interactionToReplyTo)){
			chosenType = PHONE_ENUM;
		}

		if(interactionToReplyTo->sender->id == sender->id){
			//These have been put off for later by me!
			receivers = interactionToReplyTo->receivers;
		} else {
			//RIGHT NOW ONLY RESPOND TO ONE PERSON...assumption...
			receivers.push_back(interactionToReplyTo->sender);
		}
	}

	//Now, I have the chosen type... I want to send this to as many people as I can at this time
	//Based on the media and my capital

	int maxRecp = (chosenType == PHONE_ENUM) ? Globals::PHONE_CALL_MAX_RECP : Globals::SMS_MAX_RECP;

	maxRecp = min((int)receivers.size(), maxRecp);

	double initCost = Globals::StartupCost_phone;

	double costPerAdditional = (chosenType == PHONE_ENUM) ? Globals::Cost_Per_Additional_Person_phone : Globals::Cost_Per_Additional_Person_sms;
	
	double capitalNeeded = initCost;
	int toRecv = 1;
		
	while(sender->capital >= capitalNeeded+costPerAdditional && toRecv < maxRecp){
		capitalNeeded += costPerAdditional;
		toRecv++;
	}
		
	//So Now, I interact with as many of these people at once as I can,
	// and put the rest on my queue for later

	vector<Agent*> thisInteractionReceivers(toRecv);

	std::copy(receivers.begin(), receivers.begin()+toRecv, thisInteractionReceivers.begin());
	receivers.erase(receivers.begin(),receivers.begin()+toRecv);

	if(chosenType == PHONE_ENUM){
		if(receivers.size() != 0){
			sender->interactionPQ.push(shared_ptr<Interaction>(new PhoneCall(sender,receivers,minute,importance,bounces)));//sender->getExpectedPayoff(receivers))));
		}
		if(thisInteractionReceivers.size() == 0){
			return 0;
		}
		return shared_ptr<Interaction>(new PhoneCall(sender,thisInteractionReceivers,minute,importance,bounces));//sender->getExpectedPayoff(thisInteractionReceivers)));
	}

	if(receivers.size() != 0){
		sender->interactionPQ.push(shared_ptr<Interaction>(new SMS(sender,receivers,minute,importance,bounces)));//sender->getExpectedPayoff(receivers))));
	}
	if(thisInteractionReceivers.size() == 0){
			return 0;
	}
	return shared_ptr<Interaction>(new SMS(sender,thisInteractionReceivers,minute,importance,bounces));//sender->getExpectedPayoff(thisInteractionReceivers)));

}


//hash_map<int, Relationship*>::iterator it = sender->relationships.find(receivers[0]->id);
		//double sendCount = (it == sender->relationships.end()) ? 0 : it->second->numSends;
		//double totSends = sender->numP+sender->numE;
		//double subtr = (totSends-sendCount <2) ? 1 : 1/log(totSends-sendCount);
		//probPhone =  pow(probPhone,subtr);
		//probPhone =  pow(probPhone,1/tieNumber);