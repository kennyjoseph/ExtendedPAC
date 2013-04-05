#pragma once

struct Agent;
class Relationship {
public:
		
		Relationship():agent(0),lastPayoff(0),numInteractions(0),numSends(1){
		}
		Relationship(Agent* ag, double payoff):agent(ag),lastPayoff(payoff),numInteractions(0),numSends(1){
		}

		double lastPayoff;
		int numInteractions;
		int numSends;
		Agent* agent;

		bool operator < (const Relationship& n2p) const;
};