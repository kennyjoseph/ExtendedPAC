#include <vector>
#include <cmath>
#include<algorithm>
#include<fstream>
#include<map>
#include<iostream>
#include <random>
#include "PaCGenerator.h"
#include "Agent.h"
#include "Globals.h"
#include "AgentVector.h"
#include "Log.h"
#include "InteractionFactory.h"

using namespace std;

PaCGenerator::~PaCGenerator(){
	delete interactionFactory;
}

ptrdiff_t myrandom (ptrdiff_t i) { return rand()%i;}
void PaCGenerator::run(){

	Globals::MIN_INTERACTION_COST = std::min(Globals::StartupCost_sms,Globals::StartupCost_phone);
	Globals::initialAgentCapital = std::max(Globals::StartupCost_sms+Globals::Cost_Per_Min_sms,
		Globals::StartupCost_phone+Globals::Cost_Per_Min_phone);

	//Singleton to store all the agents
	AgentVector* agentVector = AgentVector::getInstance();
	agentVector->agents.clear();

	interactionFactory = new InteractionFactory();

	vector<int> randomized;
	int id;
	
	std::tr1::ranlux64_base_01 eng; 

	std::tr1::normal_distribution<double> norm(2.28,1.96);  
    eng.seed(100);

	//initialization - create all the agents 
	 ofstream f("blah.txt");
	 f << "label" << endl;
	for( id = 0; id < Globals::NUM_AGENTS; id++){
		agentVector->agents.push_back(Agent(id));
		double rv = norm(eng);
		agentVector->agents[id].phonePreference = rv;
		f<< agentVector->agents[id].phonePreference << endl;
		randomized.push_back(id);
	}
	f.close();

	int day = 0;
	 
	for(int min = 1; min < Globals::DAYS*Globals::MINUTES_IN_DAY;min++){
		random_shuffle(randomized.begin(), randomized.end());
		//cout << min << endl;
		
		for(vector<int>::iterator it = randomized.begin(); it != randomized.end();it++){
			agentVector->agents[*it].spendCapital(min,interactionFactory);
		}

		if(!(min % Globals::MINUTES_IN_DAY) ){
			day++;
			int dayOfWeek = day % 7;
	
			vector<Agent*> toReset;
			for(vector<Agent>::iterator it = agentVector->agents.begin(); it != agentVector->agents.end();it++){
				if((double)rand()/RAND_MAX < it->probOfReset){
					toReset.push_back(&(*it));
				}
			}
			for(vector<Agent*>::iterator it = toReset.begin(); it!=toReset.end();it++){
				(*it)->reset(min,id++);
			}
			cout << "DAY: " << day << endl;
			cout << "Number of Interactions logged: " << Log::getInstance()->numInteractions << endl;
			
		}

	}

	cout << "DAY: " << ++day << endl;
	cout << "Number of Interactions logged: " << Log::getInstance()->numInteractions << endl;
			
	ofstream agent_ids(idsFileName);
	for(int i = 0; i < agentVector->agents.size();i++){
		agent_ids << agentVector->agents[i].id << endl;
	}
	agent_ids.close();
	
}

	