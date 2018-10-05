#ifndef _table
#define _table


#include <iostream>

using namespace std;


#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
class Table {
  // Students should write this class
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
#include "link.h"
#include <map>
#include <deque>
#include "float.h"
#define UNDEF 9999999

struct Info{
    unsigned next;
    double latency;
    Info () {};
    Info (unsigned x, double l) {
        next = x;
        latency = l;
    }
};

typedef map<unsigned, Info> map1;
typedef map<unsigned, double> map2;
typedef map<unsigned, map<unsigned, Info> >  map3;

class Table {
    public:
        ostream & Print(ostream &os);
        Table();
        Table(unsigned n);
        void Update();
        unsigned GetNextHop(const unsigned dest) const;
        bool update(const Link* link);
        bool update(unsigned n, map1 DV);
        bool isDVdiff(map1 &dv);
        map<unsigned, Info> getselfDV();
    private:
        map<unsigned, map<unsigned, Info> > table_;
        unsigned idx;

        //here we define neighbor_ to store the direct link between src and its neighbor
        map<unsigned, double> neighbor_;
};

#endif

inline ostream & operator<<(ostream &os, Table &t) { return t.Print(os);}

#endif
