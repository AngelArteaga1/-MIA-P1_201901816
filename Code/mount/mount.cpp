#include "mount.h"
#include <stdio.h>
#include <string.h>

#include "../estructura/estructura.h"
#include "../fdisk/fdisk.h"
#include "../listadoble/listamount.h"
#include "../listadoble/nodomount.h"
#include "../analizador/analizador.h"


listaMount *listita = new listaMount;

mount::mount(){ }

//Busca si la particion con el mismo nombre y la misma ruta ya se encuentra montada
bool find_partition_in_mount(string name, string path){
    nodoMount *tmp =listita->primero;
    while (tmp != nullptr) {
        if (tmp->path == path && tmp->nombre == name){
            return true;
        }
        tmp = tmp->sig;
    }
    return false;
}
//Busca la particion con el mismo id y devuelve el treu en caso de que lo encuentre
bool find_partition_in_mount(string id){
    nodoMount *tmp =listita->primero;
    while (tmp != nullptr) {
        if (tmp->id == id){
            return true;
        }
        tmp = tmp->sig;
    }
    return false;
}
//Busca la particion con el mismo id y devuelve el nombre en caso de que lo encuentre
nodoMount* get_partition_in_mount(string id){
    nodoMount *tmp =listita->primero;
    while (tmp != nullptr) {
        if (tmp->id == id){
            return tmp;
        }
        tmp = tmp->sig;
    }
    return nullptr;
}

//Esta funcion devuelve el caracter correspondiente dependiendo del disco de los datos que se encuentran en la lista
int get_numero(listaMount *list, string name, string path){
    int numero = 1;
    nodoMount *tmp = list->primero;
    while(tmp !=nullptr){
        if((tmp->nombre == name) && (tmp->path == path)){
            //En caso de que ya exista la particion montada
            return -1;
        }else{
            if(tmp->path == path){
                //Esto quiere decir que si es el mismo disco, pero diferente particion
                numero = (tmp->numero)+1;
            }
        }
        tmp = tmp->sig;
    }
    return numero;
}
//Esta funcion devuelve la letra correspondiente dependiendo del disco en donde se encuentre y de la lista
int get_letra(listaMount *list, string name, string path){
    int letra = 'a';
    nodoMount *tmp = list->primero;
    while(tmp !=nullptr){
        if (tmp->path == path){
            //Si ya existe una particion con el mismo disco
            return tmp->letra;
        } else if (tmp->path != path && tmp->letra == letra){
            //Pasamos a la siguiente letra
            letra++;
        }
        tmp=tmp->sig;
    }
    return letra;
}


//Esta funcion imprime la lista de particiones montadas
void print_list(listaMount *list){
    nodoMount *tmp = list->primero;
    cout << "[INFO] > Listado de particiones montadas:" << endl;
    while(tmp != nullptr){
        cout << "\t> Ruta: " << tmp->path << ", Nombre: " << tmp->nombre << ", Id: " << tmp->id << endl;
        tmp = tmp->sig;
    }
}


//Esta funcion es la encargada de agregar la particion nueva a lista
void add_new_mount(string nombre, string ruta){
    //Basicamente es agregar la particion a la lista
    if(listita->primero == nullptr ){
        //Si el primero de la lista es nulo, por lo que tiene que ingresar en la primera posicion
        nodoMount *newNode = new nodoMount("vda1", ruta, nombre, 1, 'a');
        listita->primero = newNode;
        cout << "[SUCCESS] > Se monto la particion" << endl;
    }else{
        //Esto significa que no es el primero de la lista
        nodoMount *tmp = listita->primero;
        while(tmp->sig != nullptr){
            tmp = tmp->sig;
        }

        //Nos quedamos con el ultimo nodo de la lista
        int numero = get_numero(listita, nombre, ruta);
        if (numero != -1){
            int letra = get_letra(listita, nombre, ruta);
            char letraChar = static_cast<char>(letra);
            string id = "vd" + (letraChar + to_string(numero));
            nodoMount *newNode = new nodoMount(id, ruta, nombre, numero, letra);
            tmp->sig = newNode;
            cout << "[SUCCESS] > Se monto la particion" << endl;
        }else{
            cout << "[ERROR] > La particion ya ha sido montada" << endl;
        }
    }
}


/********************* FUNCION RAIZ QUE SE ENCARGA DE REALIZAR EL MOUNT Y METERLA A LA LISTA ***********************/
void mount::make_mount(mount *montar){
    //Validaciones
    if(montar->path == "" && montar->name != "") { cout << "[Error] > No se ingreso el parametro de $path" << endl; return;}
    if(montar->name == "" && montar->path != "") { cout << "[Error] > No se ingreso el parametro de $name" << endl; return;}
    //Si no trae parametros, quiere decir que solo necesitamos imprimir
    if(montar->path == "" && montar->name == "") {print_list(listita); return;}
    FILE *file;
    bool usaraRaid = false;
    string copyPath = get_path_raid(path);
    if (!(file = fopen(path.c_str(), "r"))) { 
        path = copyPath;
        usaraRaid = true;
    } else fclose(file);
    if(usaraRaid){
        if(!(file = fopen(path.c_str(), "r"))) {
            cout << "[Error] > No se ha encontrado el disco" << endl; 
            return;
        } else fclose(file);
    }

    //Leemos el disco
    file = fopen(path.c_str(),"rb+");
    MBR master;
    //Obtenemos el mbr
    fseek(file, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, file);
    
    //Verificamos si existe la particion dentro del disco
    if(!exist_partition(path, name, master)){ cout << "[Error] > No existe una particion con el mismo nombre " << endl; fclose(file); return;}

    //Si es particion primaria, tenemos que cambiar el status
    if(is_partition_primaria(master, name)){
        //Obtenemos la poscicion de la particion primaria
        int num = get_partition_primaria(master, name);
        //Cambiamos es status de la particion y actualizamos el mbr
        master.mbr_partitiones[num].part_status = '2';
        fseek(file, 0, SEEK_SET);
        fwrite(&master, sizeof(MBR), 1, file);
        //Agregamos la particion a la lista doble enlazada
        add_new_mount(montar->name, path);
    }
    //Si la particion es extendida
    else if(is_partition_extendida(master, name)){
        cout << "[Error] > No es posible montar una particion extendida" << endl;
        fclose(file);
        return;
    }
    //Quiere decir que es una particion logica
    else {
        //Obtenemos la posicion donde se encuentra la particion logica
        int posicionLogica = get_partition_logica(name, path);
        EBR logica;
        fseek(file, posicionLogica, SEEK_SET);
        fread(&logica, sizeof(EBR), 1, file);
        ///Ahora tenemos que actualizar el status y volver a escribir el ebr
        logica.part_status = '2';
        fseek(file, logica.part_start, SEEK_SET);
        fwrite(&logica, sizeof(EBR), 1, file);
        //Agregamos la particion a la lista de montadas
        add_new_mount(montar->name, montar->path);
    }
    fclose(file);
    //Realizamos una copia del disco
    if(!usaraRaid){
        string path_copy = get_path_raid(path);
        string cmd = "sudo cp \"" + path + "\" \"" + path_copy + "\"";
        system(cmd.c_str());
    }
}