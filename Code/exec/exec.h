#ifndef EXEC_H
#define EXEC_H
#include "string"
#include <iostream>
using namespace std;

class exec
{
public:
    exec();
    string path="";
    void make_exec(exec *file);
};

#endif