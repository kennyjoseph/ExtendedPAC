This work is licensed under the Creative Commons Attribution 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/.

This code is used to simulate interactions between agents according to the model
described in the article below

Joseph, K. & Wei, W. & Carley, K.M. (2013). 
An Agent-Based Model for Simultaneous Phone and SMS Traffic Over Time. 
Proceedings of the 2013 International Conference on Social Computing, 
Behavioral-Cultural Modeling, & Prediction (SBP 2013). Washington, DC, USA. April, 2013

If this model is used, we would appreciate a reference to this article.

The main function of the code runs an instance of the PaCGenerator, which generates 
output using the Log class

The PaCGenerator is the top level for the generator, and it runs for the amount of time
specified in Globals.h.  In general, Globals.h and Globals.cpp hold global variables that,
for convenience, are treated globally - the code can and should probably be refactored once
these are straightened out.

Agents are stored, for convenience, in a singleton object called AgentVector, and can therefore
be accessed anywhere in the code. The other singleton in the code is the Log.

Agents only are aware of interactions, and consequently their interface is decoupled from different
types of interactions, i.e. the SMS and PhoneCall classes.  To add a new interaction type, one must
1) subclass Interaction and implement the resulting methods
2) add code into InteractionFactory.cpp to ensure the interaction is used
3) add to Globals.h/Globals.cpp the initial cost and cost per minute of the interaction

Future work:
-Automake or CMake
-Actually make it a command-line tool
-Provide the analysis code (currently, it contains code from Nan Du that
has not been explicitly released)

