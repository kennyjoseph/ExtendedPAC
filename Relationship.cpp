#include "Relationship.h"

bool 
Relationship::operator < (const Relationship& n2p) const{
			return (lastPayoff==n2p.lastPayoff) ? agent < n2p.agent : lastPayoff < n2p.lastPayoff;
		}