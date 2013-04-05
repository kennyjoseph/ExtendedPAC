#pragma once

#include "Interaction.h"
#include <vector>
#include <memory>

class SMS : public Interaction
{
public:
	SMS(Agent*send, std::vector<Agent*> receive, int min, double reason, double bounces);

	virtual void send(bool isReply);
	virtual void completeInteraction();
	virtual void interrupt();

};

