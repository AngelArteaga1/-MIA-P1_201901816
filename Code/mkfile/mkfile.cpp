#include "mkfile.h"
#include "../fdisk/fdisk.h"
#include "../listadoble/nodomount.h"
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

int recorridos = 0;

mkfile::mkfile(){ }

//Estructura que me apoyara para el bf o wf
typedef struct{
    int posicionInicial;
    int size;
}BloqueDisponible;

void bubble_sort_ascending(BloqueDisponible * a, int size){
    int pass = 0;
    BloqueDisponible temp;
    for(int i = 0; i<size; i++) {
        for(int j = i+1; j<10; j++)
        {
            if(a[j].size < a[i].size) {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
        pass++;
    }
}

void bubble_sort_descending(BloqueDisponible * a, int size){
    int pass = 0;
    BloqueDisponible temp;
    for(int i = 0; i<size; i++) {
        for(int j = i+1; j<10; j++)
        {
            if(a[j].size > a[i].size) {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
        pass++;
    }
}

//Esta funcion solo imprime un bloque de carpeta
void print_bloque_carpeta(BloqueCarpeta carpetita){
    cout << "**************************************" << endl;
    for(int i = 0; i < 4; i++){
        cout << " carpetita.b_name: " << carpetita.b_content[i].b_name << ", carpetita.b_inodo: " << carpetita.b_content[i].b_inodo << endl; 
    }
    cout << "**************************************" << endl;
}

//Esta funcion es para imprimir el inodo
void print_inodo(TablaInodo inodo){
    cout << "************** INODO RECORRIDO ****************" << endl;
    cout << "sizeof(TablaInodo): " << sizeof(TablaInodo) << endl;
    cout << "sizeof(Carpeta): " << sizeof(BloqueCarpeta) << endl;
    cout << "inodo.type: " << inodo.i_type << endl;
    for(int i = 0; i < 15; i++){
        cout << "inodo.i_block[" << i << "] = " << inodo.i_block[i] << endl; 
    }
}

//Devuelve el numero exacto dentro del bm donde se tiene que escribir el bloque o bitmap, con el metodo de primer ajuste
int first_fit(int count, int bm_start, int size, string path){
    /*Count es el numero de bloques o inodos que podemos usar dentro de nuestra particion
    //bm_start es la posicion donde empezara el bitmap de inodos o bloques dentro de la particion
    //size es el numero de bloques o inodos que vamos a escribir (esto unicamente seria con bloques de archivos)
    //Path es la ruta del disco*/
    
    //Primero abrimos el disco
    FILE *archivo = fopen(path.c_str(), "rb+");

    //Posicionamos el archivo en la posicion donde empieza el bitmap
    int contador = 0;
    int posicionInicial = -1;
    char status;
    bool espacioEncontrado = false;


    //Iteramos el bitmap
    fseek(archivo, bm_start, SEEK_SET);
    for(int i = 0; i < count; i++){
        //Primero leemos el estado del bloque o inodo actual
        fread(&status, sizeof(char), 1, archivo);
        //Primero verificamos si no se ha encontrado espacio anteriormente
        if(!espacioEncontrado){
            //Esto quiere decir que no hemos encontrado espacio aun
            if(status == '0'){
                espacioEncontrado = true;
                posicionInicial = i;
                contador++;
            }
        } else {
            //Esto quiere decir que ya estamos iterando espacios
            if(status == '0'){
                contador++;
            } else {
                contador = 0;
                espacioEncontrado = false;
                posicionInicial = -1;
            }
        }
        if(contador >= size) //Esto quiere decir que si hay espacio 
            break;
    }
    //Realizamos el ultimo filtro
    if(contador < size) posicionInicial = -1;

    //Actualizamos el bm si es que se logro ingresar el contenido
    if(posicionInicial != -1){
        char full = '1';
        fseek(archivo, bm_start + posicionInicial, SEEK_SET);
        for(int i = 0; i < size; i++)
            fwrite(&full, sizeof(char), 1, archivo);
    }

    //Cerramos el archivo
    fclose(archivo);
    return posicionInicial;
}

//Devuelve el numero exacto dentro del bm donde se tiene que escribir el bloque o bitmap, con el metodo de mejor ajuste
int best_fit(int count, int bm_start, int size, string path){
    //Primero abrimos el disco
    FILE *archivo = fopen(path.c_str(), "rb+");

    //Posicionamos el archivo en la posicion donde empieza el bitmap
    char status;
    int sizeBloques = 0;
    int sizelist = 0;
    int posicionInicial = -1;
    bool espacioEncontrado = false;

    //Primero contamos el numero de conjuntos de espacios libres que tenemos
    fseek(archivo, bm_start, SEEK_SET);
    for(int i = 0; i < count; i++){
        //Primero leemos el estado del bloque o inodo actual
        fread(&status, sizeof(char), 1, archivo);
        //Primero verificamos si no se ha encontrado espacio anteriormente
        if(!espacioEncontrado){
            //Esto quiere decir que no hemos encontrado espacio aun
            if(status == '0'){
                espacioEncontrado = true;
                sizelist++;
            }
        } else {
            //Esto quiere decir que ya estamos iterando espacios
            if(status == '1') espacioEncontrado = false;
        }
    }

    //Creamos un arreglo de los conjuntos de espacios libres que tenemos
    BloqueDisponible listaBloquesDisponibles[sizelist];
    int contador = 0;

    //Ahora tenemos que ingresar los bloques disponibles a la lista
    //Iteramos el bitmap
    fseek(archivo, bm_start, SEEK_SET);
    for(int i = 0; i < count; i++){
        //Primero leemos el estado del bloque o inodo actual
        fread(&status, sizeof(char), 1, archivo);
        //Primero verificamos si no se ha encontrado espacio anteriormente
        if(!espacioEncontrado){
            //Esto quiere decir que no hemos encontrado espacio aun
            if(status == '0'){
                espacioEncontrado = true;
                posicionInicial = i;
                sizeBloques++;
            }
        } else {
            //Esto quiere decir que ya estamos iterando espacios
            if(status == '0'){
                sizeBloques++;
            } else {
                //Primero ingresamos el espacio a la lista
                BloqueDisponible espacio;
                espacio.posicionInicial = posicionInicial;
                espacio.size = sizeBloques;
                listaBloquesDisponibles[contador] = espacio;
                contador ++;

                sizeBloques = 0;
                espacioEncontrado = false;
                posicionInicial = -1;
            }
        }
    }

    //Metemos el ultimo espacio en caso de que existe
    if(posicionInicial != -1) {
        BloqueDisponible espacio;
        espacio.posicionInicial = posicionInicial;
        espacio.size = size;
        listaBloquesDisponibles[contador] = espacio;
        contador ++;

        sizeBloques = 0;
        espacioEncontrado = false;
        posicionInicial = -1;
    }

    //Ahora tenemos que ordenar la lista
    bubble_sort_ascending(listaBloquesDisponibles, sizelist);

    //Ahora tenemos que iterar la lista y como esta ordenada ascendentemente, tomamos el primero donde quepa
    for(int i = 0; i < sizelist; i++){
        if(listaBloquesDisponibles[i].size >= size){
            posicionInicial = listaBloquesDisponibles[i].posicionInicial;
            break;
        }
    }

    //Actualizamos el bm si es que se logro ingresar el contenido
    if(posicionInicial != -1){
        char full = '1';
        fseek(archivo, bm_start + posicionInicial, SEEK_SET);
        for(int i = 0; i < size; i++)
            fwrite(&full, sizeof(char), 1, archivo);
    }

    //Cerramos el archivo
    fclose(archivo);
    return posicionInicial;
}

//Devuelve el numero exacto dentro del bm donde se tiene que escribir el bloque o bitmap, con el metodo de peor ajuste
int worst_fit(int count, int bm_start, int size, string path){
    cout << "Quiero ver si viene aca jajaja" << endl;
    cout << "Numero de bloques: " << count << endl;
    cout << "bm_start: " << bm_start << endl;
    cout << "size: " << size << endl;

    //Primero abrimos el disco
    FILE *archivo = fopen(path.c_str(), "rb+");

    //Posicionamos el archivo en la posicion donde empieza el bitmap
    char status;
    int sizeBloques = 0;
    int sizelist = 0;
    int posicionInicial = -1;


    bool espacioEncontrado = false;
    
    int sizeTmp = 0;
    int posicionTmp = 0;

    //int 

    //Primero contamos el numero de conjuntos de espacios libres que tenemos
    fseek(archivo, bm_start, SEEK_SET);
    for(int i = 0; i < count; i++){
        //Primero leemos el estado del bloque o inodo actual
        fread(&status, sizeof(char), 1, archivo);
        //cout << "status: " << status << endl;
        //Primero verificamos si no se ha encontrado espacio anteriormente
        if(!espacioEncontrado){
            //Esto quiere decir que no hemos encontrado espacio aun
            if(status == '0'){
                espacioEncontrado = true;
                sizeTmp++;
            }
        } else {
            //Esto quiere decir que ya estamos iterando espacios
            if(status == '1'){
                //Paramos de encontrar  el grupo libre
                if(sizeTmp)
                espacioEncontrado = false;
            } else {
                sizeTmp++;
            }
        }
    }

    //Creamos un arreglo de los conjuntos de espacios libres que tenemos
    BloqueDisponible listaBloquesDisponibles[sizelist];
    int contador = 0;

    //Ahora tenemos que ingresar los bloques disponibles a la lista
    //Iteramos el bitmap
    espacioEncontrado = false;
    fseek(archivo, bm_start, SEEK_SET);
    for(int i = 0; i < count; i++){
        //Primero leemos el estado del bloque o inodo actual
        fread(&status, sizeof(char), 1, archivo);
        //Primero verificamos si no se ha encontrado espacio anteriormente
        if(!espacioEncontrado){
            //Esto quiere decir que no hemos encontrado espacio aun
            if(status == '0'){
                espacioEncontrado = true;
                posicionInicial = i;
                sizeBloques++;
            }
        } else {
            //Esto quiere decir que ya estamos iterando espacios
            if(status == '0'){
                sizeBloques++;
            } else {
                //Primero ingresamos el espacio a la lista
                BloqueDisponible espacio;
                espacio.posicionInicial = posicionInicial;
                espacio.size = sizeBloques;
                listaBloquesDisponibles[contador] = espacio;
                contador ++;

                sizeBloques = 0;
                espacioEncontrado = false;
                posicionInicial = -1;
            }
        }
    }

    //Metemos el ultimo espacio en caso de que existe
    if(posicionInicial != -1) {
        BloqueDisponible espacio;
        espacio.posicionInicial = posicionInicial;
        espacio.size = size;
        listaBloquesDisponibles[contador] = espacio;
        contador ++;

        sizeBloques = 0;
        espacioEncontrado = false;
        posicionInicial = -1;
    }

    //Ahora tenemos que ordenar la lista
    bubble_sort_descending(listaBloquesDisponibles, sizelist);

    //Ahora tenemos que iterar la lista y como esta ordenada ascendentemente, tomamos el primero donde quepa
    cout << "listado de espacios disponibles" << endl;
    for(int i = 0; i < sizelist; i++){
        if(listaBloquesDisponibles[i].size >= size){
            cout << "Posicion inicial: " <<  listaBloquesDisponibles[i].posicionInicial << endl;
            cout << "Size del espacio: " <<  listaBloquesDisponibles[i].size << endl;
            posicionInicial = listaBloquesDisponibles[i].posicionInicial;
            break;
        }
    }

    //Actualizamos el bm si es que se logro ingresar el contenido
    if(posicionInicial != -1){
        char full = '1';
        fseek(archivo, bm_start + posicionInicial, SEEK_SET);
        for(int i = 0; i < count; i++)
            fwrite(&full, sizeof(char), 1, archivo);
    }

    //Cerramos el archivo
    fclose(archivo);
    return posicionInicial;
}


/*Esta funcion recibe el inicio del bloque carpeta, revisa si el nombre de la carpeta exite,
si si, entonces regresa true, si no encuentra regresa false*/
int verify_inode_name(int bloque_carpeta_start, string path, char * token){
    FILE *archivo = fopen(path.c_str(), "rb+");
    BloqueCarpeta carpetita;
    fseek(archivo, bloque_carpeta_start, SEEK_SET);
    fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
    //Imprimos el bloque solo para ver si todo va bien
    //cout << "*************** VERIFICANDO EL BLOQUE *****************" << endl;
    //cout << "En la posicion: " << bloque_carpeta_start << endl;
    //print_bloque_carpeta(carpetita);
    //Verificamos si tiene un hijo con el nombre
    for(int j = 1; j < 4; j++){
        //cout << "Comparando: " << carpetita.b_content[j].b_name << ", con : " << token << endl;
        //No iteramos el primero porque es el nombre de la carpeta
        if(strcmp(carpetita.b_content[j].b_name, token) == 0){
            //Creo que deberiamos revisar si el inodo es de tipo carpeta jeje
            bool esCarpeta = true;
            if(esCarpeta){
                return carpetita.b_content[j].b_inodo;
            }
        }
    }
    fclose(archivo);
    return -1;
}


//Esta funcion recibe el inicio del bloque carpeta, verifica si tiene hijos disponibles para poder enlazarlo a un inodo.
//Regresa true si fue posible la operacion, y false si no fue posible
//bloque_carpeta_start: la posicion donde se encuentra el bloque de carpeta
//path: la ruta donde se encuentra el disco
//inode_name: el nombre del inodo en caso de encontrar espacio para un hijo
//inode_start: la posicion del inodo hijo en caso de que encuentre un espacio
bool update_block_to_new_inode(int bloque_carpeta_start, string path, char * inode_children_name, int inode_children_start){
    /*cout << "*********** ACTUALIZANDO BLOQUE ************" << endl;
    cout << "En la posicion: " << bloque_carpeta_start << endl;
    cout << "Con el path: " << path << endl;
    cout << "Con el nombre del hijo: " << inode_children_name << endl;
    cout << "Con la posicion del hijo: " << inode_children_start << endl;*/




    FILE *archivo = fopen(path.c_str(), "rb+");
    BloqueCarpeta carpetita;
    fseek(archivo, bloque_carpeta_start, SEEK_SET);
    fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);

    //cout << "******* BLOQUE SIN ACTUALIZAR ********" << endl;
    //print_bloque_carpeta(carpetita);


    //Imprimos el bloque solo para ver si todo va bien
    //print_bloque_carpeta(carpetita);
    for(int i = 1; i < 4; i++){
        //No iteramos el primero porque es el nombre de la carpeta
        if(carpetita.b_content[i].b_inodo == -1){
            //Esto significa que si hay espacio para ingresar el nuevo inodo dentro de este bloque de carpeta
            //Actulizamos el bloque
            carpetita.b_content[i].b_inodo = inode_children_start;
            strcpy(carpetita.b_content[i].b_name, inode_children_name);
            //Escribimos el nuevo bloque
            //cout << "******* BLOQUE ACTUALIZADO ********" << endl;
            //print_bloque_carpeta(carpetita);

            fseek(archivo, bloque_carpeta_start, SEEK_SET);
            fwrite(&carpetita, sizeof(BloqueCarpeta), 1, archivo);

            //cout << "******* SI SE PUDO ACTUALIZAR ********" << endl;

            fclose(archivo);
            return true;
        }
    }
    fclose(archivo);
    //cout << "******* NO SE PUDO ACTUALIZAR ********" << endl;
    return false;
}

//Esta funcion crea un nuevo bloque de carpeta para el inodo padre, 
//de una vez enlaza el inodo al padre y devuelve la posicion donde se encuentra el bloque
//block_start: la posicion donde se escribe el nuevo bloque
//inodo_parent_start: la posicion donde empieza el inodo padre del bloque
//parent_name: el nombre del inodo padre
//path: la ruta donde se encuentra el disco
int make_new_block_directory(int block_start, int inodo_parent_start, char * parent_name, string path){
    FILE *archivo = fopen(path.c_str(), "rb+");
    //Creamos el bloque de la carpeta
    BloqueCarpeta carpetita;
    carpetita.b_content[0].b_inodo = inodo_parent_start;
    strcpy(carpetita.b_content[0].b_name, parent_name);
    carpetita.b_content[1].b_inodo = -1;
    strcpy(carpetita.b_content[1].b_name, "");
    carpetita.b_content[2].b_inodo = -1;
    strcpy(carpetita.b_content[2].b_name, "");
    carpetita.b_content[3].b_inodo = -1;
    strcpy(carpetita.b_content[3].b_name, "");
    //Aqui tenemos que escribir el bloque dentro de la particion
    fseek(archivo, block_start, SEEK_SET);
    fwrite(&carpetita, sizeof(BloqueCarpeta), 1, archivo);

    //cout << "Creamos carpeta en la posicion: " << block_start << ", con el nombre: " << parent_name << ", con puntero padre a :" << inodo_parent_start << endl;

    //Cerramos el archivo y devolvemos el inicio del bloque de carpeta 
    fclose(archivo);
    return block_start;
}

//Esta funcion crea un nuevo inodo, lo escribe directamente la particion
//inode_start: la posicion donde se escribira el nuevo inodo
//block_children_start: la posicion del bloque de la carpeta
//type: el tipo, si es carpeta o si es archivo
//path: la ruta donde se encuentra el disco
int make_new_inode(int inode_start, int block_children_start, char type, string path){
    FILE *archivo = fopen(path.c_str(), "rb+");
    TablaInodo inodito;
    inodito.i_uid = 1;
    inodito.i_gid = 1;
    inodito.i_size = 0;
    inodito.i_atime = time(nullptr);
    inodito.i_ctime = time(0);
    inodito.i_mtime = time(0);
    for(int i = 0; i < 15; i++)
        inodito.i_block[i] = -1;
    inodito.i_type = type;
    //Ingresamos la posicion del bloque de la carpeta raiz
    inodito.i_block[0] = block_children_start;
    //Aqui tenemos que escribir el inodo dentro de la particion
    fseek(archivo, inode_start, SEEK_SET);
    fwrite(&inodito, sizeof(TablaInodo), 1, archivo);
    fclose(archivo);
    return block_children_start;
}

//Esta funcion crea un bloque de apuntadores
//block_start: la posicion donde hay que escribir el bloque dentro del disco
//first_pointer: el primer puntero que tendra el bloque de apuntadores
//path: la ruta donde se encuentra el disco
int make_new_block_pointers(int block_start, int first_pointer, string path){
    FILE *archivo = fopen(path.c_str(), "rb+");
    BloqueApuntador bloquesito;
    for(int i = 0; i < 16; i++) bloquesito.b_pointers[i] = -1;
    bloquesito.b_pointers[0] = first_pointer;
    //Ahora escribimos
    fseek(archivo, block_start, SEEK_SET);
    fwrite(&bloquesito, sizeof(BloqueApuntador), 1, archivo);
    fclose(archivo);
    return block_start;
}

//Esta funcion se encarga de regresar la posicion del bloque libre para escribir
//Count: El numero de bloques disponible
//Bm_start: La posicion donde empieza el bitmap
//Block_start: La posicion donde empieza el area para escribir bloques
//Size: El numero de bloques que vamos a crear
//Size_struct: El tamanio de la estructura que se desea escribir
//Fit: La tecnica de colocacion que se utilizara
//Path: La ruta del del disco
int get_block_start_free(int count, int bm_start, int block_start, int size, char fit, int size_struct, string path){
    //cout << "size_struct: " << size_struct << endl;
    //Obtenemos la posicion del bloque
    int start = -1;
    if(fit == 'F') start = first_fit(count, bm_start, size, path);
    else if (fit == 'B') start = first_fit(count, bm_start, size, path);
    else start = first_fit(count, bm_start, size, path);
    if(start == -1) return start;
    //cout << "El numero de inodo disponible es:" << start << endl;
    //cout << "posicion del bloque es: " << block_start << " + ( " << start << " * " << size_struct << ")" << endl;
    start = block_start + (start * size_struct);
    return start;
}

//Esta funcion se encarga de buscar una carpeta dentro de un inodo con un nombre
//inodo: el inodo donde se buscara la carpeta
//token: el nombre de la carpeta que se esta buscando
//path: la ruta del disco
int exist_name_in_inode(TablaInodo inodo, char * token, string path){

    /*********** VERIFICAMOS SI LA CARPETA ACTUAL SI EXISTE *************/
    //Imprimimos el inodo
    //print_inodo(inodo);
    FILE *archivo = fopen(path.c_str(), "rb+");
    //Tenemos que iterar todo el inodo
    //En busca de un bloque de carpeta que tenga el nombre actual
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
                        if(posicion != -1) return posicion;
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
                                if(posicion != -1) return posicion;
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
                                        int posicion = verify_inode_name(apuntadoresDobles.b_pointers[k], path, token);
                                        if(posicion != -1) return posicion;
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                //Es un un bloque de carpeta
                int posicion = verify_inode_name(inodo.i_block[i], path, token);
                if(posicion != -1) return posicion;
            }
        }
    }

    //cout << "Este es el token: " << token << endl;

    return -1;
}


//Esta funcion busca el directorio dentro del sistema de archivos, si lo encuentra devuelve true, si no false
int search_path(char * token, int inodo_start, string path, int count_token, int total_token){
    cout << "Buscando el dir: " << token << endl;
    //Primero verificamos si el token siguiente es nulo, si si, significa que si existe la carpeta
    //cout << "************************ BUSCAMOS LA CARPETA *****************************" << endl;
    //cout << "Token: " << token << ", Count_token: " << count_token << ", Total_token: " << total_token << endl;

    //Primero verificamos si el token siguiente es nulo, si si, significa que si existe la carpeta
    if(count_token == total_token) return inodo_start;

    //Primero obtenemos el superbloque
    TablaInodo inodo;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, inodo_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);

    //Verificamos si el token se encuentra en el inodo actual
    int siguiente_inodo = exist_name_in_inode(inodo, token, path);
    if(siguiente_inodo != -1){
        //Encontramos la siguiente carpeta
        count_token++;
        token = strtok(NULL, "/");
        return search_path(token, siguiente_inodo, path, count_token, total_token);
    }

    //Si no encontro el archivo, devolvemos falso y cerramos el archivo
    fclose(archivo);
    return -1;
}

//Esta funcion se encarga de agregar un bloque a un inodo
bool set_inodo_to_inodo(int inodo_start, int nuevo_inodo, SuperBloque bloquesito, char * token, char fit, string path){
    /************************** AHORA TENEMOS QUE ACTUALIZAR EL INODO ACTUAL **************************/
    //Primero debemos de buscar si existe un bloque de carpeta libre, de ser asi, tenemos que ingresar
    //en unos de sus hijos, el puntero a la nueva carpeta, si no, debemos de crear uno nosotros
    //Primero obtenemos el inodo
    TablaInodo inodo;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, inodo_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);

    for(int i = 0; i < 15; i++){
        if(i == 12){
            //Simple apuntador indirecto
            if(inodo.i_block[i] != -1){
                //Obtenemos el bloque de apuntadores
                BloqueApuntador apuntadores;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                for(int j = 0; j < 16; j++){
                    if(apuntadores.b_pointers[j] != -1){
                        //apuntadores.b_pointers[j] tiene el apuntador a un bloque
                        if(update_block_to_new_inode(apuntadores.b_pointers[j], path, token, nuevo_inodo)){
                            fclose(archivo);
                            return true;
                        } 
                    } else {
                        //Esto quiere decir que tenemos que crear una nuevo bloque
                        int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                        if(apuntador_nuevo_bloque == -1){
                            cout << "[Error] > No hubo suficiente espacio para escribir un bloque" << endl;
                            fclose(archivo);
                            return false;
                        }
                        apuntadores.b_pointers[j] = make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                        //Ahora enlazamos el nuevo bloque al inodo que acabamos de crear
                        update_block_to_new_inode(inodo.i_block[i], path, token, nuevo_inodo);
                        //Actualizamos el bloque de apuntadores
                        fseek(archivo, inodo.i_block[i], SEEK_SET);
                        fwrite(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                        fclose(archivo);
                        return true;
                    }
                }
            } else {
                //Tenemos que ingresar un bloque indirecto que apunte a un bloque de carpeta que apunte al nuevo inodo
                //Primero creamos el bloque nuevo
                int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                
                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                int apuntador_nuevo_bloque_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                make_new_block_pointers(apuntador_nuevo_bloque_apuntador, apuntador_nuevo_bloque, path);

                //Actualizamos el apuntador
                inodo.i_block[i] = apuntador_nuevo_bloque_apuntador;
                fseek(archivo, inodo_start, SEEK_SET);
                fwrite(&inodo, sizeof(TablaInodo), 1, archivo);
                return true;
            }
        } else if (i == 13){
            //Doble apuntador indirecto
            if(inodo.i_block[i] != -1){
                //Obtenemos el bloque de apuntadores doble
                BloqueApuntador apuntadoresDobles;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&apuntadoresDobles, sizeof(BloqueApuntador), 1, archivo);
                for(int j = 0; j < 16; j++){
                    if(apuntadoresDobles.b_pointers[j] != -1){
                        //Obtenemos el bloque de apuntadores
                        BloqueApuntador apuntadores;
                        fseek(archivo, apuntadoresDobles.b_pointers[j], SEEK_SET);
                        fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                        for(int k = 0; k < 16; k++){
                            if(apuntadores.b_pointers[k] != -1){
                                //apuntadores.b_pointers[j] tiene el apuntador a un bloque
                                if(update_block_to_new_inode(apuntadores.b_pointers[k], path, token, nuevo_inodo)){
                                    fclose(archivo);
                                    return true;
                                }
                            } else {
                                //Significa que tenemos que crear un bloque y sobre ese, enlazar el nuevo inodo
                                int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                                apuntadores.b_pointers[k] = make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                                //Ahora enlazamos el nuevo bloque al inodo que acabamos de crear
                                update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                                //Actualizamos el apuntador
                                fseek(archivo, apuntadoresDobles.b_pointers[j], SEEK_SET);
                                fwrite(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                                fclose(archivo);
                                return true;
                            }
                        }
                    }else{
                        //Significa que tenemos que crear un bloque y sobre ese, enlazar el nuevo inodo
                        //Primero creamos el bloque nuevo
                        int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                        make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                        update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                        
                        //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                        int apuntador_nuevo_bloque_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                        make_new_block_pointers(apuntador_nuevo_bloque_apuntador, apuntador_nuevo_bloque, path);

                        //Actualizamos el apuntador
                        apuntadoresDobles.b_pointers[j] = apuntador_nuevo_bloque_apuntador;
                        fseek(archivo, inodo.i_block[i], SEEK_SET);
                        fwrite(&apuntadoresDobles, sizeof(BloqueApuntador), 1, archivo);
                        fclose(archivo);
                        return true;
                    }
                }
            } else {
                //Tenemos que meter un bloque de apuntadores que apunte a otro bloque de apuntadores 
                //que apunte un bloque de carpeta que apunte al nuevo inodo
                //Primero creamos el bloque nuevo
                int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                
                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                int apuntador_nuevo_bloque_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                make_new_block_pointers(apuntador_nuevo_bloque_apuntador, apuntador_nuevo_bloque, path);

                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                int apuntador_nuevo_bloque_apuntador_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                make_new_block_pointers(apuntador_nuevo_bloque_apuntador_apuntador, apuntador_nuevo_bloque_apuntador, path);

                //Actualizamos el apuntador
                inodo.i_block[i] = apuntador_nuevo_bloque_apuntador_apuntador;
                fseek(archivo, inodo_start, SEEK_SET);
                fwrite(&inodo, sizeof(TablaInodo), 1, archivo);
                fclose(archivo);
                return true;
            }
        } else if (i == 14){
            //Triple apuntador indirecto
            if(inodo.i_block[i] != -1){
                //Obtenemos el bloque de apuntadores doble
                BloqueApuntador apuntadoresTriples;
                fseek(archivo, inodo.i_block[i], SEEK_SET);
                fread(&apuntadoresTriples, sizeof(BloqueApuntador), 1, archivo);
                for(int j = 0; j < 16; j++){
                    if(apuntadoresTriples.b_pointers[j] != -1){
                        //Obtenemos el bloque de apuntadores
                        BloqueApuntador apuntadoresDobles;
                        fseek(archivo, apuntadoresTriples.b_pointers[j], SEEK_SET);
                        fread(&apuntadoresDobles, sizeof(BloqueApuntador), 1, archivo);
                        for(int k = 0; k < 16; k++){
                            if(apuntadoresDobles.b_pointers[k] != -1){
                                //Obtenemos el bloque de apuntadores
                                BloqueApuntador apuntadores;
                                fseek(archivo, apuntadoresDobles.b_pointers[k], SEEK_SET);
                                fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                                for(int m = 0; m < 16; m++){
                                    if(apuntadores.b_pointers[m] != -1){
                                        //apuntadores.b_pointers[j] tiene el apuntador a un bloque
                                        if(update_block_to_new_inode(apuntadores.b_pointers[m], path, token, nuevo_inodo)){
                                            fclose(archivo);
                                            return true;
                                        }
                                    } else {
                                        //tenemos que crear el bloque y asignarlo al apuntador
                                        int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                                        apuntadores.b_pointers[m] = make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                                        //Ahora enlazamos el nuevo bloque al inodo que acabamos de crear
                                        update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                                        //Actualizamos el apuntador
                                        fseek(archivo, apuntadoresDobles.b_pointers[k], SEEK_SET);
                                        fwrite(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                                        fclose(archivo);
                                        return true;
                                    }
                                }
                            } else {
                                //Tenemos que crear el bloque, crear un apuntador y asignarlo al apuntador
                                //Primero creamos el bloque nuevo
                                int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                                make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                                update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                                
                                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                                int apuntador_nuevo_bloque_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                                make_new_block_pointers(apuntador_nuevo_bloque_apuntador, apuntador_nuevo_bloque, path);

                                //Actualizamos el apuntador
                                apuntadoresDobles.b_pointers[k] = apuntador_nuevo_bloque_apuntador;
                                fseek(archivo, apuntadoresTriples.b_pointers[j], SEEK_SET);
                                fwrite(&apuntadoresDobles, sizeof(BloqueApuntador), 1, archivo);

                                fclose(archivo);
                                return true;
                            }
                        }
                    } else {
                        //Tenemos que crear un bloque, crear un apuntador, crear una puntador y asignarlo al apuntador
                        int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                        make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                        update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                        
                        //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                        int apuntador_nuevo_bloque_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                        make_new_block_pointers(apuntador_nuevo_bloque_apuntador, apuntador_nuevo_bloque, path);

                        //Ahora creamos el bloque de apuntadores apuntando a el bloque de apuntadores anterior
                        int apuntador_nuevo_bloque_apuntador_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                        make_new_block_pointers(apuntador_nuevo_bloque_apuntador_apuntador, apuntador_nuevo_bloque_apuntador, path);

                        //Actualizamos el apuntador
                        apuntadoresTriples.b_pointers[j] = apuntador_nuevo_bloque_apuntador_apuntador;
                        fseek(archivo, inodo.i_block[i], SEEK_SET);
                        fwrite(&apuntadoresTriples, sizeof(BloqueApuntador), 1, archivo);

                        fclose(archivo);
                        return true;
                    }
                }
            } else {
                //Tenemos que meter un bloque de apuntadores que apunte a otro bloque de apuntadores 
                //que apunte a otro bloque de apuntadores que apunte a un bloque de carpeta que apunte al nuevo inodo
                //Primero creamos el bloque nuevo
                int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                update_block_to_new_inode(apuntador_nuevo_bloque, path, token, nuevo_inodo);
                
                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                int apuntador_nuevo_bloque_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                make_new_block_pointers(apuntador_nuevo_bloque_apuntador, apuntador_nuevo_bloque, path);

                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                int apuntador_nuevo_bloque_apuntador_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                make_new_block_pointers(apuntador_nuevo_bloque_apuntador_apuntador, apuntador_nuevo_bloque_apuntador, path);

                //Ahora creamos el bloque de apuntadores apuntando directamente al bloque nuevo
                int apuntador_nuevo_bloque_apuntador_apuntador_apuntador = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueApuntador), path);
                make_new_block_pointers(apuntador_nuevo_bloque_apuntador_apuntador_apuntador, apuntador_nuevo_bloque_apuntador_apuntador, path);

                //Actualizamos el apuntador
                inodo.i_block[i] = apuntador_nuevo_bloque_apuntador_apuntador_apuntador;
                fseek(archivo, inodo_start, SEEK_SET);
                fwrite(&inodo, sizeof(TablaInodo), 1, archivo);

                fclose(archivo);
                return true;
            }
        } else {
            //Apuntador indirecto
            if(inodo.i_block[i] != -1){
                //apuntadores.b_pointers[j] tiene el apuntador a un bloque
                if(update_block_to_new_inode(inodo.i_block[i], path, token, nuevo_inodo)){
                    fclose(archivo);
                    return true;
                }
            } else {
                //Esto quiere decir que tenemos que crear un nuevo bloque
                int apuntador_nuevo_bloque = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
                make_new_block_directory(apuntador_nuevo_bloque, inodo_start, token, path);
                update_block_to_new_inode(inodo.i_block[i], path, token, nuevo_inodo);

                //Actualizamos el inodo
                inodo.i_block[i] = apuntador_nuevo_bloque;
                fseek(archivo, inodo_start, SEEK_SET);
                fwrite(&inodo, sizeof(TablaInodo), 1, archivo);

                fclose(archivo);
                return true;
            }
        }
    }
    //Si llegamos hasta aca es porque no hubo espacio dentro del inodo :c
    return false;
}

//Esta funcion crea el directorio dentro del sistema de archivos, si todo salio bien, devuelve true, si no devuelve false
int make_path(char * token, int inodo_start, string path, SuperBloque bloquesito, char fit, int count_token, int total_token){
    //cout << "************************ CREAMOS LA CARPETA*****************************" << endl;
    //cout << "Token: " << token << ", Count_token: " << count_token << ", Total_token: " << total_token << endl;
    //cout << "Posicion inicial del inodo: " << inodo_start << endl;
    //Primero verificamos si el token siguiente es nulo, si si, significa que si existe la carpeta
    if(count_token == total_token) return inodo_start;


    //Primero leemos el inodo
    TablaInodo inodo;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, inodo_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);

    //Verificamos si el token se encuentra en el inodo actual
    int siguiente_inodo = exist_name_in_inode(inodo, token, path);
    if(siguiente_inodo != -1){
        //Encontramos la siguiente carpeta
        count_token++;
        token = strtok(NULL, "/");
        return make_path(token, siguiente_inodo, path, bloquesito, fit, count_token, total_token);
    }

    //Si esta el token en el inodo, tenemos que crear el bloque de carpeta y agregarlo a este inodo
    //Obtenemos la posicion del bloque
    int carpetita_start = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, 1, fit, sizeof(BloqueCarpeta), path);
    int inodito_start = get_block_start_free(bloquesito.s_inodes_count, bloquesito.s_bm_inode_start, bloquesito.s_inode_start, 1, fit, sizeof(TablaInodo), path);

    //cout << "La posicion del nuevo bloque de la carpeta es:" << carpetita_start << endl;
    //cout << "La posicion del nuevo inodo es: " << inodito_start << endl;

    if(carpetita_start == -1 || inodito_start == -1){
        cout << "[Error] > No hubo suficiente espacio para escribir un bloque" << endl;
        fclose(archivo);
        return -1;
    }

    //Creamos el bloque de la carpeta
    make_new_block_directory(carpetita_start, inodito_start, token, path);
    //Creamos el inodo que tendra de apuntador a la carpeta
    make_new_inode(inodito_start, carpetita_start, '0', path);

    //Ahora tenemos que actualizar el inodo padre
    if(set_inodo_to_inodo(inodo_start, inodito_start, bloquesito, token, fit, path) == false){
        //Si llega a este punto, creo que es porque no encontro ningun espacio libre dentro del inodo (MUY RARO)
        cout << "[ERROR] > No se encontro mas espacio dentro del inodo" << endl;
        fclose(archivo);
        return -1;
    }

    //Cerramos el archivo antes se seguir iterando
    fclose(archivo);

    //Tenemos que pasar a la siguiente carpeta
    count_token++;
    token = strtok(NULL, "/");
    return make_path(token, inodito_start, path, bloquesito, fit, count_token, total_token);
}

int make_path_file(int inodo_start, SuperBloque bloquesito, char fit, char * token, string cont, string path){
    //Primero obtenemos el inodo
    TablaInodo inodo;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, inodo_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);

    //Verificamos si ya existe el archivo dentro del inodo
    int siguiente_inodo = exist_name_in_inode(inodo, token, path);
    /*if(siguiente_inodo != -1){
        TablaInodo inodoSiguiente;
        fseek(archivo, siguiente_inodo, SEEK_SET);
        fread(&inodoSiguiente, sizeof(TablaInodo), 1, archivo);
        //Obtenemos el contenido
        BloqueArchivo contenido;
        fseek(archivo, inodoSiguiente.i_block[0], SEEK_SET);
        fread(&contenido, sizeof(BloqueArchivo), 1, archivo);
        cout << contenido.b_content << endl;
    }*/

    //Tenemos que crear los bloques que llevaran el contenido
    //Obtenemos el size en bytes de contenido
    char contChar[cont.length()];
    strcpy(contChar, cont.c_str());

    int size_bloques = sizeof(contChar);
    //Obtenemos la cantidad de bloques que tendremos que usar para guardar el contenido
    int count_block = size_bloques / 49;
    int sobra_block = size_bloques % 49;
    if(sobra_block > 0) count_block++;

    //Obtenemos las posiciones de los bloques
    int posicion_inodito = -1;
    if(siguiente_inodo != -1) posicion_inodito = siguiente_inodo;
    else posicion_inodito = get_block_start_free(bloquesito.s_inodes_count, bloquesito.s_bm_inode_start, bloquesito.s_inode_start, 1, fit, sizeof(TablaInodo), path);
    int posicion_inicio_bloquesitos = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, count_block, fit, sizeof(BloqueArchivo), path);
    if(posicion_inicio_bloquesitos == -1 || posicion_inodito == -1){
        cout << posicion_inicio_bloquesitos << endl;
        cout << "[Error] > No se encontro espacio libre para la creacion del archivo" << endl; 
        return false;
    }

    //Obtenemos el inodo en caso de que ya exista,si no pos lo creamos
    TablaInodo inodito;
    if(siguiente_inodo != -1){
        fseek(archivo, posicion_inodito, SEEK_SET);
        fread(&inodito, sizeof(TablaInodo), 1, archivo);
    }
    inodito.i_type = '1';
    //Llenamos de -1 todas las posiciones
    for(int i = 0; i < 15; i ++)
        inodito.i_block[i] = -1;

    int posiciciones[count_block];

    //Creamos y escribimos los bloques
    fseek(archivo, posicion_inicio_bloquesitos, SEEK_SET);
    int indice = 0;
    for(int i = 0; i < count_block; i++){
        BloqueArchivo archivito;
        strcpy(archivito.b_name, token);
        if(i == count_block - 1 && sobra_block > 0){
            //Aqui solo debemos de meter los que sobran
            for(int j = 0; j < sobra_block; j++){
                archivito.b_content[j] = contChar[indice];
                indice++;
            }
        } else {
            //Aqui es llenar el bloque
            for(int j = 0; j < 49; j++){
                archivito.b_content[j] = contChar[indice];
                indice++;
            }
        }
        //Ahora tenemos que escribir el archivo
        posiciciones[i] = ftell(archivo);
        fwrite(&archivito, sizeof(BloqueArchivo), 1, archivo);
        //Reseteamos el contenido del bloque
        memset(archivito.b_content, 0, sizeof(archivito.b_content));
    }

    //Ahora tenemos que escribir las posiciones dentro del inodo
    for(int i = 0; i < count_block; i++){
        for(int j = 0; j < 15; j++){
            if(j < 12){
                if(inodito.i_block[j] == -1){
                    inodito.i_block[j] = posiciciones[i];
                    goto SiguienteIteracion;
                } 
            } else if(j == 12){
                if(inodito.i_block[j] == -1){
                    //tenemos que crear el nuevo bloque de apuntadores
                    int posicion_apuntadores = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                    inodito.i_block[j] =  make_new_block_pointers(posicion_apuntadores, posiciciones[i], path);
                    goto SiguienteIteracion;
                } else {
                    BloqueApuntador apuntadores;
                    fseek(archivo, inodito.i_block[j], SEEK_SET);
                    fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                    for (int m = 0; m < 16; m++){
                        if(apuntadores.b_pointers[m] == -1){
                            apuntadores.b_pointers[m] = posiciciones[i];
                            //Actualizamos el apuntador
                            fseek(archivo, inodito.i_block[j], SEEK_SET);
                            fwrite(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                            goto SiguienteIteracion;
                        }
                    }
                }
            } else if(j == 13){
                if(inodito.i_block[j] == -1){
                    //tenemos que crear el nuevo bloque de apuntadores
                    int posicion_apuntadores = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                    make_new_block_pointers(posicion_apuntadores, posiciciones[i], path);
                    //tenemos que crear el nuevo bloque de apuntadores
                    int posicion_apuntadores_dobles = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                    inodito.i_block[j] = make_new_block_pointers(posicion_apuntadores_dobles, posicion_apuntadores, path);
                    goto SiguienteIteracion;
                } else {
                    BloqueApuntador apuntadores_dobles;
                    fseek(archivo, inodito.i_block[j], SEEK_SET);
                    fread(&apuntadores_dobles, sizeof(BloqueApuntador), 1, archivo);
                    for (int m = 0; m < 16; m++){
                        if(apuntadores_dobles.b_pointers[m] == -1){
                            //tenemos que crear el nuevo bloque de apuntadores
                            int posicion_apuntadores = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                            apuntadores_dobles.b_pointers[m] = make_new_block_pointers(posicion_apuntadores, posiciciones[i], path);
                            //Actualizamos el apuntador
                            fseek(archivo, inodito.i_block[j], SEEK_SET);
                            fwrite(&apuntadores_dobles, sizeof(BloqueApuntador), 1, archivo);
                            goto SiguienteIteracion;
                        } else {
                            BloqueApuntador apuntadores;
                            fseek(archivo, apuntadores_dobles.b_pointers[m], SEEK_SET);
                            fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                            for (int p = 0; p < 16; p++){
                                if(apuntadores.b_pointers[p] == -1){
                                    apuntadores.b_pointers[p] = posiciciones[i];
                                    //Actualizamos el apuntador
                                    fseek(archivo, apuntadores_dobles.b_pointers[m], SEEK_SET);
                                    fwrite(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                                    goto SiguienteIteracion;
                                }
                            }
                        }
                    }
                }
            } else if(j == 14){
                if(inodito.i_block[j] == -1){
                    //tenemos que crear el nuevo bloque de apuntadores
                    int posicion_apuntadores = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                    make_new_block_pointers(posicion_apuntadores, posiciciones[i], path);
                    //tenemos que crear el nuevo bloque de apuntadores
                    int posicion_apuntadores_dobles = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                    make_new_block_pointers(posicion_apuntadores_dobles, posicion_apuntadores, path);
                    //tenemos que crear el nuevo bloque de apuntadores
                    int posicion_apuntadores_triples = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                    inodito.i_block[j] = make_new_block_pointers(posicion_apuntadores_triples, posicion_apuntadores_dobles, path);
                    goto SiguienteIteracion;
                } else {
                    BloqueApuntador apuntadores_triples;
                    fseek(archivo, inodito.i_block[j], SEEK_SET);
                    fread(&apuntadores_triples, sizeof(BloqueApuntador), 1, archivo);
                    for (int m = 0; m < 16; m++){
                        if(apuntadores_triples.b_pointers[m] == -1){
                            //tenemos que crear el nuevo bloque de apuntadores
                            int posicion_apuntadores = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                            make_new_block_pointers(posicion_apuntadores, posiciciones[i], path);
                            //tenemos que crear el nuevo bloque de apuntadores
                            int posicion_apuntadores_dobles = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                            apuntadores_triples.b_pointers[m] = make_new_block_pointers(posicion_apuntadores_dobles, posicion_apuntadores, path);
                            //Actualizamos el apuntador
                            fseek(archivo, inodito.i_block[j], SEEK_SET);
                            fwrite(&apuntadores_triples, sizeof(BloqueApuntador), 1, archivo);
                            goto SiguienteIteracion;
                        } else {
                            BloqueApuntador apuntadores_dobles;
                            fseek(archivo, apuntadores_triples.b_pointers[m], SEEK_SET);
                            fread(&apuntadores_dobles, sizeof(BloqueApuntador), 1, archivo);
                            for (int l = 0; l < 16; l++){
                                if(apuntadores_dobles.b_pointers[l] == -1){
                                    //tenemos que crear el nuevo bloque de apuntadores
                                    int posicion_apuntadores = get_block_start_free(bloquesito.s_block_count, bloquesito.s_bm_block_start, bloquesito.s_block_start, size_bloques, fit, sizeof(BloqueApuntador), path);
                                    apuntadores_dobles.b_pointers[l] = make_new_block_pointers(posicion_apuntadores, posiciciones[i], path);
                                    //Actualizamos el apuntador
                                    fseek(archivo, apuntadores_triples.b_pointers[m], SEEK_SET);
                                    fwrite(&apuntadores_dobles, sizeof(BloqueApuntador), 1, archivo);
                                    goto SiguienteIteracion;
                                } else {
                                    BloqueApuntador apuntadores;
                                    fseek(archivo, apuntadores_dobles.b_pointers[l], SEEK_SET);
                                    fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                                    for (int p = 0; p < 16; p++){
                                        if(apuntadores.b_pointers[p] == -1){
                                            apuntadores.b_pointers[p] = posiciciones[i];
                                            //Actualizamos el apuntador
                                            fseek(archivo, apuntadores_dobles.b_pointers[l], SEEK_SET);
                                            fwrite(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                                            goto SiguienteIteracion;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        SiguienteIteracion:
        int hola;
    }

    //Ahora escribimos el nuevo inodo
    fseek(archivo, posicion_inodito, SEEK_SET);
    fwrite(&inodito, sizeof(TablaInodo), 1, archivo);

    //Si el inodo es nuevo, tenemos que enlazarlo al nodo anterior jeje
    if(siguiente_inodo == -1)
        if(set_inodo_to_inodo(inodo_start, posicion_inodito, bloquesito, token, fit, path));


    //Mensaje de confirmacion
    cout << "[Success] > El archivo ha sido creado exitosamente" << endl;

    //Cerramos el archivo
    fclose(archivo);
    return posicion_inodito;
}



int get_count_tokens(char * token, int count){

    if (token == NULL) return count;

    //Iteramos el token
    count++;

    //Iteramos el token
    token = strtok(NULL, "/");
    return get_count_tokens(token, count);
}

void make_file(int start, string pathDisk, string path, int size, string cont, bool p, char fit){
    string pathCopia1 = path;
    string pathCopia2 = path;
    string pathCopia3 = path;

    //Obtenemos el cont
    string contenido = "";
    if(cont != ""){
        ifstream file(cont);
        if(file){
            string line;
            while (getline(file, line)){
                contenido += line;
            }
            file.close();
        }
    } else {
        if(size > 0){
            //Creamos el cont con el size
            char cont[size];
            int contador = 0;
            for(int i = 0; i < size; i++){
                if(contador == 10) contador = 0;
                cont[i] = static_cast<char>(contador+48);
                contador++;
            }
            string s(cont);
            contenido = s;
        } else {
            cout << "[Error] > No ingreso el parametro size ni el cont" << endl; 
            return;
        }
    }

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
    } else if(posicion_inodo_carpeta = -1 && p){
        //No existe el path pero si lo podemos crear
        ruta = &pathCopia2[0];
        token = strtok(ruta, "/");
        int start_inodo = make_path(token, bloquesito.s_inode_start, pathDisk, bloquesito, fit, 1, count_tokens);

        ruta = &pathCopia3[0];
        token = strtok(ruta, "/");
        for(int i = 0; i < count_tokens-1; i++) token = strtok(NULL, "/");
        make_path_file(start_inodo, bloquesito, fit, token, contenido, pathDisk);

    } else {
        //Si existe el path, por ende solo obtenemos el inodo y creamos el nuevo bloque
        ruta = &pathCopia3[0];
        token = strtok(ruta, "/");
        for(int i = 0; i < count_tokens-1; i++) token = strtok(NULL, "/");
        make_path_file(posicion_inodo_carpeta, bloquesito, fit, token, contenido, pathDisk);
    }

    //Cerramos el file
    fclose(archivo);
}


void mkfile::make_mkfile(mkfile *archivito){
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
        if(start != -1) make_file(start, realPath, archivito->path, archivito->size, archivito->cont, archivito->p, fit);
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