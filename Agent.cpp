#include "Agent.h"
#include "Interaction.h"
#include "InteractionFactory.h"
#include "Globals.h"
#include "AgentVector.h"
#include "Log.h"
#include "Relationship.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


struct AgentFriendComp{
	AgentFriendComp(){}
	AgentFriendComp(Agent* ag){
		agent = ag;
	}
	bool operator() (Agent* lhs,Agent* rhs) const{
		double lhs_payoff = agent->relationships[lhs->id]->lastPayoff;
		double rhs_payoff = agent->relationships[rhs->id]->lastPayoff;
		return (lhs_payoff == rhs_payoff) ? lhs->friendliness > rhs->friendliness : lhs_payoff > rhs_payoff; 
	}

	Agent* agent;
};


Agent::Agent(int identity):
		id(identity),
		expPayoffFromStrangers(0),
		avgPayoff(0),
		currInteraction(0),
		numE(0), numP(0),numER(0),numPR(0),totPhonePay(0),totSMSPay(0),timesRec(0), totOut(0), totIn(0),asksForRec(0), numInit(0),numReply(0),
		agentVector(AgentVector::getInstance())
	{
		setRandomVars();
	}	

		
inline void
Agent::setRandomVars(){
	capital=Globals::initialAgentCapital;
	capitalInWaiting=0;
	friendliness=(double)rand()/RAND_MAX;
	probOfReset=(double)rand()/RAND_MAX;
	expPayoffFromStrangers=friendliness;
}

void Agent::spendCapital(int minute, InteractionFactory* interactionFactory){

	if(currInteraction){
		//If someone has called me and I'm on the phone with them, they update
		if(currInteraction->sender->id != id){
			return;
		}
		//Update changes currInteraction to null if the interaction is over
		currInteraction->update(minute);
		return;
	}

	if( capital < Globals::MIN_INTERACTION_COST){
		capital+=capitalInWaiting;
		capitalInWaiting=0;
		idsConversedWith.clear();
		return;
	}

	//I'm not on an interaction, so I can start a new one if I have enough capital
	//The factory determines this for me... I never know what type of interaction
	//I am on

	//If I have nothing to respond to, pick a friend/stranger/recommended agent and send them an interaction
	if(interactionPQ.empty()){
		currInteraction = interactionFactory->getInteraction(this, minute);
		if(currInteraction){
			numInit += currInteraction->receivers.size();
			currInteraction->send(false);
		}
		return;
	}

	//otherwise, find the first interaction on my queue
	shared_ptr<Interaction> i = interactionPQ.top();
	interactionPQ.pop();
	if(i->hadAgentDeleted()){
		return;
	}
	if((double)rand()/RAND_MAX > 1./i->bounces){
		//this conversation is over
		int idToAdd = (i->sender->id == id) ? i->receivers[0]->id : i->sender->id;
		idsConversedWith.insert(idToAdd);
		return;
	}

	if(i->sender->id != id){
		updateNeighbor(i->getPayoff(true), i->sender);
	}

	currInteraction = interactionFactory->getInteraction(this,minute,i);
	//if I had enough capital to send the response, I do it
	if(currInteraction){		
		numReply++;
		currInteraction->startMinute = minute;
		currInteraction->send(true);
	}
}


int
Agent::findPartner(int maxNum, vector<Agent*>& receivers){
	NeighborSet::iterator it;

	int tieNumber = 0;
	for(NeighborSet::iterator it = neighbors.begin();it != neighbors.end() && receivers.size() < maxNum;it++){
		tieNumber++;
		int id = (*it)->id;
		if(relationships[id]->lastPayoff > expPayoffFromStrangers && !idsConversedWith.count(id)){ 
			receivers.push_back(*it);
		}
	}

	int needed = maxNum - receivers.size();

	vector<Agent*> newAgents;
	if(needed == 0){
		return tieNumber;
	}

	asksForRec++;
	getRecommendations(needed,newAgents);
	receivers.insert(receivers.end(), newAgents.begin(),newAgents.end());
	
	for(vector<Agent*>::iterator it = newAgents.begin(); it != newAgents.end(); it++){
		(*it)->timesRec++;
	}

	while(receivers.size() < maxNum){
		Agent* rand = agentVector->getRandomAgent(this);
		if(!rand) return -1;
		receivers.push_back(rand);
	}	
	return (tieNumber==0) ? 1 :tieNumber;
}

void
Agent::getRecommendations(int numNeeded,vector<Agent*>& receivers){
	
	vector<double> payoffs(numNeeded);
	double minPayoff = -10000;
	int minPayoffIndex = -1;

	NeighborSet::iterator neighbEnd = neighbors.end();
	hash_map<int, Relationship*>::iterator relEnd = relationships.end();

	for(NeighborSet::iterator neighborIt = neighbors.begin(); neighborIt != neighbEnd; ++ neighborIt)
	{	
		Agent* neighbor = *neighborIt;
		NeighborSet::iterator ff2End = neighbor->neighbors.end();
		
		for(NeighborSet::iterator fofIt = neighbor->neighbors.begin(); fofIt!= ff2End; ++ fofIt)
		{
			int fofId = (*fofIt)->id;
			double payoff =neighbor->relationships[fofId]->lastPayoff ;

			if(fofId == id || relationships.find(fofId) != relEnd){
			}
			else if(receivers.size() < numNeeded){
				payoffs[receivers.size()] = payoff;
				receivers.push_back(*fofIt);

				if(receivers.size() == numNeeded){
					vector<double>::iterator minElt = min_element(payoffs.begin(),payoffs.end());
					minPayoff = *minElt;
					minPayoffIndex = (minElt - payoffs.begin());
				}
			}
			else if(payoff > minPayoff){
				receivers[minPayoffIndex] = *fofIt;
				payoffs[minPayoffIndex] = payoff;
				vector<double>::iterator minElt = min_element(payoffs.begin(),payoffs.end());
				minPayoff = *minElt;
				minPayoffIndex = (minElt - payoffs.begin());
			} 
		}
	}
}

struct IdComp{
	IdComp(){}	
	IdComp(int agID):id(agID){}

	bool operator() (Agent* ag) const{
		return id == ag->id;
	}

	int id;
};


void
Agent::updateNeighbors(double payoff, vector<Agent*> agents){
	for(int i = 0; i < agents.size(); i++){
		//then we are interacting with someone who got deleted... don't update
		updateNeighbor(payoff,agents[i],false);
	}

	//rememberedPayoffs.push_back(make_pair(agents.size(),payoff));
	//payoff /= agents.size();
	////cout << "Friendliness: " << friendliness <<"\n\t";
	//avgPayoff = 0.;
	//if(rememberedPayoffs.size() > 1){
	//for(int i = 0; i < rememberedPayoffs.size()-1;i++){
	//	//cout << "(" << rememberedPayoffs[i].first << ", " << rememberedPayoffs[i].second << "), ";
	//	avgPayoff+=rememberedPayoffs[i].second;
	//}
	//avgPayoff /= rememberedPayoffs.size()-1;
	//}
	////cout << "\n\t AVG: " << avgPayoff<<endl;

}
void 
Agent::updateNeighbor(double payoff, Agent*friendAgent, bool singleUpdate){

	bool newNeighbor = false;
	
	double oldPayoff = 0;

	hash_map<int, Relationship*>::iterator relationshipIt = relationships.find(friendAgent->id);
	if(relationshipIt == relationships.end()){
		expPayoffFromStrangers = (expPayoffFromStrangers * neighbors.size()+ payoff) / (1 + neighbors.size());
		newNeighbor = true;
		Relationship* newRel= new Relationship(friendAgent, payoff);
		relationships[friendAgent->id] = newRel;
	} else{
		Relationship* oldRel = relationshipIt->second;
		oldPayoff = oldRel->lastPayoff;
		oldRel->numInteractions = oldRel->numInteractions+ 1;
		oldRel->numSends = oldRel->numSends+1;
	}

	capitalInWaiting += payoff;

	//if(singleUpdate){
	//	rememberedPayoffs.push_back(make_pair(1,payoff));
	//	//cout << "Friendliness: " << friendliness <<"\n\t";

	//	avgPayoff = 0;
	//	if(rememberedPayoffs.size() > 1){
	//		for(int i = 0; i < rememberedPayoffs.size()-1;i++){
	//			//cout << "(" << rememberedPayoffs[i].first << ", " << rememberedPayoffs[i].second << "), ";		
	//			avgPayoff+=rememberedPayoffs[i].second;
	//		}
	//		avgPayoff /= rememberedPayoffs.size()-1;
	//	}
	//	//cout << "\n\t AVG: " << avgPayoff<<endl;
	//}

	if(oldPayoff == payoff && !newNeighbor){
		return;
	}

	neighbors.remove_if(IdComp(friendAgent->id));

	//Reinsert into neighborset each time to update payoffs
	AgentFriendComp friendComp(this);
	//find - know sorted, so do binary search
	NeighborSet::iterator it = lower_bound(neighbors.begin(),neighbors.end(), friendAgent,friendComp); 

	//insert
	neighbors.insert(it, friendAgent);
}

void Agent::reset(int minute, int newID){
	//All subscribed agents get rid of me

	for(set<Agent*>::iterator it = subscribers.begin(); it != subscribers.end();it++){
		(*it)->deleteSubscribedAgent(this);
	}
	
	capital = Globals::initialAgentCapital;
	capitalInWaiting=0;
	numP=numE=numER=numPR=numReply=totPhonePay=totSMSPay=avgPayoff=0;
	setRandomVars();

	while(!interactionPQ.empty()){
		interactionPQ.pop();
	}
	relationships.clear();
	neighbors.clear();
	subscribers.clear();
	idsConversedWith.clear();
	currInteraction=0;

	id=newID;

}

void 
Agent::deleteSubscribedAgent(Agent* deletedAgent){
	neighbors.remove(deletedAgent);
	relationships.erase(deletedAgent->id);
	subscribers.erase(deletedAgent);
	idsConversedWith.erase(deletedAgent->id);
	if(currInteraction && currInteraction->involves(deletedAgent)){
		currInteraction = 0;
	}

}


double
Agent::getExpectedPayoff(Agent* agent){
	hash_map<int, Relationship*>::iterator it = relationships.find(agent->id);
	double pay = (it == relationships.end()) ? expPayoffFromStrangers : it->second->lastPayoff;
	return pay;
}


double
Agent::getExpectedPayoff(const vector<Agent*>& agents){
	double expPayoffFromReceivers = 0.;
	for(vector<Agent*>::const_iterator it = agents.begin(); it < agents.end(); it++){
		expPayoffFromReceivers += getExpectedPayoff(*it);
	}
	expPayoffFromReceivers /= agents.size();
	return expPayoffFromReceivers;
}