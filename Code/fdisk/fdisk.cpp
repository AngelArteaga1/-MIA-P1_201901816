#include "fdisk.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#include "../estructura/estructura.h"
#include "../mount/mount.h"
#include "../analizador/analizador.h"

fdisk::fdisk(){ }


/*******************************  ESTAS SON FUNCIONES DE AYUDA, DEVUELVEN APUNTADORES O VALORES ***********************************/

//Esta funcion devuelvue falso si no existe la particion, true si si existe
bool exist_partition(string path, string name, MBR master){
    //Iteramos las particiones
    for (int i = 0; i < 4; ++i) {
        //Verificamos si existe el nombre en una particion primaria
        //cout << name << " " << master.mbr_partitiones[i].part_name << endl;
        if((strcmp(name.c_str(), master.mbr_partitiones[i].part_name)) == 0){
            return true;
        }else if (master.mbr_partitiones[i].part_type == 'E'){
            //Verificamos si la particion no existe en una particion logica
            EBR sub;
            int extendida = i;
            FILE *archivo = fopen(path.c_str(), "rb+");
            //Ponemos el puntero al inicio de la particion extendida
            fseek(archivo,master.mbr_partitiones[extendida].part_start, SEEK_SET);
            int posicionFinal = master.mbr_partitiones[extendida].part_start + master.mbr_partitiones[extendida].part_size;
            //Recorremos todas las particiones logicas
            while((fread(&sub, sizeof(EBR),1, archivo)) != 0 && (ftell(archivo) < posicionFinal)){
                //cout << "Logica: " << sub.part_name << endl; 
                if(strcmp(sub.part_name, name.c_str()) == 0){
                    fclose(archivo);
                    return true;
                }
                if(sub.part_next == -1){
                    break;
                }
            }
            fclose(archivo);
        }
    }
    return false;
}
//Esta funcion devuelve falso si existe una particion extendida dentro del disco, si no devuelve true
bool exist_partition_extendida(MBR master){
    for (int i = 0; i < 4; i++)
        if(master.mbr_partitiones[i].part_type == 'E') return true;
    return false;
}


//Esta funcion devuelvue la posicion donde empieza la particion
int get_partition_start(string path, string name, MBR master){
    //Iteramos las particiones
    for (int i = 0; i < 4; ++i) {
        //Verificamos si existe el nombre en una particion primaria
        //cout << name << " " << master.mbr_partitiones[i].part_name << endl;
        if((strcmp(name.c_str(), master.mbr_partitiones[i].part_name)) == 0){
            return master.mbr_partitiones[i].part_start;
        }else if (master.mbr_partitiones[i].part_type == 'E'){
            //Verificamos si la particion no existe en una particion logica
            EBR sub;
            int extendida = i;
            FILE *archivo = fopen(path.c_str(), "rb+");
            //Ponemos el puntero al inicio de la particion extendida
            fseek(archivo,master.mbr_partitiones[extendida].part_start, SEEK_SET);
            int posicionFinal = master.mbr_partitiones[extendida].part_start + master.mbr_partitiones[extendida].part_size;
            //Recorremos todas las particiones logicas
            while((fread(&sub, sizeof(EBR),1, archivo)) != 0 && (ftell(archivo) < posicionFinal)){
                //cout << "Logica: " << sub.part_name << endl; 
                if(strcmp(sub.part_name, name.c_str()) == 0){
                    fclose(archivo);
                    return sub.part_start + sizeof(EBR);
                }
                if(sub.part_next == -1){
                    break;
                }
            }
            fclose(archivo);
        }
    }
    return -1;
}
//Esta funcion devuelvue el size  de la particion
int get_partition_size(string path, string name, MBR master){
    //Iteramos las particiones
    for (int i = 0; i < 4; ++i) {
        //Verificamos si existe el nombre en una particion primaria
        //cout << name << " " << master.mbr_partitiones[i].part_name << endl;
        if((strcmp(name.c_str(), master.mbr_partitiones[i].part_name)) == 0){
            return master.mbr_partitiones[i].part_size;
        }else if (master.mbr_partitiones[i].part_type == 'E'){
            //Verificamos si la particion no existe en una particion logica
            EBR sub;
            int extendida = i;
            FILE *archivo = fopen(path.c_str(), "rb+");
            //Ponemos el puntero al inicio de la particion extendida
            fseek(archivo,master.mbr_partitiones[extendida].part_start, SEEK_SET);
            int posicionFinal = master.mbr_partitiones[extendida].part_start + master.mbr_partitiones[extendida].part_size;
            //Recorremos todas las particiones logicas
            while((fread(&sub, sizeof(EBR),1, archivo)) != 0 && (ftell(archivo) < posicionFinal)){
                //cout << "Logica: " << sub.part_name << endl; 
                if(strcmp(sub.part_name, name.c_str()) == 0){
                    fclose(archivo);
                    return sub.part_size - sizeof(EBR);
                }
                if(sub.part_next == -1){
                    break;
                }
            }
            fclose(archivo);
        }
    }
    return -1;
}
//Esta funcion devuelvue el ajuste donde empieza la particion
char get_partition_fit(string path, string name, MBR master){
    //Iteramos las particiones
    for (int i = 0; i < 4; ++i) {
        //Verificamos si existe el nombre en una particion primaria
        if((strcmp(name.c_str(), master.mbr_partitiones[i].part_name)) == 0){
            return master.mbr_partitiones[i].part_fit;
        }else if (master.mbr_partitiones[i].part_type == 'E'){
            //Verificamos si la particion no existe en una particion logica
            EBR sub;
            int extendida = i;
            FILE *archivo = fopen(path.c_str(), "rb+");
            //Ponemos el puntero al inicio de la particion extendida
            fseek(archivo,master.mbr_partitiones[extendida].part_start, SEEK_SET);
            int posicionFinal = master.mbr_partitiones[extendida].part_start + master.mbr_partitiones[extendida].part_size;
            //Recorremos todas las particiones logicas
            while((fread(&sub, sizeof(EBR),1, archivo)) != 0 && (ftell(archivo) < posicionFinal)){
                //cout << "Logica: " << sub.part_name << endl; 
                if(strcmp(sub.part_name, name.c_str()) == 0){
                    fclose(archivo);
                    return sub.part_fit;
                }
                if(sub.part_next == -1){
                    break;
                }
            }
            fclose(archivo);
        }
    }
    return ' ';
}


//Esta funcion busca la particion, devuelve true si la particion es primaria
bool is_partition_primaria(MBR master, string name){
    for (int i = 0; i < 4; i++) {
        if(strcmp(master.mbr_partitiones[i].part_name, name.c_str()) == 0){
            if(master.mbr_partitiones[i].part_type != 'E'){
                return true;
            }
        }
    }
    return false;
}
//Esta funcion busca la particion, devuelve true si la particion es extendida
bool is_partition_extendida(MBR master, string name){
    for (int i = 0; i < 4; i++) {
        if(strcmp(master.mbr_partitiones[i].part_name, name.c_str()) == 0){
            if(master.mbr_partitiones[i].part_type == 'E'){
                return true;
            }
        }
    }
    return false;
}
//Esta funcion busca la particion, primaria y devuelve la posicion que esta dentro del mbr


int get_partition_primaria(MBR master, string name){
    for (int i = 0; i < 4; i++) {
        if(strcmp(master.mbr_partitiones[i].part_name, name.c_str()) == 0){
            if(master.mbr_partitiones[i].part_type != 'E'){
                return i;
            }
        }
    }
    return -1;
}
//Esta funcion busca la particion, extendida que se encuentra en el disco y devuelve la posicion dentro del mbr
int get_partition_extendida(MBR master){
    for (int i = 0; i < 4; i++)
        if(master.mbr_partitiones[i].part_type == 'E') return i;
    return -1;
}
//Esta funcion busca la particion dentro de las logicas, si la encuentra devuelve la posicion para leer el ebr
int get_partition_logica(string name, string path){
    FILE *archivo = fopen(path.c_str(),"rb+");
    MBR master;
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);
    int num = get_partition_extendida(master);
    if(num != -1){
        EBR Extendida;
        fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
        fread(&Extendida, sizeof(EBR), 1 , archivo);
        //Iteramos hasta encontrar la particion logica
        while ((ftell(archivo) < master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start)) {
            if (strcmp(Extendida.part_name, name.c_str()) == 0){
                //Retornamos la posicion exacta donde el ebr comienza
                return (ftell(archivo) - sizeof(EBR));
            }
            if(Extendida.part_next == -1) break;
            //Vamos a la siguiente posicion
            fseek(archivo, Extendida.part_next, SEEK_SET);
            fread(&Extendida, sizeof(EBR), 1 , archivo);
        }
    }
    fclose(archivo);
    return -1;
}


//Esta funcion devuelve el espacio usado dentro de la particion extendida
int get_memory_used_extendida(string nombre, string path){
    FILE *archivo  = fopen(path.c_str(),"rb+");
    MBR master;
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);
    int num = get_partition_extendida(master);
    int size = 0;
    if(num != -1){
        //Obtenemos el primer ebr de la particion extendida
        EBR Extendida;
        fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
        fread(&Extendida, sizeof(EBR), 1 , archivo);
        //Iteramos hasta encontrar a la particion logica
        while ((ftell(archivo) < master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start)) {
            size += Extendida.part_size;
            if(Extendida.part_next == -1) return size;
            //seguimos iterando
            fseek(archivo, Extendida.part_next, SEEK_SET);
            fread(&Extendida, sizeof(EBR), 1 , archivo);
        }
    }
    fclose(archivo);
    return -1;
}
//Esta funcion devuelve el espacio usado dentro del disco, solo las particion primarias o extendidas
int get_memory_used_mbr(MBR master){
    int espacio = 0; //Kilobyte
    for (int i = 0; i < 4; i++) {
        if (master.mbr_partitiones[i].part_status != '1'){
            espacio += master.mbr_partitiones[i].part_size;
        }
    }
    espacio += (int)sizeof(MBR);
    return espacio;
}


//Esta funcion busca la posicion libre dentro del mbr para poder escribir una particion
int get_num_partition_free(MBR master, int size){
    for (int i = 0; i < 4; i++) {
        //Verificamos si tenemos un espacio libre
        if(master.mbr_partitiones[i].part_start == -1) return i;
        //Verificamos si existe un espacio donde hubo una particion anteriormente y cabe en este espacion
        if(master.mbr_partitiones[i].part_status == '1' && master.mbr_partitiones[i].part_size >= size) return i;
    }
    return -1;
}
//Esta funcion busca si es posible escribir una particion primaria o extensa dentro del disco
bool is_posible_write_partition(MBR master, int size){
    for (int i = 0; i < 4; i++) {
        //Verificamos si tenemos un espacio libre
        if(master.mbr_partitiones[i].part_start == -1) return true;
        //Verificamos si existe un espacio donde hubo una particion anteriormente y cabe en este espacion
        if(master.mbr_partitiones[i].part_status == '1' && master.mbr_partitiones[i].part_size >= size) return true;
    }
    return false;
}


/*******************************  ESTAS SON LAS FUNCIONES QUE REALIZAN TODAS LAS OPERACIONES DE PARTICIONES ***********************************/

void make_primaria(string name, string path, int size, char fit){
    //Obtenemos el mbr
    char buffer = '1';
    MBR master;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);

    //Si tenemos espacio para una particion primaria
    if (is_posible_write_partition(master, size)){
        //Obtenemos la posicion de la particion libre
        int num = get_num_partition_free(master, size);
        //Obtenemos el espacio que hemos usado en el disco
        int EspacioUse = get_memory_used_mbr(master);

        //VALIDACIONES
        //Verificamos si si podemos usar el espacio
        if((master.mbr_tamano-EspacioUse) < size){ cout << "[Error] > No hay suficiente espacio libre" << endl; fclose(archivo); return; }
        if(exist_partition(path, name, master)){ cout << "[Error] > Ya existe una particion con el mismo nombre " << endl; fclose(archivo); return;}

        /**************** LLENAMOS LOS DATOS DE LA PARTICION *****************/
        master.mbr_partitiones[num].part_type = 'P';
        master.mbr_partitiones[num].part_fit = fit;
        //Obtenemos la posicion donde comienza la particion, si es primera o si no
        if(num == 0) master.mbr_partitiones[num].part_start = sizeof(MBR); 
        else master.mbr_partitiones[num].part_start = master.mbr_partitiones[num-1].part_start + master.mbr_partitiones[num-1].part_size;
        master.mbr_partitiones[num].part_size = size;
        master.mbr_partitiones[num].part_status = '0';
        strcpy(master.mbr_partitiones[num].part_name,name.c_str());

        //Escribimos el mbr actualizado
        fseek(archivo, 0, SEEK_SET);
        fwrite(&master, sizeof(MBR), 1, archivo);
        fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
        //Escribimos un 1 en la particion nueva
        for (int i = 0; i < size; i++) 
            fwrite(&buffer, 1,1,archivo);
        cout << "[Success] > Se creo correctamente la particion primaria" << endl;
    }else{
        for (int i = 0; i < 4; i++) {
            if((master.mbr_partitiones[i].part_status == '1' && master.mbr_partitiones[i].part_size < size)){
                cout << "[Error] > No hay suficiente espacio libre" << endl;
                fclose(archivo);
                return;
            } else {
                cout << "[Error] > Ya hay 4 particiones creadas en el disco" << endl;
                fclose(archivo);
                return;
            }
        }
    }
    fclose(archivo);
}

void make_extendida(string name, string path, int size, char fit){
    //Obtenemos el mbr
    char buffer = '1';
    MBR master;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);

    //Verificamos si existe una particion extendida
    if(exist_partition_extendida(master)) { cout << "[Error] > Ya existe una particion extendida en el disco" << endl; fclose(archivo); return; }

    //Verificamos si es posible crear una particion extendida mas
    if(is_posible_write_partition(master, size)){
        //Obtenemos la posicion de la particion libre
        int num = get_num_partition_free(master, size);
        //Obtenemos el espacio que hemos usado en el disco
        int EspacioUse = get_memory_used_mbr(master);

        //VALIDACIONES
        //Verificamos si si podemos usar el espacio
        if((master.mbr_tamano-EspacioUse) < size){ cout << "[Error] > No hay suficiente espacio libre" << endl; fclose(archivo); return; }
        //Verificamo si existe una particion con el mismo nombre
        if(exist_partition(path, name, master)){ cout << "[Error] > Ya existe una particion con el mismo nombre " << endl; fclose(archivo); return;}

        /**************** LLENAMOS LOS DATOS DE LA PARTICION *****************/
        master.mbr_partitiones[num].part_type = 'E';
        master.mbr_partitiones[num].part_fit = fit;
        //Obtenemos la posicion donde comienza la particion, si es primera o si no
        if(num==0) master.mbr_partitiones[num].part_start = sizeof(MBR);
        else master.mbr_partitiones[num].part_start = master.mbr_partitiones[num-1].part_start + master.mbr_partitiones[num-1].part_size;
        master.mbr_partitiones[num].part_size = size;
        master.mbr_partitiones[num].part_status = '0';
        strcpy(master.mbr_partitiones[num].part_name,name.c_str());

        //Escribimos el mbr actualizado
        fseek(archivo, 0, SEEK_SET);
        fwrite(&master, sizeof(MBR), 1, archivo);
        fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);

        //Creamos el ebr para la particion extendida
        EBR Extendida;
        Extendida.part_fit = fit;
        Extendida.part_start = master.mbr_partitiones[num].part_start;
        Extendida.part_status = '0';
        Extendida.part_next = -1;
        Extendida.part_size = 0;
        strcpy(Extendida.part_name, "");

        //Escribimos el ebr en la primera posicion de la nueva particion extendida y llenamos lo demas con 1
        fwrite(&Extendida, sizeof(EBR),1,archivo);
        for (int i = 0; i < (size-(int)sizeof(EBR)); i++) 
            fwrite(&buffer,1,1,archivo);
        cout << "[Success] > Se creo correctamente la particion extendida" << endl;
    }else{
        for (int i = 0; i < 4; i++) {
            if((master.mbr_partitiones[i].part_status == '1' && master.mbr_partitiones[i].part_size < size)){
                cout << "[Error] > No hay suficiente espacio libre" << endl;
                fclose(archivo);
                return;
            } else {
                cout << "[Error] > Ya hay 4 particiones creadas en el disco" << endl;
                fclose(archivo);
                return;
            }
        }
    }
    fclose(archivo);
}

void make_logica(string name, string path, int size, char fit){
    //Obtenemos el mbr
    char buffer = '1';
    MBR master;
    FILE *archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);

    //Validaciones
    //Verificamos si ya existe una particion extendida
    if(!exist_partition_extendida(master)){ cout << "[Error] > No existe una particion extendida en el disco" << endl; fclose(archivo); return;}
    //Verificamos si ya existe una particion con el mismo nombre
    if(exist_partition(path, name, master)){ cout << "[Error] > Ya existe una particion con el mismo nombre " << endl; fclose(archivo); return;}

    //Obtenemos la posicion de la particion extendida, luego obtenemos el puntador del primer ebr
    EBR Extenida;
    int num = get_partition_extendida(master);
    int ext_start = master.mbr_partitiones[num].part_start;
    fseek(archivo, ext_start, SEEK_SET);
    fread(&Extenida, sizeof(EBR), 1, archivo);
    //Cuando es la primera particion logica
    if(Extenida.part_size == 0){
        //Verificamos si tenemos espacio dentro de la extendida
        if(master.mbr_partitiones[num].part_size > size){
            //Seteamos el ebr
            Extenida.part_size = size;
            Extenida.part_start = ftell(archivo) - sizeof(EBR);
            Extenida.part_status = '0';
            Extenida.part_next = -1;
            Extenida.part_fit = fit;
            strcpy(Extenida.part_name, name.c_str());
            //Escribimos el nuevo ebr
            fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
            fwrite(&Extenida, sizeof(EBR), 1, archivo);
            cout << "[Success] > Se creo correctamente la particion logica" << endl;
        }else{
            cout << "[Error] > No hay suficiente espacio dentro de la particion extendida" << endl;
        }
    }else{
        bool particion_eliminada = false;
        //iteramos la lista de particiones logicas
        while((master.mbr_partitiones[num].part_start + master.mbr_partitiones[num].part_size > ftell(archivo)) && 
        (Extenida.part_next != -1 || Extenida.part_status == '1')){
            //Obtenemos la particion logica temporal en 'Extendida'
            fseek(archivo, Extenida.part_next, SEEK_SET);
            fread(&Extenida, sizeof(EBR), 1, archivo);
            //Si la particion fue eliminada por fast
            if(Extenida.part_status == '1'){
                //Si existe espacion suficiente
                if (Extenida.part_size >= size){
                    particion_eliminada = true;
                    break;
                }
            }
            //Si es el final de la lista
            if(Extenida.part_next == -1){
                break;
            }
        }
        //En caso de que la particion fue eliminada y se encuentra con status de baja
        if (particion_eliminada){
            //Aprovechamos el ebr para poner los nuevos datos
            Extenida.part_status = '0';
            Extenida.part_size = size;
            Extenida.part_fit = fit;
            strcpy(Extenida.part_name, name.c_str());
            //Escribimos el ebr nuevo
            fseek(archivo, ftell(archivo)-sizeof(EBR), SEEK_SET);
            fwrite(&Extenida, sizeof(EBR), 1, archivo);
            //Mensaje de confirmacion
            cout << "[Success] > Se creo correctamente la particion logica" << endl;
        }else{
            //Verificamos si hay espacio para escribir la particion
            int useEspacio = Extenida.part_size + Extenida.part_start + size;
            if((master.mbr_partitiones[num].part_size+master.mbr_partitiones[num].part_start) >= useEspacio){
                //Actualizamos el actual ebr (el ultimo), para que apunte al siguiente jeje
                Extenida.part_next = Extenida.part_start + Extenida.part_size;
                fseek(archivo, ftell(archivo)-sizeof(EBR), SEEK_SET);
                fwrite(&Extenida, sizeof(EBR), 1, archivo);
                fseek(archivo, Extenida.part_start + Extenida.part_size, SEEK_SET);

                //Creamos el nuevo ebr
                Extenida.part_size = size;
                Extenida.part_start = ftell(archivo);
                Extenida.part_status = '0';
                Extenida.part_next = -1;
                Extenida.part_fit = fit;
                //Escribimos sin un fseek porque el archivo quedo apuntando en el final cabal
                strcpy(Extenida.part_name, name.c_str());
                fwrite(&Extenida, sizeof(EBR), 1, archivo);
                cout << "[Success] > Se creo correctamente la particion logica" << endl;
            }else{
                cout << "[Error] > No hay espacio suficiente" << endl;
            }
        }
    }
    fclose(archivo);
}

void update_partition(string name, string path, int size){
    //Obtenemos el mbr del disco
    FILE *archivo = fopen(path.c_str(), "rb+");
    MBR master;
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR),1 , archivo);

    //Validaciones
    //Aqui estamos validando que la particion no se encuentre montada
    if(find_partition_in_mount(name, path)) { cout << "[Error] > No se puede actualizar una particion montada" << endl; fclose(archivo); return; }
    if(size == 0) { cout << "[Error] > El tamaño para actualizar la particion es invalido" << endl; fclose(archivo); return; }
    if(!exist_partition(path, name, master)) { cout << "[Error] > No existe una particion con ese nombre" << endl; fclose(archivo); return; }

    //Miramos si tenemos que quitar o agregar espacio a la particion
    bool agregar = size > 0 ? true : false;

    //  SI ES PARTICION PRIMARIA
    if(is_partition_primaria(master, name)){
        //Actualizamos espacion dentro de la particion primaria
        int num = get_partition_primaria(master, name);
        if (agregar == true){
            //Tenemos que agregar espacio a la particion
            bool esUltimaParticion = num == 3 ? true : false;
            if(!esUltimaParticion){
                //ParticionSiguiente.start - posicionFinalDeLaParticionActual = diferencia entre una particion y la siguiente 
                int diferencia = master.mbr_partitiones[num+1].part_start - (master.mbr_partitiones[num].part_start + master.mbr_partitiones[num].part_size);
                if(diferencia != 0){
                    //Quiere decir que si existe una diferencia de espacio entre esta particion y la siguiente
                    if(diferencia >= size){
                        //Quiere decir que si hay espacio sufiente para aumentar la particion primaria
                        master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size;
                        fseek(archivo, 0, SEEK_SET);
                        fwrite(&master, sizeof(MBR), 1, archivo);
                        cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                    }else if(master.mbr_partitiones[num+1].part_status == '0' && master.mbr_partitiones[num+1].part_size == 0){
                        //Quiere decir que no es la ultima particion pero si la ultima que se ingreso
                        //size es menor o igual que el size que esta disponible (size del disco menos el final de esta particion)
                        if(size <= (master.mbr_tamano - (master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start))){
                            //Quiere decir que si hay espacio dentro del disco para sumar espacio a la particion
                            
                            //Sumamos el size a la particion y actualizamos el mbr
                            master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size;
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(MBR), 1, archivo);
                            cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                        }else{
                            cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                        }
                    }else{
                        cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                    }
                }else{
                    //Quiere decir que no hay diferencia entre una particion y la otra
                    //Revisamos si la siguiente particion fue eliminada
                    if(master.mbr_partitiones[num+1].part_status == '1'){
                        //Revisamos si cabe el espacio dentro de la particion siguiente eliminada
                        if(size <= master.mbr_partitiones[num+1].part_size){
                            //Si existe espacio, tenemos que actualizar ambas particiones
                            master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size; //Aumentamos el size
                            master.mbr_partitiones[num+1].part_start = master.mbr_partitiones[num+1].part_start + size; //Actualizamos el start de la siguiente
                            master.mbr_partitiones[num+1].part_size = master.mbr_partitiones[num+1].part_size - size; //Actualizamos el size de la siguiente
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(MBR), 1, archivo);
                            cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                        }else{
                            cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                        }
                    }else{
                        cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                    }
                }
            }else{
                //Esto quiere decir que es la ultima particion del disco
                //Diferencia entre el size del disco y la posicion final de la particion
                int diferencia =  master.mbr_tamano - (master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start);
                if (diferencia != 0){
                    //Quiere decir que si hay diferencia, que hay algo de espacio libre
                    if(diferencia >= size){
                        //Si hay espacio para agregar, entonces agregamos
                        master.mbr_partitiones[num].part_size = (master.mbr_partitiones[num].part_size - size);
                        fseek(archivo, 0, SEEK_SET);
                        fwrite(&master, sizeof(MBR), 1 , archivo);
                        cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                    }else{
                        cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                    }
                }else{
                    cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                }
            }
        }else{
            //Esto quiere decir que tenemos que eliminar espacio en vez de agregar
            if(-(size) >= master.mbr_partitiones[num].part_size){
                cout << "[Error] > La cantidad que desea quitar excede al size de la particion" << endl;
            }else{
                //Quiere decir que si existe espacio, solo actualizamos la particion en el mbr
                master.mbr_partitiones[num].part_size = (master.mbr_partitiones[num].part_size + size);
                fseek(archivo, 0, SEEK_SET);
                fwrite(&master, sizeof(MBR), 1, archivo);
                cout << "[Success] > Se quito memoria a la particion" << endl;
            }
        }
    }
    //  SI ES PARTICION EXTENDIDA
    else if(is_partition_extendida(master, name)){
        //Actualizamos espacio dentro de la particion extendida
        int num = get_partition_primaria(master, name);
        if (agregar == true){
            //Tenemos que agregar espacio a la particion
            bool esUltimaParticion = num == 3 ? true : false;
            if(!esUltimaParticion){
                //ParticionSiguiente.start - posicionFinalDeLaParticionActual = diferencia entre una particion y la siguiente 
                int diferencia = master.mbr_partitiones[num+1].part_start - (master.mbr_partitiones[num].part_start + master.mbr_partitiones[num].part_size);
                if(diferencia != 0){
                    //Quiere decir que si existe una diferencia de espacio entre esta particion y la siguiente
                    if(diferencia >= size){
                        //Quiere decir que si hay espacio sufiente para aumentar la particion primaria
                        master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size;
                        fseek(archivo, 0, SEEK_SET);
                        fwrite(&master, sizeof(MBR), 1, archivo);
                        cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                    }else if(master.mbr_partitiones[num+1].part_status == '0' && master.mbr_partitiones[num+1].part_size == 0){
                        //Quiere decir que no es la ultima particion pero si la ultima que se ingreso
                        //size es menor o igual que el size que esta disponible (size del disco menos el final de esta particion)
                        if(size <= (master.mbr_tamano - (master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start))){
                            //Quiere decir que si hay espacio dentro del disco para sumar espacio a la particion
                            
                            //Sumamos el size a la particion y actualizamos el mbr
                            master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size;
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(MBR), 1, archivo);
                            cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                        }else{
                            cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                        }
                    }else{
                        cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                    }
                }else{
                    //Quiere decir que no hay diferencia entre una particion y la otra
                    //Revisamos si la siguiente particion fue eliminada
                    if(master.mbr_partitiones[num+1].part_status == '1'){
                        //Revisamos si cabe el espacio dentro de la particion siguiente eliminada
                        if(size <= master.mbr_partitiones[num+1].part_size){
                            //Si existe espacio, tenemos que actualizar ambas particiones
                            master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size; //Aumentamos el size
                            master.mbr_partitiones[num+1].part_start = master.mbr_partitiones[num+1].part_start + size; //Actualizamos el start de la siguiente
                            master.mbr_partitiones[num+1].part_size = master.mbr_partitiones[num+1].part_size - size; //Actualizamos el size de la siguiente
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(MBR), 1, archivo);
                            cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                        }else{
                            cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                        }
                    }else{
                        cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                    }
                }
            }else{
                //Esto quiere decir que es la ultima particion del disco
                //Diferencia entre el size del disco y la posicion final de la particion
                int diferencia =  master.mbr_tamano - (master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start);
                if (diferencia != 0){
                    //Quiere decir que si hay diferencia, que hay algo de espacio libre
                    if(diferencia >= size){
                        //Si hay espacio para agregar, entonces agregamos
                        master.mbr_partitiones[num].part_size = (master.mbr_partitiones[num].part_size - size);
                        fseek(archivo, 0, SEEK_SET);
                        fwrite(&master, sizeof(MBR), 1 , archivo);
                        cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                    }else{
                        cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                    }
                }else{
                    cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                }
            }
        }else{
            //Esto quiere decir que tenemos que eliminar espacio en vez de agregar
            if (-(size) >= master.mbr_partitiones[num].part_size){
                cout << "[Error] > La cantidad que desea quitar excede al size de la particion" << endl;
            }else{
                //Si existe espacio para eliminar dentro de la particion

                //Obtenemos el primer ebr de la particion extendida
                EBR Extendida;
                fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
                fread(&Extendida, sizeof(EBR), 1 , archivo);

                //Iteramos las particiones logicas
                while((ftell(archivo) < master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start) && Extendida.part_next !=-1){
                    //Solo obtenemos el ebr de la ultima particion logica
                    fseek(archivo, Extendida.part_next, SEEK_SET);
                    fread(&Extendida, sizeof(EBR), 1 , archivo);
                }
                //Obtenemos la posicion donde terminaria la particion en caso de que se le quitara el espacio
                int diferencia = (master.mbr_partitiones[num].part_start + master.mbr_partitiones[num].part_size) + size;
                if(diferencia > (Extendida.part_start + Extendida.part_size)){
                    //Quiere decir que si es posible quitar el pedazo a la particion extendida
                    //Actualizamos y escribimos el nuevo mbr
                    master.mbr_partitiones[num].part_size = master.mbr_partitiones[num].part_size + size;
                    fseek(archivo,0, SEEK_SET);
                    fwrite(&master, sizeof(MBR), 1, archivo);
                    cout << "[Success] > Se quito memoria a la particion" << endl;

                }else{
                    cout << "[Error] > No es posible quitar la memoria porque la particion logica se corromperia" << endl;
                }
            }
        }
    } 
    // ENTONCES ES UNA PARTICION LOGICA
    else {
        //Actualizamos espacio dentro de la particion logica
        int num = get_partition_extendida(master);
        int start_logica = get_partition_logica(name, path);
        int memoria_usada = get_memory_used_extendida(name, path);
        if(start_logica != -1){
            if (agregar){
                //Obtenemos el ebr de particion logica
                EBR Extendida;
                fseek(archivo,start_logica, SEEK_SET);
                fread(&Extendida, sizeof(EBR), 1, archivo);
                //Revisamos si no tiene siguiente particion logica y si cabe el espacio que estamos agregando
                if(Extendida.part_next == -1 && master.mbr_partitiones[num].part_size >= (memoria_usada + size)){
                    //Si tiene espacio, por lo que procedemos a escribir el nuevo espacio en el ebr
                    Extendida.part_size = Extendida.part_size + size;
                    //Escribimos el nuevo ebr
                    fseek(archivo, start_logica, SEEK_SET);
                    fwrite(&Extendida, sizeof(EBR), 1, archivo);
                    cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                } //Revisamos si hay espacio entre el la particion y la siguiente
                else if ((Extendida.part_next - (Extendida.part_start + Extendida.part_size)) >= size && Extendida.part_next != -1){ 
                    //Si existe espacio entre las dos particiones por lo que podemos aumentar el size
                    Extendida.part_size = Extendida.part_size + size;
                    //Actualizamos el ebr con el nuevo size
                    fseek(archivo, start_logica, SEEK_SET);
                    fwrite(&Extendida, sizeof(EBR), 1, archivo);
                    cout << "[Success] > Se agrego mas memoria a la particion" << endl;
                }else{
                    cout << "[Error] > No hay suficiente espacio a la derecha" << endl;
                }
            }else{
                //Esto quiere decir que tenemos que eliminar espacio dentro de la particion

                //Obtenemos el ebr de la particion
                EBR Extendida;
                fseek(archivo,start_logica, SEEK_SET);
                fread(&Extendida, sizeof(EBR), 1, archivo);
                if (-(size) >= Extendida.part_size){
                    cout << "[Error] > La cantidad que desea quitar excede al size de la particion" << endl;
                }else{
                    //Podemos quitar espacio de la particion
                    Extendida.part_size = Extendida.part_size + size;
                    //Actualizamos el ebr nuevamente
                    fseek(archivo, start_logica, SEEK_SET);
                    fwrite(&Extendida, sizeof(EBR), 1, archivo);
                    cout << "[Success] > Se quito memoria a la particion" << endl;
                }
            }
        }else{
            cout << "[Error] > No existe una particion con ese nombre" << endl;
        }
    }
    
    //Cerramos el archivo
    fclose(archivo);
}

void delete_partition(string name, string path, string type){
    //Obtenemos el mbr
    char buffer = '\0';
    FILE *archivo = fopen(path.c_str(),"rb+");
    MBR master;
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);

    //Validaciones
    if(find_partition_in_mount(name, path)) { cout << "[Error] > No se puede eliminar una particion montada" << endl; fclose(archivo); return; }
    if(!exist_partition(path, name, master)) { cout << "[Error] > No existe una particion con ese nombre" << endl; fclose(archivo); return; }

    //  SI LA PARTICION ES PRIMARIA
    if(is_partition_primaria(master, name)){
        //Primaria
        int num = get_partition_primaria(master, name);
        //Si la eliminacion es de tipo rapida
        if (type == "FAST"){
            //Solo hay que cambiar el status a deshabilitado y quitarle el nombre
            master.mbr_partitiones[num].part_status = '1';
            strcpy(master.mbr_partitiones[num].part_name, "");
            //Actualizamos el mbr
            fseek(archivo, 0, SEEK_SET);
            fwrite(&master, sizeof(MBR), 1, archivo);

            //Mensaje de confirmacion
            cout << "[Success] > Se elimino correctamente la particion primaria" << endl;
        }else{
            //La eliminacion es completa, por lo que tenemos que borrar la data

            //Actualizamos la particion
            master.mbr_partitiones[num].part_status = '1';
            strcpy(master.mbr_partitiones[num].part_name, "");

            //Borramos la data que se encuentre dentro de la particion
            fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
            fwrite(&buffer, 1, master.mbr_partitiones[num].part_size, archivo);

            //Actualizamos el disco
            fseek(archivo, 0, SEEK_SET);
            fwrite(&master, sizeof(MBR),1, archivo);

            //Mensaje de confimacion
            cout << "[Success] > Se elimino correctamente la particion primaria" << endl;
        }
    }
    //  SI LA PARTICION ES EXTENDIDA
    else if(is_partition_extendida(master, name)){
        int num = get_partition_extendida(master);
        //Extenedida
        //Si la eliminacion es de tipo rapida
        if (type == "FAST"){
            //Solo hay que cambiar el status a deshabilitado y quitarle el nombre
            master.mbr_partitiones[num].part_status = '1';
            master.mbr_partitiones[num].part_fit = '0';
            strcpy(master.mbr_partitiones[num].part_name, "");
            //Actualizamos el mbr
            fseek(archivo, 0, SEEK_SET);
            fwrite(&master, sizeof(MBR), 1, archivo);

            //Mensaje de confirmacion
            cout << "[Success] > Se elimino correctamente la particion " << endl;
        }else{
            //La eliminacion es completa, por lo que tenemos que borrar la data

            //Actualizamos la particion
            master.mbr_partitiones[num].part_status = '1';
            master.mbr_partitiones[num].part_fit = '0';
            strcpy(master.mbr_partitiones[num].part_name, "");

            //Borramos la data que se encuentre dentro de la particion
            fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
            fwrite(&buffer, 1, master.mbr_partitiones[num].part_size, archivo);

            //Actualizamos el disco
            fseek(archivo, 0, SEEK_SET);
            fwrite(&master, sizeof(MBR),1, archivo);

            //Mensaje de confimacion
            cout << "[Success] > Se elimino correctamente la particion " << endl;
        }
    }
    //  ENTONCES ES UNA PARTICION LOGICA
    else {
        //logica
        int num = get_partition_extendida(master);
        bool comprobar = false;

        //Obtenemos la particion logica
        EBR Extendida;
        fseek(archivo, master.mbr_partitiones[num].part_start, SEEK_SET);
        fread(&Extendida, sizeof(EBR), 1 , archivo);
        if (Extendida.part_size != 0){
            while ((ftell(archivo) < master.mbr_partitiones[num].part_size + master.mbr_partitiones[num].part_start)) {
                //Si la particion tiene el mismo nombre
                if (strcmp(Extendida.part_name, name.c_str()) == 0) break;
                //Si llegamos a la ultima particion
                if(Extendida.part_next == -1) break;
                fseek(archivo, Extendida.part_next, SEEK_SET);
                fread(&Extendida, sizeof(EBR), 1 , archivo);
            }
        }
        //Si el tipo de eliminacion es de manera fast
        if(type == "FAST"){
            //La variable extendida trae el ebr de la particion logica que tenemos
            Extendida.part_status = '1';
            strcpy(Extendida.part_name, "");

            //Actualizamos la particion con valores nulos
            fseek(archivo, ftell(archivo) - sizeof(EBR), SEEK_SET);
            fwrite(&Extendida, sizeof(EBR), 1, archivo);

            //Mensaje de confirmacion
            cout << "[Success] > Se elimino correctamente la particion logica" << endl;
        }else{
            //Es de tipo eliminacion completa
            //Cambiamos la logica a los valores nulos
            Extendida.part_status = '1';
            strcpy(Extendida.part_name, "");

            //Escribimos el 0 dentro de toda la particion logica
            fseek(archivo, ftell(archivo)-sizeof(EBR), SEEK_SET);
            fwrite(&buffer, sizeof(EBR), 1, archivo);

            //Actualizamos el ebr de la particion logica eliminada
            fseek(archivo, ftell(archivo)-sizeof(EBR), SEEK_SET);
            fwrite(&Extendida, sizeof(EBR), 1, archivo);

            //Mensaje de confirmacion
            cout << "[Success] > Se elimino correctamente la particion logica" << endl;
        }
    }

    //Cerramos el archivo
    fclose(archivo);
}


/*******************************  FUNCION RAIZ QUE SE ENCARGA DE VERIFICAR QUE ACCION HACER Y EJECUTARLAS ***********************************/
void fdisk::make_fdisk(fdisk *particion){
    //Validaciones
    if(particion->path == "") { cout << "[Error] > No se ingreso el parametro de $path" << endl; return;}
    if(particion->nombre == "") { cout << "[Error] > No se ingreso el parametro de $name" << endl; return;}
    FILE *file;
    bool usaraRaid = false;
    string copyPath = get_path_raid(particion->path);
    if (!(file = fopen(particion->path.c_str(), "r"))) { 
        particion->path = copyPath;
        usaraRaid = true;
    } else fclose(file);
    if(usaraRaid){
        if(!(file = fopen(particion->path.c_str(), "r"))) {
            cout << "[Error] > No se ha encontrado el disco" << endl; 
            return;
        } else fclose(file);
    }

    //Seteamos el tamanio de la particion y tambien el tamanio que agregaria o quitaria
    int size = 0;
    int add = 0;
    transform(particion->unidad.begin(), particion->unidad.end(), particion->unidad.begin(), :: tolower);
    if (particion->unidad == "k" || particion->unidad == ""){
        size = particion->size;
        add = particion->agregar;
    }else if (particion->unidad == "b"){
        size = particion->size/1000;
        add = particion->agregar/1000;
    }else if (particion->unidad == "m"){
        size = particion->size*1000;
        add = particion->agregar*1000;
    }else{
        cout << "[Error] > El valor de @unit no es permitido" << endl;
        return;
    }

    //Seteamos el ajuste de la particion
    char fit ;
    transform(particion->ajuste.begin(), particion->ajuste.end(), particion->ajuste.begin(), :: tolower);
    if (particion->ajuste == "bf") fit = 'B';
    else if (particion->ajuste == "ff") fit = 'F';
    else if (particion->ajuste == "wf" || particion->ajuste == "") fit = 'W';
    else {cout << "[Error] > El valor de @fit no es permitido" << endl; return;} 

    //Verificamos si hay que agregar o eliminar espacio a la particion
    if(particion->agregar != 0){
        //Agregamos o eliminamos espacio de la particion
        update_partition(particion->nombre, particion->path, add);
        return;
    }
    if(particion->eliminar != ""){
        //Eliminamos la particion como tal
        delete_partition(particion->nombre, particion->path, particion->eliminar);
        return;
    }
    if(particion->mov){
        //Solo imprimimos que vino un parametro de mov
        cout << "[Info] > Comando de mov detectado" << endl;
    }

    //Si no agregamos o eliminamos anteriormente, por ende tenemos que crear la particion
    if (particion->tipo == "P" || particion->tipo == ""){
        //Creamos una particion primaria
        make_primaria(particion->nombre, particion->path, size, fit);
    }else if (particion->tipo == "E"){
        //Creamos una particion extendida
        make_extendida(particion->nombre, particion->path, size, fit);
    }else if (particion->tipo == "L"){
        //Creamos una particion logica
        make_logica(particion->nombre, particion->path, size, fit);
    }

    //Realizamos una copia del disco
    if(!usaraRaid){
        string path_copy = get_path_raid(particion->path);
        string cmd = "sudo cp \"" + particion->path + "\" \"" + path_copy + "\"";
        system(cmd.c_str());
    }
}