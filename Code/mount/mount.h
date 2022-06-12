#ifndef MOUNT_H
#define MOUNT_H

#include "string"
#include <iostream>
using namespace std;

//Funcion para encontrar una posible particion en la lista de montadas
bool find_partition_in_mount(string name, string path);
class mount
{
public:
    mount();
    string path="";
    string name="";
    void make_mount(mount *montar);
};

#endif