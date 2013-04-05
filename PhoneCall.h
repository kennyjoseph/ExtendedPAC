#pragma once

#include "Interaction.h"
#include <vector>
#include <memory>

class PhoneCall : public Interaction
{
public:
	PhoneCall(Agent*send, std::vector<Agent*> receive, int min, double reason, double bounces);

	virtual void send(bool isReply);
	virtual void completeInteraction();
	virtual void interrupt();

	//Because we only talk to one agent, we just hold on to him here for
	//convenience
	Agent* receiver;
};

