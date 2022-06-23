#ifndef SYNCRONICE_H
#define SYNCRONICE_H

#include "string"
#include <iostream>
using namespace std;

extern std::ofstream outputjson;

class syncronice
{
public:
    syncronice();

    string id = "";
    string path = "";

    void make_syncronice(syncronice *sincronito);
};

#endif