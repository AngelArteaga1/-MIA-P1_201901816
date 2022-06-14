#ifndef MKFILE_H
#define MKFILE_H

#include "string"
#include <iostream>
using namespace std;

class mkfile
{
public:
    mkfile();

    string id="";
    string path="";
    bool p = false;
    int size = 0;
    string cont="";

    void make_mkfile(mkfile *Archivito);

};

#endif