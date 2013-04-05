#pragma once

#include <vector>
#include <memory>
#include <set>
struct Agent;

class Interaction : public std::tr1::enable_shared_from_this<Interaction>
{
public:
	Interaction(Agent*send, std::vector<Agent*> receive, 
				int min,
				double richness,
				double cost_per_addl,
				double cost_per_min, 
				double reason,
				double bounces,
				double alpha);

	void update(int minute);

	virtual void send(bool isReply);

	double getPayoff(bool isRecv){
		return currPayoff;
	}
	double getBenefit(double sendF, double recvF){
		return std::pow(sendF*recvF,.5);
	}
	bool shouldInterrupt(std::tr1::shared_ptr<Interaction> receiverCurr){
		return false;
	}
	bool hadAgentDeleted();

	bool involves(Agent* agent);

	int startMinute;
	Agent* sender;
	std::vector<Agent*> receivers;
	
	std::set<int> originalIDs;

	double importance;
	int callLength;
	double benefit;

	double lastPayoff;
	double currPayoff;

	double richness;
	double cost_pm;
	double cost_per_additional_agent;

	double bounces;
	double alpha;

	virtual void interrupt(){}

	//Housekeeping vars;
	int interruptedCount;

protected:
	virtual void completeInteraction() = 0;
	double interruptProbability;
	
};

