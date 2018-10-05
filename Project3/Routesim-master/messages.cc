#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

#endif


#if defined(DISTANCEVECTOR)

RoutingMessage::RoutingMessage(unsigned num, map<unsigned, Info> vec){
    this -> num = num;
    this -> DV = vec;
}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs){
    num = rhs.num;
    DV = rhs.DV;
}

RoutingMessage &RoutingMessage::operator=(const RoutingMessage &rhs) {
    return *(new(this) RoutingMessage(rhs));
}

ostream &RoutingMessage::Print(ostream &os) const {
    os << "RountingMessage from src: " << num << endl;
    for (map<unsigned, Info>::const_iterator itr = DV.begin(); itr != DV.end(); itr++) {
        os << "\tdest: " << itr->first << ", next: " << itr->second.next
            << ", cost: " << itr->second.latency << endl;
    }
    return os;
}
#endif

