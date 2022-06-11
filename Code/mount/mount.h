#ifndef MOUNT_H
#define MOUNT_H

#include "string"
#include <iostream>
#include "listamount.h"
using namespace std;


class mount
{
public:
    mount();
    string path="";
    string name="";
    void make_mount(mount *montar);
};

#endif