#pragma once
#include <vector>
#include "Agent.h"

class AgentVector {
  public:

    static AgentVector* getInstance();

	Agent* getRandomAgent(Agent* thisAgent);

	std::vector<Agent> agents;
  protected:
	  AgentVector(){} // constructor
  private:
    static AgentVector* pInstance;
};
