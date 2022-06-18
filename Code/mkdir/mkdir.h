#ifndef MKDIR_H
#define MKDIR_H

#include "string"
#include <iostream>
using namespace std;

class mkdir
{
public:
    mkdir();

    string id="";
    string path="";
    bool p = false;

    void make_mkdir(mkdir *mkdirsito);

};

#endif