#include "syncronice.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include "../estructura/estructura.h"
#include "../listadoble/nodomount.h"
#include "../estructura/estructura.h"
#include "../mount/mount.h"
#include "../analizador/analizador.h"
#include "../fdisk/fdisk.h"
#include "../mkfile/mkfile.h"

syncronice::syncronice(){ }

std::ofstream outputjson;

string get_dir_name(int start_first_block, string path){
    //Obtenemos el bloque
    BloqueCarpeta carpetita;
    FILE *archivo = fopen((path).c_str(), "rb+");
    fseek(archivo, start_first_block, SEEK_SET);
    fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
    fclose(archivo);

    //Devolvemos el primer nombre ya que es el del padre
    string s(carpetita.b_content[0].b_name);
    return s;
}

string get_file_name(int start_first_block, string path){
    //Obtenemos el bloque
    BloqueArchivo archivito;
    FILE *archivo = fopen((path).c_str(), "rb+");
    fseek(archivo, start_first_block, SEEK_SET);
    fread(&archivito, sizeof(BloqueArchivo), 1, archivo);
    fclose(archivo);

    //Devolvemos el primer nombre ya que es el del padre
    string s(archivito.b_name);
    return s;
}

void convert_inode(int start_inodo, string path){
    //Obtenemos el inodo
    TablaInodo inodo;
    FILE *archivo = fopen((path).c_str(), "rb+");
    fseek(archivo, start_inodo, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);
    fclose(archivo);
    
    outputjson << "{" << endl;
    //Verificamos si es un inodo de carpeta o de archivo
    if(inodo.i_type == '0'){
        //Es un inodo de carpeta
        string nombre_carpeta = get_dir_name(inodo.i_block[0], path);
        char type = inodo.i_type;
        int size = inodo.i_size;


        //Escribimos el objeto
        outputjson << "\"name\": \"" << nombre_carpeta << "\"," << endl; 
        outputjson << "\"type\": \"" << type << "\"," << endl; 
        outputjson << "\"size\": \"" << size << "\"," << endl; 
        outputjson << "\"sons\": [" << endl; 

        //Ahora los hijos
        for(int i = 0; i < 15; i++){
            if(i < 12){
                if(inodo.i_block[i] != -1){
                    //Escribimos los hijos
                    //Primero obtenemos el bloque 
                    BloqueCarpeta carpetita;
                    archivo = fopen((path).c_str(), "rb+");
                    fseek(archivo, inodo.i_block[i], SEEK_SET);
                    fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
                    fclose(archivo);
                    //Revisamos si existe siguiente apuntador
                    bool existeSiguiente = false;
                    if(i < 14)
                        if(inodo.i_block[i+1] != -1) existeSiguiente = true;
                    for(int j = 1; j < 4; j++){
                        if(carpetita.b_content[j].b_inodo != -1){
                            //if(j != 1) outputjson << "," << endl;
                            convert_inode(carpetita.b_content[j].b_inodo, path);
                            if(existeSiguiente){
                                outputjson << "," << endl;
                            } else if(j < 3){
                                if(carpetita.b_content[j+1].b_inodo != -1){
                                    outputjson << "," << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        outputjson << "]," << endl;
        outputjson << "\"content\": \"\"" << endl; 
    } else {
        //Es un inodo de archivo

        string nombre_archivo = get_file_name(inodo.i_block[0], path);
        char type = inodo.i_type;
        int size = inodo.i_size;

        outputjson << "\"name\": \"" << nombre_archivo << "\"," << endl; 
        outputjson << "\"type\": \"" << type << "\"," << endl; 
        outputjson << "\"size\": \"" << size << "\"," << endl; 
        outputjson << "\"sons\": []," << endl; 
        outputjson << "\"content\": \""; 
        //Ahora el content
        for(int i = 0; i < 15; i++){
            if(i < 12){
                if(inodo.i_block[i] != -1){
                    //Escribimos los hijos
                    //Primero obtenemos el bloque 
                    BloqueArchivo archivito;
                    archivo = fopen((path).c_str(), "rb+");
                    fseek(archivo, inodo.i_block[i], SEEK_SET);
                    fread(&archivito, sizeof(BloqueArchivo), 1, archivo);
                    fclose(archivo);
                    //Ahora solo escribimos la informacion
                    outputjson << archivito.b_content;
                }
            }
        }
        outputjson << "\"" << endl; 
    }
    outputjson << "}";
}


void make_json(string pathJson, string path, string name){
    //Obtenemos el el mbr
    MBR master;
    FILE *archivo = fopen((path).c_str(), "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);


    //Obtenemos el superbloque
    SuperBloque bloquesito;
    int part_start = get_partition_start(path, name, master);
    fseek(archivo, part_start, SEEK_SET);
    fread(&bloquesito, sizeof(SuperBloque), 1, archivo);
    fclose(archivo);

    //Abrimos el archivo donde escribimos el reporte
    pathJson += "tree.json";
    outputjson.open(pathJson, ios::out);
    if(outputjson.fail()){
        cout<<"[Error] > No se pudo crear o abrir el archivo json";
        return;
    }

    //Ahora tenemos que iterar todos los inodos desde el principio jajaja, que dios me ayude
    convert_inode(bloquesito.s_inode_start, path);
    outputjson.close();


    if(archivo= fopen(pathJson.c_str(), "rb+")){
        cout << "[Success] > Se creo el archivo json correctamente" << endl;
        fclose(archivo);
    }else{
        cout << "[Error] > No fue posible crear el archivo json" << endl;
    }
}




void syncronice::make_syncronice(syncronice *sincronito){
    //Validaciones
    if(sincronito->path == "") { cout << "[Error] > No se ingreso el parametro @path" << endl; return;}
    if(sincronito->id == "") { cout << "[Error] > No se ingreso el parametro @id" << endl; return;}

    //Verificamos si existe la particion
    if(find_partition_in_mount(sincronito->id)){
        //Obtenemos el nodo donde esta la particion
        nodoMount* nodo = get_partition_in_mount(sincronito->id);
        //Verificamos si existe el path
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

        //Verificamos si la ruta existe, si no creamos las carpetas
        string path_without_name = get_path_without_name(sincronito->path);
        if( access( path_without_name.c_str(), F_OK ) == -1 ) {
            string cmd = "mkdir -p \"" + path_without_name + "\"";
            system(cmd.c_str());
        }

        //AHORA TENEMOS QUE ESCRIBIR EL ARCHIVO JSON
        make_json(sincronito->path, nodo->path, nodo->nombre);
    }

}