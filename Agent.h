#pragma once

#include<hash_map>
#include "Interaction.h"
#include<vector>
#include <queue>
#include <list>
#include <memory>
#include <set>

using namespace std;

class Relationship;
class AgentVector;
class InteractionFactory;

struct Agent
{

	Agent(int identity);
	Agent(){}
	
	void spendCapital(int minute,InteractionFactory* interactionFactory);
	void reset(int minute,int newID);
	void deleteSubscribedAgent(Agent* deletedAgent);
	void updateNeighbors(double payoff, vector<Agent*> agents);
	void updateNeighbor(double payoff, Agent* friendAgent, bool singleUpdate=true);
	int findPartner(int numNeeded, std::vector<Agent*>& receivers);
	void getRecommendations(int numNeeded, std::vector<Agent*>& receivers);
	double getExpectedPayoff(Agent* agent);
	double getExpectedPayoff(const vector<Agent*>& agents);
	inline void setRandomVars();

	int id;//user id
	double capital;//user capital
	double capitalInWaiting;
	set<int> idsConversedWith;

	double friendliness;
	double probOfReset;

	double expPayoffFromStrangers;

	double phonePreference;
	double avgPayoff;
	int asksForRec; 
	int numE;
	int numP;
	int numER;
	int numPR;
	int numReply;
	int numInit;
	double totPhonePay;
	double totSMSPay;
	double lastPhonePayoff;
	double lastSMSPayoff;
	double totOut;
	double totIn;
	int timesRec;

	std::set<Agent*> subscribers;

	std::tr1::shared_ptr<Interaction> currInteraction;
	hash_map<int, Relationship*> relationships;
	hash_map<int, int> counterMap;
	typedef list<Agent*> NeighborSet;
	NeighborSet neighbors;
	class InteractionComparison
	{
	public:
	  bool operator() (std::tr1::shared_ptr<Interaction> lhs,std::tr1::shared_ptr<Interaction> rhs) const{
		  return lhs->importance < rhs->importance;
	  }
	};
	typedef priority_queue<std::tr1::shared_ptr<Interaction>, vector<std::tr1::shared_ptr<Interaction> >,InteractionComparison> InteractionPQ;
	InteractionPQ interactionPQ;

	AgentVector* agentVector;

	vector< pair<int,double> > rememberedPayoffs;

};

