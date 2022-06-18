#include "analizador.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../mkdisk/mkdisk.h"
#include "../rmdisk/rmdisk.h"
#include "../fdisk/fdisk.h"
#include "../exec/exec.h"
#include "../mount/mount.h"
#include "../unmount/unmount.h"
#include "../mkfs/mkfs.h"
#include "../mkfile/mkfile.h"
#include "../mkdir/mkdir.h"
#include "../rep/rep.h"

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

/*Funcion para crear la copia de la ruta del archivo*/
string get_path_raid(string path){
    string path_raid = "";
    for(int i = 0; i < path.length(); i++){
        //Verificamos si el nombre es alfanumerico y con _
        char caracter = path.at(i);
        //Revisamos cuando el nombre traiga la extencion
        if(caracter == '.'){
            path_raid += "_copy"; 
        }
        path_raid += caracter;
    }
    return path_raid;
}
/*Funcion para devolver el path sin la ruta*/
string get_path_without_name(string path){
    string path_without_name = "";
    string tmp = "";
    for(int i = 0; i < path.length(); i++){
        //Verificamos si el nombre es alfanumerico y con _
        char caracter = path.at(i);
        tmp += caracter;
        if(caracter == '/') path_without_name = tmp;
    }
    return path_without_name;
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
    fdisk *particion = new fdisk();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$size"){
            particion->size = stoi(valor);
        } else if (tipo == "@unit"){
            particion->unidad = valor;
        } else if (tipo == "$path"){
            valor = delete_comillas(valor);
            particion->path = valor;
        } else if (tipo == "@type"){
            particion->tipo = valor;
        } else if (tipo == "@fit"){
            particion->ajuste = valor;
        }  else if (tipo == "@delete"){
            particion->eliminar = valor;
        } else if (tipo == "$name"){
            particion->nombre = valor;
        } else if (tipo == "@add"){
            particion->agregar = stoi(valor);
        } else if (tipo == "@mov"){
            particion->mov = true;
        } 
        parametros = strtok(NULL, " ");
    }
    //Imprimimos la clase de particion
    /*cout << "size: " << particion->size << endl;
    cout << "unit: " << particion->unidad << endl;
    cout << "path: " << particion->path << endl;
    cout << "type: " << particion->tipo << endl;
    cout << "fit: " << particion->ajuste << endl;
    cout << "delete: " << particion->eliminar << endl;
    cout << "name: " << particion->nombre << endl;
    cout << "add: " << particion->agregar << endl;
    cout << "mov: " << particion->mov << endl;*/

    //Creamos la particion
    particion->make_fdisk(particion);
}
/*Funcion para analizar los scripts, comando a comando*/
void analizar_exec(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    exec *exectutito = new exec();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual (los parametros ya vienen en lowercase)
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto
        if(tipo == "$path"){
            valor = delete_comillas(valor);
            exectutito->path = valor;
        }
        parametros = strtok(NULL, " ");
    }
    //Eliminamos el disco
    exectutito->make_exec(exectutito);
} 
/*Funcion para montar una particion*/
void analizar_mount(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    mount *mountsito = new mount();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual (los parametros ya vienen en lowercase)
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto
        if(tipo == "$path"){
            valor = delete_comillas(valor);
            mountsito->path = valor;
        } else if (tipo == "$name"){
            mountsito->name = valor;
        } 
        parametros = strtok(NULL, " ");
    }
    //Eliminamos el disco
    mountsito->make_mount(mountsito);
} 
/*Funcion para montar una particion*/
void analizar_unmount(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    while(parametros != NULL){
        //Inicializamos nuestro disco
        unmount *unmountsito = new unmount();
        //Obtenemos el tipo y el valor del parametro actual (los parametros ya vienen en lowercase)
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto
        if(tipo.length() > 3){
            if(tipo.at(0) == '$' && tipo.at(1) == 'i' && tipo.at(2) == 'd'){
                unmountsito->id = valor;
            }
        }
        //Hacemos el unmount
        unmountsito->make_unmount(unmountsito);
        parametros = strtok(NULL, " ");
    }

} 
/*Funcion para analizar el comando de mkfs*/
void analizar_mkfs(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    mkfs *mkfsito = new mkfs();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$id"){
            mkfsito->id = valor;
        } else if (tipo == "@type"){
            mkfsito->type = valor;
        } else if (tipo == "@add"){
            mkfsito->add = stoi(valor);
        } else if (tipo == "@unit"){
            mkfsito->unit = valor;
        }
        parametros = strtok(NULL, " ");
    }
    //Creamos la particion
    mkfsito->make_mkfs(mkfsito);
}
/*Funcion para analizar el comando de mkfile*/
void analizar_mkfile(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    mkfile *mkfilesito = new mkfile();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$id"){
            mkfilesito->id = valor;
        } else if (tipo == "$path"){
            valor = delete_comillas(valor);
            mkfilesito->path = valor;
        } else if (tipo == "@p"){
            mkfilesito->p = true;
        } else if (tipo == "@size"){
            mkfilesito->size = stoi(valor);
        } else if (tipo == "@cont"){
            valor = delete_comillas(valor);
            mkfilesito->cont = valor;
        }
        parametros = strtok(NULL, " ");
    }
    //Creamos la particion
    mkfilesito->make_mkfile(mkfilesito);
}
/*Funcion para analizar el comando de mkfile*/
void analizar_mkdir(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    mkdir *mkdirsito = new mkdir();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$id"){
            mkdirsito->id = valor;
        } else if (tipo == "$path"){
            valor = delete_comillas(valor);
            mkdirsito->path = valor;
        } else if (tipo == "@p"){
            mkdirsito->p = true;
        }
        parametros = strtok(NULL, " ");
    }
    //Creamos la particion
    mkdirsito->make_mkdir(mkdirsito);
}
/*Funcion para analizar el comando de mkfile*/
void analizar_rep(char *parametros){
    //Pasamos a la siguiente posicion
    parametros = strtok(NULL, " ");
    //Inicializamos nuestro disco
    rep *reportito = new rep();
    while(parametros != NULL){
        //Obtenemos el tipo y el valor del parametro actual
        string tmpParam = parametros;
        string tipo = get_tipo_parametro(tmpParam);
        string valor = get_valor_parametro(tmpParam);
        //Verificamos cual parametro es para inicializar el objeto (los parametros ya vienen en lowercase)
        if(tipo == "$name"){
            reportito->name = valor;
        } else if (tipo == "$path"){
            valor = delete_comillas(valor);
            reportito->path = valor;
        } else if (tipo == "$id"){
            reportito->id = valor;
        }
        parametros = strtok(NULL, " ");
    }
    //Creamos la particion
    reportito->make_rep(reportito);
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
    } else if (strcasecmp(token, "exec") == 0){
        analizar_exec(token);
    } else if (strcasecmp(token, "mount") == 0){
        analizar_mount(token);
    } else if (strcasecmp(token, "unmount") == 0){
        analizar_unmount(token);
    } else if (strcasecmp(token, "mkfs") == 0){
        analizar_mkfs(token);
    } else if (strcasecmp(token, "mkfile") == 0){
        analizar_mkfile(token);
    } else if (strcasecmp(token, "mkdir") == 0){
        analizar_mkdir(token);
    } else if (strcasecmp(token, "rep") == 0){
        analizar_rep(token);
    } else {
        cout << "Comando no aceptado :c" << endl;
    }
}