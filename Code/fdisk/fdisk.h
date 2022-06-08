#ifndef FDISK_H
#define FDISK_H

#include "string"
#include <iostream>
using namespace std;


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