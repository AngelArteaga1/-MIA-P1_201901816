
#include "fdisk.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "../estructura/estructura.h"

fdisk::fdisk(){ }

/*
int buscParticion(string nombre, string path){
    FILE *archivo;
    Disco master;
    if(archivo = fopen(path.c_str(),"rb+")){
        int NParticionE = -1;
        fseek(archivo, 0, SEEK_SET);
        fread(&master, sizeof(Disco), 1, archivo);
        for (int i = 0; i < 4; i++) {
            if (master.particiones[i].p_type == 'E'){
                NParticionE = i;
                break;
            }
        }
        if(NParticionE != -1){
            ext Extendida;
            fseek(archivo, master.particiones[NParticionE].p_comienzo, SEEK_SET);
            fread(&Extendida, sizeof(ext), 1 , archivo);
            while ((ftell(archivo)<master.particiones[NParticionE].p_tam + master.particiones[NParticionE].p_comienzo)) {
                if (strcmp(Extendida.e_nombre,nombre.c_str())==0){
                    //cout<<Extendida.e_comienzo;
                    return (ftell(archivo)-sizeof(ext));
                }
                if(Extendida.e_siguiente == -1){
                    break;
                }
                fseek(archivo, Extendida.e_siguiente, SEEK_SET);
                fread(&Extendida, sizeof(ext), 1 , archivo);
            }
        }
        fclose(archivo);
    }
    return -1;
}

int tamLogica(string nombre, string path){
    FILE *archivo;
    Disco master;
    if(archivo = fopen(path.c_str(),"rb+")){
        int NParticionE = -1;
        fseek(archivo, 0, SEEK_SET);
        fread(&master, sizeof(Disco), 1, archivo);
        for (int i = 0; i < 4; i++) {
            if (master.particiones[i].p_type == 'E'){
                NParticionE = i;
                break;
            }
        }
        if(NParticionE != -1){
            ext Extendida;
            int tam =0;
            fseek(archivo, master.particiones[NParticionE].p_comienzo, SEEK_SET);
            fread(&Extendida, sizeof(ext), 1 , archivo);
            while ((ftell(archivo)<master.particiones[NParticionE].p_tam + master.particiones[NParticionE].p_comienzo)) {
                tam += Extendida.e_tam;
                if(Extendida.e_siguiente == -1){
                    return tam;
                }
                fseek(archivo, Extendida.e_siguiente, SEEK_SET);
                fread(&Extendida, sizeof(ext), 1 , archivo);
            }
        }
        fclose(archivo);
    }
    return -1;
}




/*

void AgregarParticion(string path, string nombre, int sizebyte){
    FILE *archivo;
    Disco master;

    string tipo = "Nulo";
    if(sizebyte > 0){
        tipo = "agregar";
    }

    if(archivo=fopen(path.c_str(), "rb+")){
        bool montado = ListaMontado(path, nombre);
        if(!montado){
            bool Exparticion = false;
            int NParticion = -1;
            int NParticionE = -1;
            fseek(archivo, 0, SEEK_SET);
            fread(&master, sizeof(Disco),1 , archivo);
            for (int i = 0; i < 4; i++) {
                //cout << master.particiones[i].p_type<<endl;
                if(strcmp(master.particiones[i].p_nombre,nombre.c_str())==0){
                    if(master.particiones[i].p_type=='E'){
                        Exparticion = true;
                    }
                    NParticion = i;
                    break;
                }else if(master.particiones[i].p_type =='E'){
                    Exparticion = true;
                    NParticionE = i;
                }
            }
            if(NParticion != -1){
                if(!Exparticion){
                    if (tipo == "agregar"){
                        if(NParticion != 3){
                            int comprobar = master.particiones[NParticion+1].p_comienzo - (master.particiones[NParticion].p_comienzo + master.particiones[NParticion].p_tam);
                            if(comprobar != 0){
                                if(comprobar>=sizebyte){
                                    master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                    fseek(archivo, 0, SEEK_SET);
                                    fwrite(&master, sizeof(Disco), 1, archivo);
                                    printf("Se agrego mas memoria a la particion\n");
                                }else if(master.particiones[NParticion+1].p_estado == '0' && master.particiones[NParticion+1].p_tam == 0){
                                    if(sizebyte<= (master.d_tam-(master.particiones[NParticion].p_tam+master.particiones[NParticion].p_comienzo))){
                                        master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                        fseek(archivo, 0, SEEK_SET);
                                        fwrite(&master, sizeof(Disco), 1, archivo);
                                        printf("Se agrego mas memoria a la particion\n");
                                    }else{
                                        printf("Error no hay suficiente espacio a la derecha\n");
                                    }
                                }else{
                                    printf("Error no hay suficiente espacio a la derecha\n");
                                }
                            }else{
                                if(master.particiones[NParticion+1].p_estado == '1'){
                                    if(sizebyte<= master.particiones[NParticion+1].p_tam){
                                        master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                        master.particiones[NParticion+1].p_comienzo =master.particiones[NParticion+1].p_comienzo + sizebyte;
                                        master.particiones[NParticion+1].p_tam = master.particiones[NParticion+1].p_tam - sizebyte;
                                        fseek(archivo, 0, SEEK_SET);
                                        fwrite(&master, sizeof(Disco), 1, archivo);
                                        printf("Se agrego mas memoria a la particion\n");
                                    }else{
                                        printf("Error no hay suficiente espacio a la derecha\n");
                                    }
                                }else{
                                    printf("Error no hay suficiente espacio a la derecha\n");
                                }

                            }
                        }else{
                            int comprobar =  (master.d_tam + (int)sizeof(Disco)) - (master.particiones[NParticion].p_tam+master.particiones[NParticion].p_comienzo);
                            if (comprobar!=0){
                                if(comprobar>= sizebyte){
                                    master.particiones[NParticion].p_tam = (master.particiones[NParticion].p_tam - sizebyte);
                                    fseek(archivo, 0, SEEK_SET);
                                    fwrite(&master, sizeof(Disco), 1 , archivo);
                                    printf("Se agrego mas memoria a la particion\n");
                                }else{
                                    printf("Error no hay suficiente espacio a la derecha\n");
                                }
                            }else{
                                printf("Error no hay suficiente espacio a la derecha\n");
                            }
                        }
                    }else{
                        if(-(sizebyte) >= master.particiones[NParticion].p_tam){
                            printf("Error la cantidad que desea quitar excede al size de la particion\n");
                        }else{
                            master.particiones[NParticion].p_tam = (master.particiones[NParticion].p_tam + sizebyte);
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(Disco), 1, archivo);
                            printf("Se quito memoria a la particion\n");
                        }
                    }
                }else{
                    if(tipo == "agregar"){
                        if(NParticion !=3){
                            int comprobar = master.particiones[NParticion+1].p_comienzo - (master.particiones[NParticion].p_comienzo+master.particiones[NParticionE].p_tam);
                            if(comprobar !=0){
                                if (comprobar>= sizebyte){
                                    master.particiones[NParticion].p_tam= master.particiones[NParticion].p_tam + sizebyte;
                                    fseek(archivo, 0, SEEK_SET);
                                    fwrite(&master, sizeof(Disco), 1, archivo);
                                    printf("Se agrego mas memoria a la particion\n");
                                }else if(master.particiones[NParticion+1].p_estado == '0' && master.particiones[NParticion+1].p_tam == 0){
                                    if(sizebyte<= (master.d_tam-(master.particiones[NParticion].p_tam+master.particiones[NParticion].p_comienzo))){
                                        master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                        fseek(archivo, 0, SEEK_SET);
                                        fwrite(&master, sizeof(Disco), 1, archivo);
                                        printf("Se agrego mas memoria a la particion\n");
                                    }else{
                                        printf("Error no hay suficiente espacio a la derecha\n");
                                    }
                                }else{
                                    printf("Error no hay suficiente espacio a la derecha\n");
                                }
                            }else{
                                if(master.particiones[NParticion+1].p_estado =='1'){
                                    if(sizebyte<=master.particiones[NParticion+1].p_tam){
                                        master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                        master.particiones[NParticion+1].p_comienzo = master.particiones[NParticion+1].p_comienzo + sizebyte;
                                        master.particiones[NParticion+1].p_tam = master.particiones[NParticion+1].p_tam - sizebyte;
                                        fseek(archivo,0, SEEK_SET);
                                        fwrite(&master, sizeof(Disco), 1, archivo);
                                        printf("Se agrego mas memoria a la particion\n");
                                    }else{
                                        printf("Error no hay suficiente espacio a la derecha\n");
                                    }
                                }else{
                                    printf("Error no hay suficiente espacio a la derecha\n");
                                }
                            }
                        }else{
                            int comprobar = (master.d_tam + (int)sizeof(Disco)) - (master.particiones[NParticion].p_tam+master.particiones[NParticion].p_comienzo);
                            if (comprobar !=0){
                                if(comprobar >= sizebyte){
                                    master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                    fseek(archivo, 0, SEEK_SET);
                                    fwrite(&master, sizeof(Disco), 1 , archivo);
                                    printf("Se agrego mas memoria a la particion\n");
                                }else{
                                    printf("Error no hay suficiente espacio a la derecha\n");
                                }
                            }else{
                                printf("Error no hay suficiente espacio a la derecha\n");
                            }
                        }
                    }else{
                        if (sizebyte >= master.particiones[NParticionE].p_tam){
                            printf("Error la cantidad que desea quitar excede al size de la particion\n");
                        }else{
                            ext Extendida;
                            fseek(archivo, master.particiones[NParticion].p_comienzo, SEEK_SET);
                            fread(&Extendida, sizeof(ext), 1 , archivo);
                            while((ftell(archivo)<master.particiones[NParticion].p_tam+master.particiones[NParticion].p_comienzo)&&Extendida.e_siguiente !=-1){
                                fseek(archivo, Extendida.e_siguiente, SEEK_SET);
                                fread(&Extendida, sizeof(ext), 1 , archivo);
                            }
                            int comprobar = (master.particiones[NParticion].p_comienzo+master.particiones[NParticion].p_tam) + sizebyte;
                            if(comprobar>(Extendida.e_comienzo+Extendida.e_tam)){
                                master.particiones[NParticion].p_tam = master.particiones[NParticion].p_tam + sizebyte;
                                fseek(archivo,0, SEEK_SET);
                                fwrite(&master, sizeof(Disco), 1, archivo);
                                printf("Se quito memoria a la particion\n");
                            }else{
                                printf("Error porque la particion logica se corromperia\n");
                            }
                        }
                    }
                }
            }else{
                if(NParticionE != -1){
                    int buscar = buscParticion(nombre, path);
                    int total = tamLogica(nombre, path);
                    if(buscar != -1){
                        if (tipo == "agregar"){
                            ext Extendida;
                            fseek(archivo,buscar, SEEK_SET);
                            fread(&Extendida, sizeof(ext), 1, archivo);
                            //cout<< master.particiones[NParticionE].p_tam<<endl;
                            if(Extendida.e_siguiente == -1 && master.particiones[NParticionE].p_tam >= (total+sizebyte)){
                                Extendida.e_tam = Extendida.e_tam + sizebyte;
                                fseek(archivo, buscar, SEEK_SET);
                                fwrite(&Extendida, sizeof(ext), 1, archivo);
                                printf("Se agrego mas memoria a la particion\n");

                            }else if ((Extendida.e_siguiente-(Extendida.e_comienzo+Extendida.e_tam))>= sizebyte && Extendida.e_siguiente != -1){
                                Extendida.e_tam = Extendida.e_tam + sizebyte;
                                fseek(archivo, buscar, SEEK_SET);
                                fwrite(&Extendida, sizeof(ext), 1, archivo);
                                printf("Se agrego mas memoria a la particion\n");
                            }else{
                                printf("Error no hay suficiente espacio a la derecha\n");
                            }
                        }else{
                            ext Extendida;
                            fseek(archivo,buscar, SEEK_SET);
                            fread(&Extendida, sizeof(ext), 1, archivo);
                            if (-(sizebyte) >= Extendida.e_tam){
                                printf("Error porque la particion logica se corromperia\n");
                            }else{
                                //cout<< Extendida.e_tam<<endl;
                                Extendida.e_tam = Extendida.e_tam + sizebyte;
                                fseek(archivo, buscar, SEEK_SET);
                                fwrite(&Extendida, sizeof(ext), 1, archivo);
                                printf("Se quito memoria a la particion\n");
                            }
                        }
                    }else{
                        printf("Error no se encontro la particion con el mismo nombre\n");
                    }
                }else{
                    printf("Error no se encontro la particion con el mismo nombre\n");
                }
            }
        }else{
            printf("Error la particion se encuentra montada\n");
        }
        fclose(archivo);
    }else{
        printf("Error no se encontro el disco\n");
    }

}

void EliminarParticion(string path, string nombre, string tipo){
    FILE *archivo;
    Disco master;

    if(archivo = fopen(path.c_str(),"rb+")){
        bool montado = ListaMontado(path, nombre);
        if(!montado){
            int NParticion = -1;
            int NParticionE = -1;
            char buffer = '\0';
            bool Exparticion = false;
            fseek(archivo, 0, SEEK_SET);
            fread(&master, sizeof(Disco), 1, archivo);
            for (int i = 0; i < 4; i++) {
                if(strcmp(master.particiones[i].p_nombre,nombre.c_str())==0){
                    NParticion = i;
                    if(master.particiones[i].p_type=='E'){
                        Exparticion = true;
                    }
                    break;
                }else if (master.particiones[i].p_type == 'E'){
                    NParticionE = i;
                }
            }

            cout << "Estas seguro de eliminar la particion S/N > ";
            string confirmacion;
            getline(cin, confirmacion);
            if (confirmacion == "S" | confirmacion == "s"){
                if (NParticion != -1){
                    if(!Exparticion){
                        if (tipo == "FAST"){
                            master.particiones[NParticion].p_estado = '1';
                            strcpy(master.particiones[NParticion].p_nombre, "");
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(Disco), 1, archivo);
                            printf("Se elimino correctamente la particion primaria\n");
                        }else{
                            master.particiones[NParticion].p_estado = '1';
                            strcpy(master.particiones[NParticion].p_nombre, "");

                            fseek(archivo, master.particiones[NParticion].p_comienzo, SEEK_SET);
                            fwrite(&buffer, 1, master.particiones[NParticion].p_tam,archivo);

                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(Disco),1, archivo);


                            printf("Se elimino correctamente la particion primaria\n");
                        }
                    }else{
                        if(tipo=="FAST"){
                            master.particiones[NParticion].p_estado = '1';
                            master.particiones[NParticion].p_type='0';
                            strcpy(master.particiones[NParticion].p_nombre, "");
                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(Disco), 1, archivo);
                            printf("Se elimino correctamente la particion extendida\n");
                        }else{
                            master.particiones[NParticion].p_estado = '1';
                            master.particiones[NParticion].p_type='0';
                            strcpy(master.particiones[NParticion].p_nombre, "");

                            fseek(archivo, master.particiones[NParticion].p_comienzo, SEEK_SET);
                            fwrite(&buffer, 1, master.particiones[NParticion].p_tam,archivo);

                            fseek(archivo, 0, SEEK_SET);
                            fwrite(&master, sizeof(Disco),1, archivo);

                            printf("Se elimino correctamente la particion extendida\n");
                        }
                    }
                }else{
                    if(NParticionE != -1){
                        bool comprobar = false;
                        ext Extendida;
                        fseek(archivo, master.particiones[NParticionE].p_comienzo, SEEK_SET);
                        fread(&Extendida, sizeof(ext), 1 , archivo);
                        if (Extendida.e_tam != 0){
                            while ((ftell(archivo)<master.particiones[NParticionE].p_tam + master.particiones[NParticionE].p_comienzo)) {
                                if (strcmp(Extendida.e_nombre,nombre.c_str())==0){
                                    comprobar = true;
                                    break;
                                }
                                if(Extendida.e_siguiente == -1){
                                    break;
                                }
                                fseek(archivo, Extendida.e_siguiente, SEEK_SET);
                                fread(&Extendida, sizeof(ext), 1 , archivo);
                            }
                        }
                        if (comprobar){
                            if(tipo=="FAST"){
                                Extendida.e_estado = '1';
                                strcpy(Extendida.e_nombre, "");

                                //cout<<Extendida.e_siguiente<<endl;

                                fseek(archivo, ftell(archivo)-sizeof(ext), SEEK_SET);
                                fwrite(&Extendida, sizeof(ext), 1, archivo);
                                //cout<<Extendida.e_tam<<endl;

                                printf("Se elimino correctamente la particion logica\n");
                            }else{
                                Extendida.e_estado = '1';
                                strcpy(Extendida.e_nombre, "");

                                fseek(archivo, ftell(archivo)-sizeof(ext), SEEK_SET);
                                fwrite(&buffer, sizeof(ext), 1, archivo);

                                fseek(archivo, ftell(archivo)-sizeof(ext), SEEK_SET);
                                fwrite(&Extendida, sizeof(ext), 1, archivo);

                                printf("Se elimino correctamente la particion extendida\n");
                            }
                        }else{
                            printf("Error no se encontro la particion con el mismo nombre\n");
                        }
                    }else{
                        printf("Error no se encontro la particion con el mismo nombre\n");
                    }
                }
            }else{
                printf("Se cancelo la eliminacion de la parcicion\n");
            }
        }else{
            printf("Error la particion se encuentra montada\n");
        }
        fclose(archivo);
    }else{
        printf("Error no se encontro el disco\n");
    }
}*/

bool exist_partition(string path, string name, MBR master){
    FILE *archivo;
    //Iteramos las particiones
    for (int i = 0; i < 4; ++i) {
        //Verificamos si existe el nombre en una particion primaria
        //cout << "nombre de la particion: " << master.mbr_partitiones[i].part_name << ", type: " << master.mbr_partitiones[i].part_type << endl;
        if((strcmp(name.c_str(), master.mbr_partitiones[i].part_name)) == 0){
            fclose(archivo);
            return true;
        }else if (master.mbr_partitiones[i].part_type == 'E'){
            //Verificamos si la particion no existe en una particion extendida
            EBR sub;
            int extendida = i;
            //Ponemos el puntero al inicio de la particion extendida
            fseek(archivo,master.mbr_partitiones[extendida].part_start, SEEK_SET);
            int posicionFinal = master.mbr_partitiones[extendida].part_size + master.mbr_partitiones[extendida].part_start;
            //Recorremos todas las particiones logicas
            while((fread(&sub, sizeof(EBR),1, archivo)) != 0 && (ftell(archivo) < posicionFinal)){
                if(strcmp(sub.part_name, name.c_str()) == 0){
                    fclose(archivo);
                    return true;
                }
                if(sub.part_next == -1){
                    fclose(archivo);
                    return false;
                }
            }
            fclose(archivo);
        }
    }
    return false;
}

bool exist_partition_extendida(MBR master){
    for (int i = 0; i < 4; i++)
        if(master.mbr_partitiones[i].part_type == 'E') return true;
    return false;
}

int get_partition_extendida(MBR master){
    for (int i = 0; i < 4; i++)
        if(master.mbr_partitiones[i].part_type == 'E') return i;
    return -1;
}

int get_espacio_usado(MBR master){
    int espacio = 0; //Kilobyte
    for (int i = 0; i < 4; i++) {
        if (master.mbr_partitiones[i].part_status != '1'){
            espacio += master.mbr_partitiones[i].part_size;
        }
    }
    espacio += (int)sizeof(MBR);
    return espacio;
}

bool is_posible_write_partition(MBR master, int size){
    for (int i = 0; i < 4; i++) {
        //Verificamos si tenemos un espacio libre
        if(master.mbr_partitiones[i].part_start == -1) return true;
        //Verificamos si existe un espacio donde hubo una particion anteriormente y cabe en este espacion
        if(master.mbr_partitiones[i].part_status == '1' && master.mbr_partitiones[i].part_size >= size) return true;
    }
    return false;
}

int get_num_partition_free(MBR master, int size){
    for (int i = 0; i < 4; i++) {
        //Verificamos si tenemos un espacio libre
        if(master.mbr_partitiones[i].part_start == -1) return i;
        //Verificamos si existe un espacio donde hubo una particion anteriormente y cabe en este espacion
        if(master.mbr_partitiones[i].part_status == '1' && master.mbr_partitiones[i].part_size >= size) return i;
    }
    return -1;
}

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
        int EspacioUse = get_espacio_usado(master);

        //VALIDACIONES
        //Verificamos si si podemos usar el espacio
        if((master.mbr_tamano-EspacioUse) < size){ cout << "[Error] > No hay suficiente espacio libre" << endl; fclose(archivo); return; }
        if(exist_partition(path, name, master)){ cout << "[Error] > Ya existe una particion con el mismo nombre " << endl; fclose(archivo); return;}

        /**************** LLENAMOS LOS DATOS DE LA PARTICION *****************/
        master.mbr_partitiones[num].part_type = 'P';
        master.mbr_partitiones[num].part_fit = fit;
        //Obtenemos la posicion donde comienza la particion, si es primera o si no
        if(num == 0) master.mbr_partitiones[num].part_start = sizeof(master); 
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
        int EspacioUse = get_espacio_usado(master);

        //VALIDACIONES
        //Verificamos si si podemos usar el espacio
        if((master.mbr_tamano-EspacioUse) < size){ cout << "[Error] > No hay suficiente espacio libre" << endl; fclose(archivo); return; }
        if(exist_partition(path, name, master)){ cout << "[Error] > Ya existe una particion con el mismo nombre " << endl; fclose(archivo); return;}

        /**************** LLENAMOS LOS DATOS DE LA PARTICION *****************/
        master.mbr_partitiones[num].part_type = 'E';
        master.mbr_partitiones[num].part_fit = fit;
        //Obtenemos la posicion donde comienza la particion, si es primera o si no
        if(num==0) master.mbr_partitiones[num].part_start = sizeof(master);
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
    if(!exist_partition_extendida(master)){ cout << "[Error] > No existe una particion extendida en el disco" << endl; fclose(archivo); return;}
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
            cout << "[Error] > No hay suficiente espacio" << endl;
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
                //Actualizamos el actual ebr (el ultimo)
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

void fdisk::make_fdisk(fdisk *particion){
    //Validaciones
    if(particion->path == "") { cout << "[Error] > No se ingreso el parametro de $path" << endl; return;}
    if(particion->nombre == "") { cout << "[Error] > No se ingreso el parametro de $name" << endl; return;}
    
    FILE *file;
    const char * ruta = particion->path.c_str();
    if (!(file = fopen(ruta, "r"))) { cout << "[Error] > No se ha encontrado el disco" << endl; return;} else {fclose(file);}

    //Seteamos el tamanio de la particion y tambien el tamanio que agregaria o quitaria
    int size = 0;
    int add = 0;
    if (particion->unidad=="K" || particion->unidad == ""){
        size = particion->size;
        add = particion->agregar;
    }else if (particion->unidad == "B"){
        size = particion->size/1000;
        add = particion->agregar/1000;
    }else if (particion->unidad=="M"){
        size = particion->size*1000;
        add = particion->agregar*1000;
    }else{
        cout << "[Error] > El valor de @unit no es permitido" << endl;
        return;
    }

    //Seteamos el ajuste de la particion
    char fit ;
    if (particion->ajuste == "BF") fit = 'B';
    else if (particion->ajuste == "FF") fit = 'F';
    else if (particion->ajuste == "WF" || particion->ajuste == "") fit = 'W';
    else cout << "[Error] > El valor de @fit no es permitido" << endl;

    //Verificamos si hay que agregar o eliminar espacio a la particion
    if(particion->agregar != 0){
        //Agregamos o eliminamos espacion de la particion
        //AgregarParticion(particion->path, particion->nombre, add);
        return;
    }
    if(particion->eliminar != ""){
        //Eliminamos la particion como tal
        //EliminarParticion(particion->path, particion->nombre, particion->eliminar);
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
}