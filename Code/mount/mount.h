#ifndef MOUNT_H
#define MOUNT_H

#include "string"
#include <iostream>
#include "../listadoble/listamount.h"
#include "../listadoble/nodomount.h"
using namespace std;

//Funcion para encontrar una posible particion en la lista de montadas
bool find_partition_in_mount(string name, string path);
//Busca la particion con el mismo id y devuelve el treu en caso de que lo encuentre
bool find_partition_in_mount(string id);
//Busca la particion con el mismo id y devuelve el nombre en caso de que lo encuentre
nodoMount* get_partition_in_mount(string id);
class mount
{
public:
    mount();
    string path="";
    string name="";
    void make_mount(mount *montar);
};

#endif