#include "analizador.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../mkdisk/mkdisk.h"
#include "../rmdisk/rmdisk.h"
#include "../fdisk/fdisk.h"

/*Funcion para saber si la cadena es aceptada como nombre*/
bool verify_name(string name){
    for(int i = 0; i < name.length(); i++){
        //Verificamos si el nombre es alfanumerico y con _
        char caracter = name.at(i);
        if(!(iswalnum(caracter) || caracter == '_' || caracter == '.')) return false;
        //Revisamos cuando el nombre traiga la extencion
        if(caracter == '.'){
            if(i + 3 == name.length() - 1){
                char d = name.at(i + 1);
                char s = name.at(i + 2);
                char k = name.at(i + 3);
                if(d != 'd' || s != 's' || k != 'k') return false;
            } else {
                return false;
            }
        }
    }
    return true;
}

/*Funcion para quitar las comillas a las rutas*/
string delete_comillas(string cadena){
    return cadena.substr(1, cadena.length() - 2);
}

/*Funciones que devuelven el tipo y el valor de un parametro en strings ya en lowercase */
string get_tipo_parametro(string parametro){
    //Iteramos hasta obtener el tipo del parametro
    string tipo = "";
    for(int i = 0; i < parametro.length(); i++){
        if(parametro.at(i) == '=') break;
        char caracter = tolower(parametro.at(i));
        tipo = tipo + caracter;
    }
    //devolvemos el string
    return tipo;
}
string get_valor_parametro(string parametro){
    //Iteramos hasta obtener el tipo del parametro
    string valor = "";
    bool concatenar = false;
    for(int i = 0; i < parametro.length(); i++){
        if(concatenar){
            char caracter = parametro.at(i);
            valor = valor + caracter;
        }
        if(parametro.at(i) == '>') concatenar = true;
    }
    //devolvemos el string
    return valor;
}


/*Funcion para analizar el comando de mkdisk*/
void analizar_mkdisk(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    mkdisk *disco = new mkdisk();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$size"){
            disco->size = stoi(valor);
        } else if (tipo == "$path"){
            valor = delete_comillas(valor);
            disco->path = valor;
        } else if (tipo == "$name"){
            disco->name = valor;
        }
        parametros = strtok(NULL, " ");
    }
    //Creamos el disco
    disco->make_mkdisk(disco);
}
/*Funcion para analizar el comando de mkdisk*/
void analizar_rmdisk(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    rmdisk *disco = new rmdisk();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual (los parametros ya vienen en lowercase)
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto
        if(tipo == "$path"){
            valor = delete_comillas(valor);
            disco->path = valor;
        }
        parametros = strtok(NULL, " ");
    }
    //Eliminamos el disco
    disco->make_rmdisk(disco);
}
/*Funcion para analizar el comando de fdisk*/
void analizar_fdisk(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    fdisk *disco = new fdisk();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$size"){
            disco->size = stoi(valor);
        } else if (tipo == "@unit"){
            disco->unidad = valor;
        } else if (tipo == "$path"){
            valor = delete_comillas(valor);
            disco->path = valor;
        } else if (tipo == "@type"){
            disco->tipo = valor;
        } else if (tipo == "@fit"){
            disco->ajuste = valor;
        }  else if (tipo == "@delete"){
            disco->eliminar = valor;
        } else if (tipo == "$name"){
            disco->nombre = valor;
        } else if (tipo == "@add"){
            disco->tipo = valor;
        } else if (tipo == "@mov"){
            disco->mov = true;
        } 
        parametros = strtok(NULL, " ");
    }
    //Imprimimos la clase de particion
    cout << "size: " << disco->size << endl;
    cout << "unit: " << disco->unidad << endl;
    cout << "path: " << disco->path << endl;
    cout << "type: " << disco->tipo << endl;
    cout << "fit: " << disco->ajuste << endl;
    cout << "delete: " << disco->eliminar << endl;
    cout << "name: " << disco->nombre << endl;
    cout << "add: " << disco->agregar << endl;
    cout << "mov: " << disco->mov << endl;

    //Creamos la particion
    //disco->make_mkdisk(disco);
}

/*Funcion que define que comando es el que hay que ejecutar*/
void analizar(char *comando) {
    char *token = strtok(comando, " ");
    if(strcasecmp(token, "mkdisk") == 0){
        analizar_mkdisk(token);
    } else if (strcasecmp(token, "rmdisk") == 0){
        analizar_rmdisk(token);
    } else if (strcasecmp(token, "fdisk") == 0){
        analizar_fdisk(token);
    } else {
        cout << "Comando no aceptado :c" << endl;
    }
}