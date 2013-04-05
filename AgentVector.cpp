#include "AgentVector.h"
#include "Globals.h"
#include "Log.h"
#include <iostream>
#include <cstdlib>
using namespace std;

AgentVector* AgentVector::pInstance = 0;

AgentVector* AgentVector::getInstance(){
  if(!pInstance){
    pInstance = new AgentVector();
  }
  return pInstance;
}


Agent*
AgentVector::getRandomAgent(Agent* thisAgent){
	//not the same, not a neighbor
	unsigned int count = 0;
	Agent * j = 0;
	while(j== 0 || (j->id== thisAgent->id) ||  (thisAgent->relationships.find(j->id) != thisAgent->relationships.end()))
    {
		j =  &agents[rand() % Globals::NUM_AGENTS];
		if ((++ count) == Globals::NUM_AGENTS)
       {
		  return 0;
       }
    }
    return j;
}


