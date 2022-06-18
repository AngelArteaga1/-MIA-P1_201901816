#include "mkdir.h"
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
#include <fstream>
#include <iostream>
#include "../analizador/analizador.h"
#include "../mkfile/mkfile.h"

mkdir::mkdir(){ }

void make_dir(int start, string pathDisk, string path, bool p, char fit){
    string pathCopia1 = path;
    string pathCopia2 = path;
    string pathCopia3 = path;

    //Primero obtenemos el superbloque
    SuperBloque bloquesito;
    FILE *archivo = fopen(pathDisk.c_str(), "rb+");
    fseek(archivo, start, SEEK_SET);
    fread(&bloquesito, sizeof(SuperBloque), 1, archivo);

    //Necesitamos partir el path entre carpetas, esto para ver si existe la carpeta
    char * ruta = &path[0];
    char * token = strtok(ruta, "/");

    //Conseguimos el numero de tokens disponibles
    int count_tokens = get_count_tokens(token, 0);

    //Verificamos si el 
    ruta = &pathCopia1[0];
    token = strtok(ruta, "/");
    int posicion_inodo_carpeta = search_path(token, bloquesito.s_inode_start, pathDisk, 1, count_tokens);


    if(posicion_inodo_carpeta == -1 && !p){
        //No existe el path ni tampoco debemos de crearlo
        cout << "[Error] > La ruta donde se quiere escribir no existe" << endl; 
        fclose(archivo);
        return;
    } else {
        //No existe el path pero si lo podemos crear
        ruta = &pathCopia2[0];
        token = strtok(ruta, "/");
        int start_inodo = make_path(token, bloquesito.s_inode_start, pathDisk, bloquesito, fit, 0, count_tokens);
        if(start_inodo != -1){
            //Mensaje de confirmacion
            cout << "[Success] > La carpeta ha sido creada exitosamente" << endl;
        }
    }

    //Cerramos el file
    fclose(archivo);
}

void mkdir::make_mkdir(mkdir *mkdirsito){
    //Validaciones
    //Parametros obligatorios
    if(mkdirsito->id == "") { cout << "[Error] > No se ingreso el parametro de $id" << endl; return;}
    if(mkdirsito->path == "") { cout << "[Error] > No se ingreso el parametro de $path" << endl; return;}

    //Validamos si existe la particion montada
    if(find_partition_in_mount(mkdirsito->id)){
        //Quiere decir que si existe la particion montada
        //Ahora obtenemos el nodo de la lista con la particion montada
        nodoMount* nodo = get_partition_in_mount(mkdirsito->id);

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
        char fit = get_partition_fit(realPath, nodo->nombre, master);

        //Si devuelve -1 quiere decir que no encontro la particion
        if(start != -1) make_dir(start, realPath, mkdirsito->path, mkdirsito->p, fit);
        else cout << "[ERROR] > No existe la particion con ese nombre" << endl;
        fclose(archivo);

        //Realizamos una copia del disco
        if(!usaraRaid){
            string path_copy = get_path_raid(realPath);
            string cmd = "sudo cp \"" + realPath + "\" \"" + path_copy + "\"";
            system(cmd.c_str());
        }
    } else {
        cout << "[ERROR] > No existe una particion montada con ese id" << endl;
    }
}