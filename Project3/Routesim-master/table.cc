#include "table.h"
#include <algorithm>

using namespace std;

#define min(a, b) ((a) < (b) ? (a):(b))

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}

#endif

#if defined(LINKSTATE)

#endif

#if defined(DISTANCEVECTOR)

Table::Table() {}

Table::Table(unsigned n){
	idx = n;
	table_ = map3 ();
	neighbor_ = map2 ();
	neighbor_[idx] = 0;
	table_[idx] = map1 ();
	table_[idx][idx] = Info(idx,0);
}


unsigned Table::GetNextHop(const unsigned dest) const {
    map3::const_iterator it = table_.find(idx);
    map1::const_iterator itr = it->second.find(dest);
    if (itr != it->second.end()) {
        return itr->second.next;
    } else {
        return UNDEF;
    }
}

map<unsigned, Info> Table::getselfDV(){
    return table_[idx];
};

bool Table::update(unsigned n, map1 DV){
    map1 old_ = table_[idx];
    table_[n] = DV;
    Update();
    return isDVdiff(old_);
}

bool Table::update(const Link* link){
    //copy the previous vector
    //map<unsigned Info> map1
    //map<unsigned, double> map2
    //map<unsigned, map<unsigned, Info>>  map3
    map1 old_ = map1 (table_[idx]);
    double len = link -> GetLatency();
	unsigned dest = link -> GetDest();
    //double old_edge = neighbor_.find(dest) == neighbor_.end()? len : neighbor_[dest] ;
    neighbor_[dest] = len;

    if(table_[idx].find(dest) == table_[idx].end()){
        //initial from idx to dest;
        table_[idx][dest] = Info(dest, len);
        if(table_.find(dest)!=table_.end()){
            for(map1::iterator it = table_[dest].begin(); it != table_[dest].end(); it++){
                if(table_[idx].find(it->first) == table_[idx].end()){
                    table_[idx][it->first] = Info(dest, len + it->second.latency);
                }
            }

        }
    }
    Update();
    return isDVdiff(old_);
}

bool Table::isDVdiff(map1 &dv){
    for(map1::iterator iter = table_[idx].begin(); iter != table_[idx].end(); iter++){
        if(dv.find(iter->first)==dv.end() || dv[iter->first].next != iter->second.next
            || dv[iter->first].latency != iter->second.latency){
                return true;
            }
    }
    return false;
}

void Table::Update(){
    //preprocess distant vector:
    for(map1::iterator it = table_[idx].begin(); it != table_[idx].end();){
        map<unsigned, double>::iterator iter = neighbor_.find(it->first);
        it -> second = iter != neighbor_.end()? Info(iter->first, iter->second):Info(UNDEF, DBL_MAX);
        for(map2::iterator it1 = neighbor_.begin(); it1 != neighbor_.end(); it1++){
            if(table_.find(it1->first) != table_.end()
                && table_[it1->first].find(it->first) != table_[it1->first].end()){
                    double temp = neighbor_[it1->second] + table_[it1->first][it->first].latency;
                    if(temp < it->second.latency){
                        it->second.next = temp;
                        it->second.latency = it1->first;
                    }
                }
        }
        if(it->second.next == UNDEF || it->second.latency == DBL_MAX){
            it = table_[idx].erase(it);
        }else{
            it++;
        }
    }
}

ostream &Table::Print(ostream &os)  {
    os << "Routing table for node: " << idx << endl;
    for (map3::const_iterator itr = table_.begin(); itr != table_.end(); itr++) {
        os << "................................................" << endl;
        if(itr->first != idx){
            os<<"neigbor is: " << itr -> first << endl;
            for(map1::const_iterator itr2 = table_[itr->first].begin(); itr2 != table_[itr->first].end(); itr2++){
                os << "from src: " << itr->first << " to " << itr2->first << " ; next hop is: " << itr2->second.next;
                os << " ;latency is" << itr2->second.latency;
            }
        os << "................................................." << endl;
        }else{
            os <<".................................................."<<endl;
            os << "source is: " << itr->first << endl;
            for(map1::const_iterator itr2 = table_[itr->first].begin(); itr2 != table_[itr->first].end(); itr2++){
                os << "from src: " << itr->first << " to " << itr2->first << " ; next hop is: " << itr2->second.next;
                os << " ;latency is" << itr2->second.latency;
            }
            os <<"..................................................."<<endl;
        }
    }
    return os;
}
#endif
