#include "Interaction.h"
#include "Agent.h"
#include "Globals.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;


Interaction::Interaction(Agent*send, vector<Agent*> receive, int min, 
					double rich, double cost,double cost_addl, 
					double reason, double bounce, double a):
	startMinute(min), callLength(0),
	richness(rich),cost_pm(cost), cost_per_additional_agent(cost_addl),interruptedCount(0), 
	lastPayoff(0), interruptProbability(0), importance(reason),currPayoff(0),bounces(bounce),alpha(a){
	
	sender = send;
	originalIDs.insert(sender->id);
	receivers = receive;
	//@TODO: may not work w/ more than 1 receiver
	for(int i = 0; i < receivers.size(); i++){
		receivers[i]->subscribers.insert(sender);
		sender->subscribers.insert(receivers[i]);
		originalIDs.insert(receivers[i]->id);
	}
}

void
Interaction::send(bool isReply){
	callLength++;
	currPayoff =alpha*benefit - cost_pm;
	sender->capital -=cost_pm;
}

bool
Interaction::involves(Agent* deletedAgent){
	return originalIDs.count(deletedAgent->id);
}

bool
Interaction::hadAgentDeleted(){
	if(!originalIDs.count(sender->id)){
		return true;
	}
	for(int i = 0; i < receivers.size();i++){
		if(!originalIDs.count(receivers[i]->id)){
			return true;
		}
	}
	return false;
}
//Return true if the interaction has ENDED
void
Interaction::update(int currentMinute){

	if(sender->capital < cost_pm+richness || lastPayoff > currPayoff){
		//If I can't communicate any more, the interaction is over
		completeInteraction();
		return;
	}

	lastPayoff = currPayoff;

	callLength++;
	double a = 1 - pow(alpha,callLength);
	a /= (1 -alpha);
	currPayoff = benefit * a - cost_pm* callLength;

	sender->capital-=cost_pm;

}


