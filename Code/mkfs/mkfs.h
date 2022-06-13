#ifndef MKFS_H
#define MKFS_H

#include "string"
#include <iostream>
using namespace std;

class mkfs
{
public:
    mkfs();

    string id="";
    string type="";
    int add = 0;
    string unit = "";

    void make_mkfs(mkfs *Disco);

};

#endif