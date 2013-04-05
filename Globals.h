
#pragma once

struct Globals {
	static const int MINUTES_IN_DAY = 60*24;

	static int NUM_AGENTS;//parameter
	static int DAYS; //parameter
	
	static double StartupCost_phone;//parameter
	static double Cost_Per_Min_phone; //parameter
	static double Cost_Per_Additional_Person_phone;

	static double StartupCost_sms;//parameter
	static double Cost_Per_Min_sms; //parameter
	static double Cost_Per_Additional_Person_sms;
	static double MIN_INTERACTION_COST;

	static int SMS_MAX_RECP; //parameter
	static int PHONE_CALL_MAX_RECP; //parameter

	static double initialAgentCapital;
	static double alphaPhone;
	static double alphaSMS;//parameter

	static double choiceExponent;

};
