#include "Globals.h"

int	   Globals::NUM_AGENTS = 5000;//parameter
int	   Globals::DAYS = 10;
double Globals::StartupCost_phone = .1;//parameter
double Globals::StartupCost_sms = .1;//parameter
double Globals::Cost_Per_Additional_Person_phone = .2;
double Globals::Cost_Per_Additional_Person_sms = .2;

double Globals::Cost_Per_Min_phone=.4; //parameter
double Globals::Cost_Per_Min_sms= .4; //parameter

double Globals::MIN_INTERACTION_COST = 1;
double Globals::initialAgentCapital = 1;

int Globals::SMS_MAX_RECP = 1; //parameter
int Globals::PHONE_CALL_MAX_RECP=1; //parameter

double Globals::alphaPhone = .9;//parameter
double Globals::alphaSMS = .6;

double Globals::choiceExponent = .5;
