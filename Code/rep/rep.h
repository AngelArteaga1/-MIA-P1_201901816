#ifndef REP_H
#define REP_H

#include "string"
#include <iostream>
using namespace std;

extern std::ofstream outputfile;

class rep
{
public:
    rep();

    string name = "";
    string path = "";
    string id = "";

    void make_rep(rep *rep);
};

#endif