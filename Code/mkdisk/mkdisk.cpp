#include "mkdisk.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "../estructura/estructura.h"
#include "../analizador/analizador.h"

mkdisk::mkdisk(){}

void set_formato(FILE *archivo, int size){
    char buffer[1000];
    for(int i = 0; i < 1000; i++) buffer[i]='\0';
    for(int i = 0; i < size; i++) fwrite(&buffer,1000,1,archivo);
}

void create_mkdisk(string path, string name, int size, Mbr master){
    //Inicializamos el mbr
    master.m_creacion = time(nullptr);
    master.m_creacion = (int)time(nullptr);
    for (int i = 0; i < 4; i++) {
        master.m_particiones[i].p_estado='0';
        master.m_particiones[i].p_tam = 0;
        master.m_particiones[i].p_type = '0';
        master.m_particiones[i].p_comienzo = -1;
        master.m_particiones[i].p_tipo = '0';
        strcpy(master.m_particiones[i].p_nombre,"");
    }

    //Verificamos si la ruta existe, si no la creamos
    if( access( path.c_str(), F_OK ) == -1 ) {
        string cmd = "sudo mkdir -p \"" + path + "\"";
        system(cmd.c_str());
    }

    //Combinamos el path con el name
    string rutaCompleta = path + name;
    const char * ruta = rutaCompleta.c_str();

    //Creamos un archivo con el tamaño indicado
    FILE *archivo = fopen(ruta, "wb");
    fseek(archivo, 0, SEEK_SET);
    set_formato(archivo, size);
    fclose(archivo);

    //Escribimos el mbr en la posicion 0
    archivo = fopen(ruta, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fwrite(&master, sizeof(Mbr),1, archivo);
    fclose(archivo);

    //Mensaje de success
    cout << "[Success] > Se creo correctamente el disco" << endl;
}


void mkdisk::make_mkdisk(mkdisk *disco){
    //Validaciones
    if(disco->size <= 0) { cout << "[Error] > El tamaño del disco debe de ser mayor o igual a 0" << endl; return;}
    if(disco->size % 8 != 0) { cout << "[Error] > El tamaño del disco no es multiplo de 8" << endl; return;}
    if(!verify_name(disco->name)) { cout << "[Error] > El nombre del disco no es correcto" << endl; return;}
    //Inicializamos el discos
    Mbr master;
    master.m_tam = disco->size*1000;
    //Creamos el disco
    create_mkdisk(disco->path, disco->name, disco->size*1000, master);
}