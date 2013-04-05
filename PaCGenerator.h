#pragma once

#include <string>

class InteractionFactory;

struct PaCGenerator
{
public:
	void run();
	PaCGenerator(std::string allOutN, std::string smsOutN, std::string phoneOutN, std::string idsFileN):
		allOutName(allOutN), smsOutName(smsOutN),phoneOutName(phoneOutN),idsFileName(idsFileN){}

	~PaCGenerator();
private: 
	InteractionFactory* interactionFactory;

	std::string allOutName;
	std::string smsOutName;
	std::string phoneOutName;
	std::string idsFileName;
};

