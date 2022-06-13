#include "mkfs.h"
#include "../fdisk/fdisk.h"
#include "../listadoble/nodomount.h"
#include "../estructura/estructura.h"
#include <ctime>
#include <algorithm>
#include <string>
#include "../mount/mount.h"
#include <cmath>
#include <stdio.h>
#include <string.h>


mkfs::mkfs(){ }

void make_ext(int start, int size, string path, string type){
    //Primero inicializamos las estructuras
    //Tabla de inodo y el Superbloque 
    TablaInodo inode;
    BloqueCarpeta block;

    //Obtenemos n de la formula
    double n = (size - int(sizeof(SuperBloque))) / (int(sizeof(Journaling)) + 4 + int(sizeof(TablaInodo)) + (int(sizeof(BloqueArchivo)) * 3));
    int numero_estructuras = int(floor(n));

    //Seteamos el numero de bloques y de estructuras
    SuperBloque bloquesito;
    bloquesito.s_filesystem_type = (int)time(nullptr);
    bloquesito.s_inodes_count = bloquesito.s_free_inodes_count = numero_estructuras;
    bloquesito.s_block_count = bloquesito.s_free_blocks_count = numero_estructuras * 3;
    bloquesito.s_mtime = time(nullptr);
    bloquesito.s_umtime = time(nullptr);
    bloquesito.s_mnt_count = 0;
    bloquesito.s_magic = 0xEF53;
    bloquesito.s_inode_size = sizeof(inode);
    bloquesito.s_block_size = sizeof(block);
    bloquesito.s_bm_inode_start = start + sizeof(SuperBloque) + (numero_estructuras * sizeof(Journaling));
    bloquesito.s_bm_block_start = bloquesito.s_bm_inode_start + numero_estructuras;
    bloquesito.s_inode_start = bloquesito.s_bm_block_start + (3 * numero_estructuras);
    bloquesito.s_block_start = bloquesito.s_bm_inode_start + (numero_estructuras * sizeof(TablaInodo));
    bloquesito.s_first_ino = 1;
    bloquesito.s_first_blo = 1;

    //Escribimos el superbloque en la posicion inicial
    FILE *file = fopen(path.c_str(),"rb+");
    fseek(file, start, SEEK_SET);
    fwrite(&bloquesito, sizeof(SuperBloque), 1, file);

    //Escribimos todos los journaling posibles
    Journaling journalingsito;
    for (int i = 0; i < numero_estructuras; i++) {
        fwrite(&journalingsito, sizeof(Journaling), 1, file);
    }

    //Ahora escribimos el bitmap de inodos
    char empty = '0';
    fseek(file, bloquesito.s_bm_inode_start, SEEK_SET);
    for (int i = 0; i < bloquesito.s_inodes_count; i++){
        fwrite(&empty, sizeof(char), 1, file);
    }

    //Ahora escribimos el bitmap de bloques
    fseek(file, bloquesito.s_bm_block_start, SEEK_SET);
    for (int i = 0; i < bloquesito.s_block_count; i++){
        fwrite(&empty, sizeof(char), 1, file);
    }

    //Ahora escribimos los inodos
    TablaInodo inodo;
    fseek(file, bloquesito.s_inode_start, SEEK_SET);
    for (int i = 0; i < bloquesito.s_inodes_count; i++) {
        fwrite(&inode, sizeof(TablaInodo), 1, file);
    }

    //Ahora escribimos los bloques
    BloqueArchivo bloque;
    for (int i = 0; i < bloquesito.s_block_count; i++) {
        fwrite(&inode, sizeof(TablaInodo), 1, file);
    }

    //Ahora creamos nuestro bloque de carpeta para crear la carpeta root
    BloqueCarpeta carpetita;
    carpetita.b_content[0].b_inodo = -1;
    strcpy(carpetita.b_content[0].b_name, "root");
    carpetita.b_content[1].b_inodo = -1;
    strcpy(carpetita.b_content[1].b_name, "");
    carpetita.b_content[2].b_inodo = -1;
    strcpy(carpetita.b_content[2].b_name, "");
    carpetita.b_content[3].b_inodo = -1;
    strcpy(carpetita.b_content[3].b_name, "");

    //Ahora solo tenemos que ingresar el primer inodo raiz
    inode.i_uid = 1;
    inode.i_gid = 1;
    inode.i_size = 0;
    inode.i_atime = time(nullptr);
    inode.i_ctime = time(0);
    inode.i_mtime = time(0);
    for(int i = 0; i < 15; i++)
        inode.i_block[i] = -1;
    inode.i_type = '0';

    //Ahora tenemos que escribir el nuevo bloque dentro de la particion

    //Luego tenemos que escribir el inodo dentro de la particion

    //Por ultimo actualizamos el superbloque si es que no lo hacemos antes

    //Mostramos mensaje de success
}


void mkfs::make_mkfs(mkfs *mkfsito){
    //Validaciones
    //Parametros obligatorios
    if(mkfsito->id == "") { cout << "[Error] > No se ingreso el parametro de $id" << endl; return;}

    string tipo = "";
    //convertimos los string a toupper para poder comparar
    transform(mkfsito->unit.begin(), mkfsito->unit.end(),mkfsito->unit.begin(), ::toupper);
    transform(mkfsito->type.begin(), mkfsito->type.end(),mkfsito->type.begin(), ::toupper);
    //Revisamos que tipo de tyes es el formato
    if (mkfsito->type=="FAST" || mkfsito->type == "FULL") tipo = mkfsito->type;
    else if (mkfsito->type == "") tipo = "FULL";
    else{ cout << "[ERROR] > El parametro type es incorrecto" << endl; return; }

    //Validamos si existe la particion montada
    if(find_partition_in_mount(mkfsito->id)){
        //Quiere decir que si existe la particion montada
        //Ahora obtenemos el nodo de la lista con la particion montada
        nodoMount* nodo = get_partition_in_mount(mkfsito->id);
        FILE *archivo;
        if(archivo=fopen((nodo->path).c_str(), "rb+")){
            //Obtenemos el mbr
            MBR master;
            fseek(archivo, 0 , SEEK_SET);
            fread(&master, sizeof(MBR), 1, archivo);
            //Obtenemos la posicion de la particion
            int start = get_partition_start(nodo->path, nodo->nombre, master);
            //Obtenemos el size de la particion
            int size = get_partition_size(nodo->path, nodo->nombre, master);

            //Si devuelve -1 quiere decir que no encontro la particion
            if(start != -1) make_ext(start, size, nodo->path, mkfsito->type);
            else cout << "[ERROR] > No existe la particion con ese nombre" << endl;
            fclose(archivo);
        }else{
            cout << "[ERROR] > No existe ese disco" << endl;
        }
    } else {
        cout << "[ERROR] > No existe una particion montada con ese id" << endl;
    }
}