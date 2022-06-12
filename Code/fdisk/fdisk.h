#ifndef FDISK_H
#define FDISK_H

#include "string"
#include <iostream>
#include "../estructura/estructura.h"
using namespace std;

/*Definicion de funciones que se pueden usar en otro ambito*/
bool exist_partition(string path, string name, MBR master);
bool is_partition_primaria(MBR master, string name);
bool is_partition_extendida(MBR master, string name);
int get_partition_primaria(MBR master, string name);
int get_partition_extendida(MBR master);
int get_partition_logica(string name, string path);

class fdisk
{
public:
    fdisk();

    int size=0;
    int agregar=0;
    string path="";
    string ajuste="";
    string unidad="";
    string tipo="";
    string eliminar="";
    string nombre="";
    bool mov = false;

    void make_fdisk(fdisk *disco);
};

#endif // FDISK_H