#pragma once

#include <memory>
#include <map>
#include <vector>
class Interaction;
struct Agent;
class MTRand;


class InteractionFactory
{
public:
	InteractionFactory();
	~InteractionFactory();

	
	enum InteractionType{SMS_ENUM,PHONE_ENUM, LAST};
	std::tr1::shared_ptr<Interaction> getInteraction(Agent* sender, int minute,
		std::tr1::shared_ptr<Interaction> interactionToReplyTo = 0);

};

