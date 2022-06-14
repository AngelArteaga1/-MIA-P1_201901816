#include "mkfile.h"
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

mkfile::mkfile(){ }

void print_bloque_carpeta(BloqueCarpeta carpetita){
    cout << "************** BLOQUE RECORRIDO ****************" << endl;
    for(int i = 0; i < 4; i++){
        cout << " carpetita.b_name: " << carpetita.b_content[i].b_name << ", carpetita.b_inodo" << carpetita.b_content[i].b_inodo << endl; 
    }
}

/*Esta funcion recibe el inicio del bloque carpeta, revisa si el nombre de la carpeta exite,
si si, entonces regresa true, si no encuentra regresa false*/
bool verify_inode_name(int bloque_carpeta_start, string path, char * token){
    FILE *archivo = fopen(path.c_str(), "rb+");
    BloqueCarpeta carpetita;
    fseek(archivo, bloque_carpeta_start, SEEK_SET);
    fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
    //Imprimos el bloque solo para ver si todo va bien
    print_bloque_carpeta(carpetita);
    //Verificamos si tiene un hijo con el nombre
    for(int j = 1; j < 4; j++){
        cout << "Comparando: " << carpetita.b_content[j].b_name << ", con : " << token << endl;
        //No iteramos el primero porque es el nombre de la carpeta
        if(strcmp(carpetita.b_content[j].b_name, token) == 0){
            //Creo que deberiamos revisar si el inodo es de tipo carpeta jeje
            bool esCarpeta = true;
            if(esCarpeta){
                return carpetita.b_content[j].b_inodo;
            }
        }
    }
    return -1;
}

bool search_path(char * token, int inodo_start, string path){
    //Primero leemos el inodo
    int inodo_next = -1;
    //Primero obtenemos el superbloque
    TablaInodo inodo;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, inodo_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            //Encontramos apuntador a un bloque
            int posicion_bloque = -1;
            bool bloque_encontrado = false;
            if(i == 12){
                //Es un bloque simple indirecto
                //Obtenemos el bloque de aputnadores
                BloqueApuntador apuntadores;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                //Tenemos que iterar el bloque
                for(int j = 0; j < 16; j++){
                    //Verificamos si el puntero tiene contenido
                    if(apuntadores.b_pointers[j] != -1){
                        //Si si tiene contenido, este es el apuntador al bloque de carpeta
                        //Es un un bloque de carpeta
                        int posicion = verify_inode_name(apuntadores.b_pointers[j], path, token);
                        if(posicion != -1){
                            posicion_bloque = posicion;
                            bloque_encontrado = true;
                            break;
                        }
                    }
                }
            } else if(i == 13){
                //Es un bloque doble indirecto
                //Obtenemos el bloque de aputnadores
                BloqueApuntador apuntadores;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                //Tenemos que iterar el bloque
                for(int j = 0; j < 16; j++){
                    //Verificamos si el puntero tiene contenido
                    if(apuntadores.b_pointers[j] != -1){
                        //Si si tiene contenido, este es el apuntador a otro bloque indirecto
                        //Obtenemos el bloque de apuntadores
                        BloqueApuntador apuntadoresDobles;
                        fseek(archivo, apuntadores.b_pointers[j], SEEK_SET);
                        fread(&apuntadoresDobles, sizeof(BloqueApuntador), 1, archivo);
                        //Tenemos que iterar el bloque
                        for(int k = 0; k < 16; k++){
                            //Verificamos si el puntero tiene contenido
                            if(apuntadoresDobles.b_pointers[k] != -1){
                                //Es un un bloque de carpeta
                                int posicion = verify_inode_name(apuntadoresDobles.b_pointers[k], path, token);
                                if(posicion != -1){
                                    posicion_bloque = posicion;
                                    bloque_encontrado = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            } else if(i == 14){
                //Es un bloque triple indirecto
                //Obtenemos el bloque de aputnadores
                BloqueApuntador apuntadores;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                //Tenemos que iterar el bloque
                for(int j = 0; j < 16; j++){
                    //Verificamos si el puntero tiene contenido
                    if(apuntadores.b_pointers[j] != -1){
                        //Si si tiene contenido, este es el apuntador a otro bloque indirecto
                        //Obtenemos el bloque de apuntadores
                        BloqueApuntador apuntadoresDobles;
                        fseek(archivo, apuntadores.b_pointers[j], SEEK_SET);
                        fread(&apuntadoresDobles, sizeof(BloqueApuntador), 1, archivo);
                        //Tenemos que iterar el bloque
                        for(int k = 0; k < 16; k++){
                            //Verificamos si el puntero tiene contenido
                            if(apuntadoresDobles.b_pointers[k] != -1){
                                //Obtenemos el bloque de apuntadores
                                BloqueApuntador apuntadoresTriples;
                                fseek(archivo, apuntadoresDobles.b_pointers[k], SEEK_SET);
                                fread(&apuntadoresTriples, sizeof(BloqueApuntador), 1, archivo);
                                //Tenemos que iterar el bloque
                                for(int m = 0; m < 16; m++){
                                    //Verificamos si el puntero tiene contenido
                                    if(apuntadoresTriples.b_pointers[m] != -1){
                                        //Es un un bloque de carpeta
                                        if(verify_inode_name(apuntadoresTriples.b_pointers[m], path, token)){
                                            posicion_bloque = apuntadoresTriples.b_pointers[m];
                                            bloque_encontrado = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                //Es un un bloque de carpeta
                if(verify_inode_name(inodo.i_block[i], path, token)){
                    posicion_bloque = inodo.i_block[i];
                    bloque_encontrado = true;
                    break;
                }
            }

            //Si encontramos la carpeta
            if(bloque_encontrado){
                //Verificamos si el siguiente ya es la carpeta final
                char * siguienteToken = token;
                siguienteToken = strtok(NULL, "/");
                siguienteToken = strtok(NULL, "/");
                //Esto quiere decir que encontramos la carpeta final
                if(siguienteToken == NULL) return true;
                //Encontramos la siguiente carpeta
                token = strtok(NULL, "/");
                return search_path(token, posicion_bloque, path);
            }
        }
    }
    //Esto es en caso de que no se haya encontrado
    //Cerramos el archivo
    fclose(archivo);
    return false;
}

bool make_dir(char * token, int inodo_start, string path){
    //Primero leemos el inodo
    int inodo_next = -1;
    //Primero obtenemos el superbloque
    TablaInodo inodo;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, inodo_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            //Encontramos apuntador a un bloque
            if(i == 12){
                //Es un bloque simple indirecto
            } else if(i == 13){
                //Es un bloque doble indirecto
            } else if(i == 14){
                //Es un bloque triple indirecto
            } else {
                //Es un un bloque de carpeta
                //Obtenemos el bloque de carpeta
                BloqueCarpeta carpetita;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
                //Imprimos el bloque solo para ver si todo va bien
                print_bloque_carpeta(carpetita);
                //Verificamos si tiene un hijo con el nombre
                for(int j = 1; j < 4; j++){
                    cout << "Comparando: " << carpetita.b_content[j].b_name << ", con : " << token << endl;
                    //No iteramos el primero porque es el nombre de la carpeta
                    if(strcmp(carpetita.b_content[j].b_name, token) == 0){
                        //Creo que deberiamos revisar si el inodo es de tipo carpeta jeje
                        bool esCarpeta = true;
                        if(esCarpeta){
                            //Verificamos si el siguiente ya es la carpeta final
                            char * siguienteToken = token;
                            siguienteToken = strtok(NULL, "/");
                            siguienteToken = strtok(NULL, "/");
                            //Esto quiere decir que encontramos la carpeta final
                            if(siguienteToken == NULL) return true;
                            //Encontramos la siguiente carpeta
                            token = strtok(NULL, "/");
                            return search_path(token, carpetita.b_content[j].b_inodo, path);
                        }
                    }
                }
            }
        }
    }
    //Esto es en caso de que no se haya encontrado
    //Cerramos el archivo
    fclose(archivo);
    return false;
}

void make_file(int start, string pathDisk, string path, int size, string cont, bool p){
    /*cout << "Start : " << start << endl;
    cout << "PathDisk : " << pathDisk << endl;
    cout << "Path : " << path << endl;
    cout << "Size : " << size << endl;
    cout << "Cont : " << cont << endl;*/

    //Primero obtenemos el superbloque
    SuperBloque bloquesito;
    FILE *archivo = fopen(pathDisk.c_str(), "rb+");
    fseek(archivo, start, SEEK_SET);
    fread(&bloquesito, sizeof(SuperBloque), 1, archivo);

    //Necesitamos partir el path entre carpetas, esto para ver si existe la carpeta
    char * ruta = &path[0];
    char * token = strtok(ruta, "/");
    bool exist_path = search_path(token, bloquesito.s_inode_start, pathDisk);
    if(!exist_path && !p){
        //No existe el path ni tampoco debemos de crearlo
        cout << "[Error] > No se ingreso el parametro de $id" << endl; 
        fclose(archivo);
        return;
    } else if(!exist_path && p){
        //No existe el path pero si lo podemos crear

    } else {
        //Si existe el path, por ende solo obtenemos el inodo y creamos el nuevo bloque

    }

    //Cerramos el file
    fclose(archivo);
}

void mkfile::make_mkfile(mkfile *archivito){
    cout << "Hola bb" << endl;
    //Validaciones
    //Parametros obligatorios
    if(archivito->id == "") { cout << "[Error] > No se ingreso el parametro de $id" << endl; return;}
    if(archivito->path == "") { cout << "[Error] > No se ingreso el parametro de $path" << endl; return;}
    //El size no puede ser menor a 0
    if(archivito->size < 0) { cout << "[Error] > El parametro de size no puede ser negativo" << endl; return;}

    //Validamos si existe la particion montada
    if(find_partition_in_mount(archivito->id)){
        //Quiere decir que si existe la particion montada
        //Ahora obtenemos el nodo de la lista con la particion montada
        nodoMount* nodo = get_partition_in_mount(archivito->id);
        FILE *archivo;
        if(archivo = fopen((nodo->path).c_str(), "rb+")){
            //Ahora validamos si el cont existe
            FILE *contFile;
            //Esto quiere decir que el archivo del cont si existe en la computadora
            //Obtenemos el mbr
            MBR master;
            fseek(archivo, 0 , SEEK_SET);
            fread(&master, sizeof(MBR), 1, archivo);
            //Obtenemos la posicion de la particion
            int start = get_partition_start(nodo->path, nodo->nombre, master);

            //Si devuelve -1 quiere decir que no encontro la particion
            if(start != -1) make_file(start, nodo->path, archivito->path, archivito->size, archivito->cont, archivito->p);
            else cout << "[ERROR] > No existe la particion con ese nombre" << endl;
            fclose(archivo);
        }else{
            cout << "[ERROR] > No existe ese disco" << endl;
        }
    } else {
        cout << "[ERROR] > No existe una particion montada con ese id" << endl;
    }
}