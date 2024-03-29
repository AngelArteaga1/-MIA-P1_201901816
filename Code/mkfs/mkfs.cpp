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
#include "../analizador/analizador.h"

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
    bloquesito.s_first_ino = 2;
    bloquesito.s_first_blo = 2;

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
    char full = '1';
    fseek(file, bloquesito.s_bm_inode_start, SEEK_SET);
    for (int i = 0; i < bloquesito.s_inodes_count; i++){
        if(i == 0) fwrite(&full, sizeof(char), 1, file); //Esto es por el inodo root
        fwrite(&empty, sizeof(char), 1, file);
    }

    //Ahora escribimos el bitmap de bloques
    fseek(file, bloquesito.s_bm_block_start, SEEK_SET);
    for (int i = 0; i < bloquesito.s_block_count; i++){
        if(i == 0) fwrite(&full, sizeof(char), 1, file); //Esto es por el bloque root
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
    //Ingresamos la posicion del bloque de la carpeta raiz
    inode.i_block[0] = bloquesito.s_block_start;

    //Ahora tenemos que escribir el nuevo bloque dentro de la particion
    fseek(file, bloquesito.s_block_start, SEEK_SET);
    fwrite(&carpetita, sizeof(BloqueCarpeta), 1, file);

    //Luego tenemos que escribir el inodo dentro de la particion
    fseek(file, bloquesito.s_inode_start, SEEK_SET);
    fwrite(&inode, sizeof(TablaInodo), 1, file);

    //Mostramos mensaje de success
    cout << "[Success] > Se le dio formato a la particion" << endl;

    //Cerramos el file
    fclose(file);
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
    else{ cout << "[Error] > El parametro type es incorrecto" << endl; return; }

    //Validamos si existe la particion montada
    if(find_partition_in_mount(mkfsito->id)){
        //Quiere decir que si existe la particion montada
        //Ahora obtenemos el nodo de la lista con la particion montada
        nodoMount* nodo = get_partition_in_mount(mkfsito->id);
        
        FILE *file;
        bool usaraRaid = false;
        string copyPath = get_path_raid(nodo->path);
        string realPath = nodo->path;
        if (!(file = fopen(realPath.c_str(), "r"))) { 
            realPath = copyPath;
            usaraRaid = true;
        } else fclose(file);
        if(usaraRaid){
            if(!(file = fopen(realPath.c_str(), "r"))) {
                cout << "[Error] > No se ha encontrado el disco" << endl; 
                return;
            } else fclose(file);
        }

        FILE *archivo = fopen((realPath).c_str(), "rb+");
        //Obtenemos el mbr
        MBR master;
        fseek(archivo, 0 , SEEK_SET);
        fread(&master, sizeof(MBR), 1, archivo);
        //Obtenemos la posicion de la particion
        int start = get_partition_start(realPath, nodo->nombre, master);
        //Obtenemos el size de la particion
        int size = get_partition_size(realPath, nodo->nombre, master);

        //Si devuelve -1 quiere decir que no encontro la particion
        if(start != -1) make_ext(start, size, realPath, mkfsito->type);
        else cout << "[Error] > No existe la particion con ese nombre" << endl;
        fclose(archivo);

        //Realizamos una copia del disco
        if(!usaraRaid){
            string path_copy = get_path_raid(realPath);
            string cmd = "sudo cp \"" + realPath + "\" \"" + path_copy + "\"";
            system(cmd.c_str());
        }
    } else {
        cout << "[Error] > No existe una particion montada con ese id" << endl;
    }
}