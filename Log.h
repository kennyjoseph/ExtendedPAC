#pragma once
#include "Globals.h"
#include <string>
#include <fstream>
#include <memory>
#include <vector>
struct Agent;
class Interaction;

class Log {
  public:

	static void initializeLog(std::string allOutFN, std::string smsFN, std::string phoneFN);

    static Log* getInstance();
	~Log(){
		allOut.close();
		smsOut.close();
		phoneCallOut.close();
	}

	void printInteraction(std::tr1::shared_ptr<Interaction> interaction);

	void printMessage(const std::string& message);

	void printMessageWithAgent(const std::string& message, Agent* agent);

	void printAgentAttributes(std::vector<Agent>& agents, int day);

	void printMessageWithValue(const std::string& message, int val);
	void printMessageWithValue(const std::string& message, double val);

	int numInteractions;

protected:
	Log(std::string allOutFileName, 
		std::string smsOutFileName, 
		std::string phoneOutFileName,
		std::string attributesFileName);
private:
	static Log* pInstance;
	std::ofstream allOut;
	std::ofstream smsOut;
	std::ofstream phoneCallOut;
	std::ofstream agentAttributesOut;

};
