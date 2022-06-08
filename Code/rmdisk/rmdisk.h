#ifndef RMDISK_H
#define RMDISK_H

#include "string"
#include <iostream>
using namespace std;

class rmdisk
{
public:
    rmdisk();
    string path="";
    void make_rmdisk(rmdisk *disco);
};

#endif // RMDISK_H