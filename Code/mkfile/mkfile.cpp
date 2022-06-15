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
    cout << "************** BLOQUE RECORRIDO ****************" << endl;
    for(int i = 0; i < 4; i++){
        cout << " carpetita.b_name: " << carpetita.b_content[i].b_name << ", carpetita.b_inodo" << carpetita.b_content[i].b_inodo << endl; 
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

    //Cerramos el archivo
    fclose(archivo);
    return posicionInicial;
}

//Devuelve el numero exacto dentro del bm donde se tiene que escribir el bloque o bitmap, con el metodo de peor ajuste
int worst_fit(int count, int bm_start, int size, string path){
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
    bubble_sort_descending(listaBloquesDisponibles, sizelist);

    //Ahora tenemos que iterar la lista y como esta ordenada ascendentemente, tomamos el primero donde quepa
    for(int i = 0; i < sizelist; i++){
        if(listaBloquesDisponibles[i].size >= size){
            posicionInicial = listaBloquesDisponibles[i].posicionInicial;
            break;
        }
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

//Esta funcion busca el directorio dentro del sistema de archivos, si lo encuentra devuelve true, si no false
bool search_path(char * token, int inodo_start, string path){
    //Primero verificamos si el token siguiente es nulo, si si, significa que si existe la carpeta
    char * tokenSiguiente = token;
    tokenSiguiente = strtok(NULL, "/");
    if(tokenSiguiente == NULL) return true;

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

//Esta funcion crea el directorio dentro del sistema de archivos, si todo salio bien, devuelve true, si no devuelve false
int make_path(char * token, int inodo_start, string path, SuperBloque bloquesito, char fit){
    //Primero verificamos si el token siguiente es nulo, si si, significa que si existe la carpeta
    char * tokenSiguiente = token;
    tokenSiguiente = strtok(NULL, "/");
    if(tokenSiguiente == NULL) return inodo_start;

    //Primero leemos el inodo
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
                //Encontramos la siguiente carpeta
                token = strtok(NULL, "/");
                return make_path(token, posicion_bloque, path, bloquesito, fit);
            }
        }
    }

    /*********** SI LLEGO HASTA AQUI QUIERE DECIR QUE NO ENCUENTRA LA CARPETA ACTUAL *************/

    //Obtenemo la posicion del bloque
    int carpetita_start = -1;
    int inodito_start = -1;
    if(fit == 'F'){
        carpetita_start = first_fit(bloquesito.s_block_count, bloquesito.s_bm_block_start, 1, path);
        inodito_start = first_fit(bloquesito.s_inodes_count, bloquesito.s_bm_inode_start, 1, path);
    } 
    else if (fit == 'B') {
        carpetita_start = best_fit(bloquesito.s_block_count, bloquesito.s_bm_block_start, 1, path);
        inodito_start = best_fit(bloquesito.s_inodes_count, bloquesito.s_bm_inode_start, 1, path);
    } 
    else {
        carpetita_start = worst_fit(bloquesito.s_block_count, bloquesito.s_bm_block_start, 1, path);
        inodito_start = worst_fit(bloquesito.s_inodes_count, bloquesito.s_bm_inode_start, 1, path);
    }
    //Verificamos si los bloques fueron posibles de escribir
    if(carpetita_start == -1){
        cout << "[Error] > No hubo suficiente espacio para escribir la carpeta" << endl;
        fclose(archivo);
        return -1;
    }
    if(inodito_start == -1){
        cout << "[Error] > No hubo suficiente espacio para escribir la carpeta" << endl;
        fclose(archivo);
        return -1;
    }

    //Obtenemos el inicio de los dos bloques
    carpetita_start = bloquesito.s_block_start + (carpetita_start * sizeof(BloqueArchivo));
    inodito_start = bloquesito.s_inode_start + (inodo_start * sizeof(TablaInodo));

    //Creamos el bloque de la carpeta
    BloqueCarpeta carpetita;
    carpetita.b_content[0].b_inodo = inodo_start;
    strcpy(carpetita.b_content[0].b_name, token);
    carpetita.b_content[1].b_inodo = -1;
    strcpy(carpetita.b_content[1].b_name, "");
    carpetita.b_content[2].b_inodo = -1;
    strcpy(carpetita.b_content[2].b_name, "");
    carpetita.b_content[3].b_inodo = -1;
    strcpy(carpetita.b_content[3].b_name, "");
    //Aqui tenemos que escribir el bloque dentro de la particion
    fseek(archivo, carpetita_start, SEEK_SET);
    fwrite(&carpetita, sizeof(BloqueCarpeta), 1, archivo);


    //Tenemos que crear el inodo de la carpeta
    TablaInodo inodito;
    inodito.i_uid = 1;
    inodito.i_gid = 1;
    inodito.i_size = 0;
    inodito.i_atime = time(nullptr);
    inodito.i_ctime = time(0);
    inodito.i_mtime = time(0);
    for(int i = 0; i < 15; i++)
        inodito.i_block[i] = -1;
    inodito.i_type = '0';
    //Ingresamos la posicion del bloque de la carpeta raiz
    inodito.i_block[0] = carpetita_start;
    //Aqui tenemos que escribir el inodo dentro de la particion
    fseek(archivo, inodito_start, SEEK_SET);
    fwrite(&inodito, sizeof(TablaInodo), 1, archivo);


    /************************** AHORA TENEMOS QUE ACTUALIZAR EL INODO ACTUAL **************************/
    //Primero debemos de buscar si existe un bloque de carpeta libre, de ser asi, tenemos que ingresar
    //en unos de sus hijos, el puntero a la nueva carpeta, si no, debemos de crear uno nosotros
    for(int i = 0; i < 15; i++){
        if(i == 12){
            //Simple apuntador indirecto
        } else if (i == 13){
            //Doble apuntador indirecto

        } else if (i == 14){
            //Triple apuntador indirecto

        } else {
            //Apuntador indirecto
            if(inodo.i_block[i] != -1){
                 
            }

        }
    }

    //Cerramos el archivo antes se seguir iterando
    fclose(archivo);

    //Tenemos que pasar a la siguiente carpeta
    token = strtok(NULL, "/");
    return make_path(token, inodito_start, path, bloquesito, fit);
}


void make_file(int start, string pathDisk, string path, int size, string cont, bool p, char fit){
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
            char fit = get_partition_fit(nodo->path, nodo->nombre, master);

            //Si devuelve -1 quiere decir que no encontro la particion
            if(start != -1) make_file(start, nodo->path, archivito->path, archivito->size, archivito->cont, archivito->p, fit);
            else cout << "[ERROR] > No existe la particion con ese nombre" << endl;
            fclose(archivo);
        }else{
            cout << "[ERROR] > No existe ese disco" << endl;
        }
    } else {
        cout << "[ERROR] > No existe una particion montada con ese id" << endl;
    }
}