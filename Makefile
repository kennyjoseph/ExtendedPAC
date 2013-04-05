#default build is to expand DEBUG IFDEF in source
DEBUG ?= 0
CXXFLAGS = 
ifneq ($(DEBUG), 0)
    CXXFLAGS +=-g3 -gdwarf-2 -DDEBUG -Wall -fmessage-length=0
else
    CXXFLAGS +=-DNDEBUG -O2 
endif		


TARGET = Analysis
OBJS =	Agent.o AgentVector.o main.o Analysis.o BasicProperties.o SMS.o \
	Globals.o Interaction.o InteractionFactory.o IO.o Log.o \
	PaCGenerator.o PhoneCall.o Utility.o
LIBS =

#we are using the TR1 extensions of c++, for MacOS X this requires use
#of g++ 4.2 or higher (4.0 does not include TR1)
CXX=g++ 

#$@ == name of target file
$(TARGET):	$(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(LIBS) 
	./Analysis

#pull in dependency info for existing .o files
-include $(OBJS:.o=.d)

all:	$(TARGET)
	./Analysis

# recall $< == Name of first  prerequisite file (the first one after the colon)

#from http://scottmcpeak.com/autodepend/autodepend.html on 28 Nov 11
# compile and generate dependency info;
# more complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) -MM $(CXXFLAGS)  $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp  
	  
clean:
	rm -f $(OBJS) *.d $(TARGET) *~ $(OBJS:.o=.d) 

package: 
	tar -cvzf project_kjoseph_mlanham_wwei.tar.gz *.h *.cpp Makefile