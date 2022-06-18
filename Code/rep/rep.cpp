#include "rep.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include "../estructura/estructura.h"
#include "../listadoble/nodomount.h"
#include "../estructura/estructura.h"
#include "../mount/mount.h"
#include "../analizador/analizador.h"
#include "../fdisk/fdisk.h"

rep::rep(){ }

std::ofstream outputfile;

/*

void comprobarRuta(string path){
    bool comprobar = true;
    string Carpeta = path;
    while(comprobar){
        Carpeta = Carpeta.substr(0, Carpeta.size()-1);
        char aux = Carpeta.back();
        if (aux=='/'){
            Carpeta = Carpeta.substr(0, Carpeta.size()-1);
            comprobar = false;
        }
    }
    //Verificamos si la ruta existe, si no creamos las carpetas
    string rutaCarpeta = Carpeta;
    if( access( rutaCarpeta.c_str(), F_OK ) == -1 ) {
        string cmd = "mkdir -p \"" + rutaCarpeta + "\"";
        system(cmd.c_str());
     }
}









void crearINODE(string path, string id){
    bool comprobar =false;
    nodoMount *aux = ListaM->primero;
    if (aux->id == id){
        comprobar = true;
    }else{
        while (aux != nullptr){
            if (aux->id == id){
                comprobar = true;
                break;
            }
            aux = aux->sig;
        }
    }
    if (comprobar){
        FILE *archivo;
        if(archivo =fopen((aux->path).c_str(),"rb+")){
            Disco master;
            int NParticion = -1;
            fseek(archivo, 0, SEEK_SET);
            fread(&master, sizeof(Disco), 1, archivo);

            ofstream file;
            comprobarRuta(path);
            file.open("./reportes/reporte.dot",ios::out);
            if(file.fail()){
                cout<<"Error no se pudo abrir el archivo";
                exit(1);
            }


            for (int i = 0; i < 4; i++) {
                if (strcmp(master.particiones[i].p_nombre, (aux->nombre).c_str())==0){
                    NParticion = i;
                    break;
                }
            }
            if(NParticion != -1){
                file<<"digraph G{\n"<<"node[shape=plaintext]\n"<<"concentrate=true\n"<<"ReporteINODE[label=<\n";
                file<<"<TABLE BORDER=\"2\" CELLBORDER=\"1\" CELLSPACING=\"2\" >\n";
                file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">Index</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">Tipo</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">Nombre</TD>\n"<<"</TR>\n";

                SuperBloque super;
                BloqueCarpeta block;
                TablaInodo inodo;
                fseek(archivo, master.particiones[NParticion].p_comienzo, SEEK_SET);
                fread(&super, sizeof(TablaInodo), 1, archivo);

                fseek(archivo, super.s_block_start, SEEK_SET);
                fread(&block, sizeof(BloqueCarpeta), 1, archivo);
                fseek(archivo, super.s_inode_start, SEEK_SET);
                fread(&inodo, sizeof(TablaInodo), 1, archivo);
                int n = 0;
                int inodeS = super.s_inode_start;
                string tipo = "";
                while(block.b_content[n+1].b_inodo!=-1){
                    if (inodo.i_type == '0'){
                        tipo = "Carpeta";
                    }else if (inodo.i_type == '1'){
                        tipo = "Archivo";
                    }
                    string nombre = "";
                    nombre = block.b_content[n+1].b_name;
                    if (nombre == ".."){
                        nombre = "";
                    }
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">"+to_string(block.b_content[n+1].b_inodo)+"</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+tipo+"</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">/"+nombre+"</TD>\n"<<"</TR>\n";
                    inodeS += int(sizeof(TablaInodo));
                    fseek(archivo, inodeS, SEEK_SET);
                    fread(&inodo, sizeof(TablaInodo), 1, archivo);
                    n++;
                }
                file<<"</TABLE>>]\n}";
                file.close();

                string extension = "dot -T" +comprobarExtension(path)+ " ./reportes/reporte.dot -o "+path;
                system(extension.c_str());
                fclose(archivo);

                if(archivo= fopen(path.c_str(), "rb+")){
                    printf("Se creo correctamente el reporte Inode\n");
                    fclose(archivo);
                }else{
                    printf("Error al crear el reporte Inode\n");
                }

            }else{
                printf("Error no se encontro la particion\n");
            }
        }else{
            printf("Error no se encontro el disco\n");
        }
    }else{
        printf("Error no se encontro el mismo id montada\n");
    }
}

void crearBLOCK(string path, string id){
    bool comprobar =false;
    nodoMount *aux = ListaM->primero;
    if (aux->id == id){
        comprobar = true;
    }else{
        while (aux != nullptr){
            if (aux->id == id){
                comprobar = true;
                break;
            }
            aux = aux->sig;
        }
    }

    if (comprobar){
        FILE *archivo;
        if(archivo =fopen((aux->path).c_str(),"rb+")){
            Disco master;
            int NParticion = -1;
            int NParticionE = -1;
            fseek(archivo, 0, SEEK_SET);
            fread(&master, sizeof(Disco), 1, archivo);

            ofstream file;
            comprobarRuta(path);
            file.open("./reportes/reporte.dot",ios::out);
            if(file.fail()){
                cout<<"Error no se pudo abrir el archivo";
                exit(1);
            }
            for (int i = 0; i < 4; i++) {
                if (strcmp(master.particiones[i].p_nombre, (aux->nombre).c_str())==0){
                    NParticion = i;
                    break;
                }
                if (master.particiones[i].p_type =='E'){
                    NParticionE = i;
                }
            }
            if(NParticion != -1){
                file<<"digraph G{\n"<<"node[shape=box]\n"<<"concentrate=true\n"<<"rankdir=\"LR\"\n";
                BloqueArchivo blockFile;
                BloqueCarpeta blockCarpet;
                SuperBloque super;
                fseek(archivo, master.particiones[NParticion].p_comienzo, SEEK_SET);
                fread(&super, sizeof(SuperBloque), 1, archivo);


                int contador = 0;
                int aux = super.s_bm_block_start;
                char buffer;

                while(aux<super.s_inode_start){
                    fseek(archivo, super.s_bm_block_start+contador, SEEK_SET);
                    buffer = char(fgetc(archivo));
                    aux++;

                    if(buffer== '1'){
                        fseek(archivo, super.s_block_start+64*contador, SEEK_SET);
                        fread(&blockCarpet, 64, 1 , archivo);
                        file<< "nodo"+to_string(contador)+"[label=<\n<TABLE BORDER =\"0\">\n";
                        file<<"<TR>\n<TD COLSPAN=\"2\">Bloque Carpeta "+to_string(contador)+"</TD>\n</TR>\n";
                        file<<"<TR>\n<TD>b_name</TD><TD>b_inodo</TD>\n</TR>\n";
                        /*for (int i = 0; i < 4; i++) {
                            file<<
                        }*
                        int n = 0;
                        while(blockCarpet.b_content[n].b_inodo != -1){
                            string nombre = "";
                            nombre = blockCarpet.b_content[n].b_name;
                            //cout<<nombre<<endl;
                            file<<"<TR>\n<TD>"+nombre+"</TD><TD>"+to_string(blockCarpet.b_content[n].b_inodo)+"</TD>\n</TR>\n";
                            n++;
                        }
                        contador++;
                        file<<"</TABLE>>]";
                    }else if(buffer =='2'){
                        fseek(archivo, super.s_block_start+64*contador,SEEK_SET);
                        fread(&blockFile, 64, 1, archivo);
                        string contenido ="";
                        contenido = blockFile.b_content;
                        file<<"nodo"+to_string(contador)+"[label=\"Bloque Archivo "+to_string(contador)+" \\n "+contenido+"\"]\n";
                        contador++;

                    }

                }

                for (int i = 0; i+1 < contador; i++) {
                    file << "nodo"+to_string(i)+"->nodo"+to_string(i+1)+"\n";
                }
                file<< "}";
                file.close();

                string extension = "dot -T"+comprobarExtension(path)+ " ./reportes/reporte.dot -o "+path;
                system(extension.c_str());
                fclose(archivo);

                if(archivo= fopen(path.c_str(), "rb+")){
                    printf("Se creo correctamente el reporte block\n");
                    fclose(archivo);
                }else{
                    printf("Error al crear el reporte block\n");
                }

            }else{
                if(NParticionE != -1){
                    ext Extendida;
                    fseek(archivo, master.particiones[NParticionE].p_comienzo, SEEK_SET);
                    fread(&Extendida, sizeof(ext), 1, archivo);
                    bool comprobar2 = false;
                    while ((ftell(archivo)<master.particiones[NParticionE].p_tam + master.particiones[NParticionE].p_comienzo)) {
                        if (strcmp(Extendida.e_nombre, (aux->nombre).c_str())==0){
                            comprobar2 = true;
                            break;
                        }
                        if(Extendida.e_siguiente == -1){
                            break;
                        }
                        fseek(archivo, Extendida.e_siguiente, SEEK_SET);
                        fread(&Extendida, sizeof(ext), 1 , archivo);
                    }
                    if (comprobar2){
                        file<<"digraph G{\n"<<"node[shape=box]\n"<<"concentrate=true\n"<<"rankdir=\"LR\"\n";
                        BloqueArchivo blockFile;
                        BloqueCarpeta blockCarpet;
                        SuperBloque super;
                        fseek(archivo, Extendida.e_comienzo, SEEK_SET);
                        fread(&super, sizeof(SuperBloque), 1, archivo);


                        int contador = 0;
                        int aux = super.s_bm_block_start;
                        char buffer;

                        while(aux<super.s_inode_start){
                            fseek(archivo, super.s_bm_block_start+contador, SEEK_SET);
                            buffer = char(fgetc(archivo));
                            aux++;

                            if(buffer== '1'){
                                fseek(archivo, super.s_block_start+64*contador, SEEK_SET);
                                fread(&blockCarpet, 64, 1 , archivo);
                                file<< "nodo"+to_string(contador)+"[label=<\n<TABLE BORDER =\"0\">\n";
                                file<<"<TR>\n<TD COLSPAN=\"2\">Bloque Carpeta "+to_string(contador)+"</TD>\n</TR>\n";
                                file<<"<TR>\n<TD>b_name</TD><TD>b_inodo</TD>\n</TR>\n";
                                int n = 0;
                                while(blockCarpet.b_content[n].b_inodo != -1){
                                    string nombre = "";
                                    nombre = blockCarpet.b_content[n].b_name;
                                    //cout<<nombre<<endl;
                                    file<<"<TR>\n<TD>"+nombre+"</TD><TD>"+to_string(blockCarpet.b_content[n].b_inodo)+"</TD>\n</TR>\n";
                                    n++;
                                }
                                contador++;
                                file<<"</TABLE>>]";
                            }else if(buffer =='2'){
                                fseek(archivo, super.s_block_start+64*contador,SEEK_SET);
                                fread(&blockFile, 64, 1, archivo);
                                string contenido ="";
                                contenido = blockFile.b_content;
                                file<<"nodo"+to_string(contador)+"[label=\"Bloque Archivo "+to_string(contador)+" \\n "+contenido+"\"]\n";
                                contador++;

                            }

                        }

                        for (int i = 0; i+1 < contador; i++) {
                            file << "nodo"+to_string(i)+"->nodo"+to_string(i+1)+"\n";
                        }
                        file<< "}";
                        file.close();

                        string extension = "dot -T"+comprobarExtension(path)+ " ./reportes/reporte.dot -o "+path;
                        system(extension.c_str());
                        fclose(archivo);

                        if(archivo= fopen(path.c_str(), "rb+")){
                            printf("Se creo correctamente el reporte block\n");
                            fclose(archivo);
                        }else{
                            printf("Error al crear el reporte block\n");
                        }
                    }else{
                        printf("Error no se encontro la particion\n");
                    }

                }else{
                    printf("Error no se encontro la particion\n");
                }
            }
       }else{
            printf("Error no se encontro el disco\n");
        }
    }else{
        printf("Error no se encontro el mismo id montada\n");
    }
}

void crearSB(string path, string id){
    bool comprobar =false;
    nodoMount *aux = ListaM->primero;
    if (aux->id == id){
        comprobar = true;
    }else{
        while (aux != nullptr){
            if (aux->id == id){
                comprobar = true;
                break;
            }
            aux = aux->sig;
        }
    }
    if(comprobar){
        FILE *archivo;
        if(archivo =fopen((aux->path).c_str(),"rb+")){
            Disco master;
            int NParticion = -1;
            int NParticionE = -1;
            fseek(archivo, 0, SEEK_SET);
            fread(&master, sizeof(Disco), 1, archivo);

            ofstream file;
            comprobarRuta(path);
            file.open("./reportes/reporte.dot",ios::out);
            if(file.fail()){
                cout<<"Error no se pudo abrir el archivo";
                exit(1);
            }
            for (int i = 0; i < 4; i++) {
                if (strcmp(master.particiones[i].p_nombre, (aux->nombre).c_str())==0){
                    NParticion = i;
                    break;
                }
                if (master.particiones[i].p_type == 'E'){
                    NParticionE = i;
                }
            }
            if(NParticion!=-1){
                file<<"digraph G{\n"<<"node[shape=box]\n"<<"concentrate=true\n";
                file<<"ReporteSB[label=<<TABLE BORDER=\"2\" CELLBORDER=\"1\" CELLSPACING=\"1\">\n";
                file<<"<TR>\n<TD>NOMBRE</TD>\n<TD>VALOR</TD>\n</TR>";
                SuperBloque super;
                fseek(archivo, master.particiones[NParticion].p_comienzo, SEEK_SET);
                fread(&super, sizeof(super), 1, archivo);
                file<<"<TR>\n<TD>s_inodes_count</TD>\n<TD>"+to_string(super.s_inodes_count)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_blocks_count</TD>\n<TD>"+to_string(super.s_block_count)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_free_blocks_count</TD>\n<TD>"+to_string(super.s_free_blocks_count)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_free_inodes_count</TD>\n<TD>"+to_string(super.s_free_inodes_count)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_mtime</TD>\n<TD>"+obtenerFecha(super.s_mtime)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_umtime</TD>\n<TD>"+obtenerFecha(super.s_unmtime)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_mnt_count</TD>\n<TD>"+to_string(super.s_block_count)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_magic</TD>\n<TD>"+to_string(super.s_magic)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_inode_size</TD>\n<TD>"+to_string(super.s_inode_size)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_block_size</TD>\n<TD>"+to_string(super.s_block_size)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_first_ino</TD>\n<TD>"+to_string(super.s_firts_ino)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_first_blo</TD>\n<TD>"+to_string(super.s_firts_blo)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_bm_inode_start</TD>\n<TD>"+to_string(super.s_bm_inode_start)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_bm_block_start</TD>\n<TD>"+to_string(super.s_bm_block_start)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_inode_start</TD>\n<TD>"+to_string(super.s_inode_start)+"</TD>\n</TR>";
                file<<"<TR>\n<TD>s_block_start</TD>\n<TD>"+to_string(super.s_block_start)+"</TD>\n</TR>";
                file<<"</TABLE>>]\n}";
                file.close();
                string extension = "dot -T"+comprobarExtension(path)+ " ./reportes/reporte.dot-o "+path;
                system(extension.c_str());
                fclose(archivo);

                if(archivo= fopen(path.c_str(), "rb+")){
                    printf("Se creo correctamente el reporte sb\n");
                    fclose(archivo);
                }else{
                    printf("Error al crear el reporte sb\n");
                }
            }else{
                if(NParticionE != -1){
                    ext Extendida;
                    fseek(archivo, master.particiones[NParticionE].p_comienzo, SEEK_SET);
                    fread(&Extendida, sizeof(ext), 1, archivo);
                    bool comprobar2 = false;
                    while ((ftell(archivo)<master.particiones[NParticionE].p_tam + master.particiones[NParticionE].p_comienzo)) {
                        if (strcmp(Extendida.e_nombre, (aux->nombre).c_str())==0){
                            comprobar2 = true;
                            break;
                        }
                        if(Extendida.e_siguiente == -1){
                            break;
                        }
                        fseek(archivo, Extendida.e_siguiente, SEEK_SET);
                        fread(&Extendida, sizeof(ext), 1 , archivo);
                    }
                    if (comprobar2){
                        file<<"digraph G{\n"<<"node[shape=box]\n"<<"concentrate=true\n";
                        file<<"ReporteSB[label=<<TABLE BORDER=\"2\" CELLBORDER=\"1\" CELLSPACING=\"1\">\n";
                        file<<"<TR>\n<TD>NOMBRE</TD>\n<TD>VALOR</TD>\n</TR>";
                        SuperBloque super;
                        fseek(archivo, Extendida.e_comienzo, SEEK_SET);
                        fread(&super, sizeof(super), 1, archivo);
                        file<<"<TR>\n<TD>s_inodes_count</TD>\n<TD>"+to_string(super.s_inodes_count)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_blocks_count</TD>\n<TD>"+to_string(super.s_block_count)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_free_blocks_count</TD>\n<TD>"+to_string(super.s_free_blocks_count)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_free_inodes_count</TD>\n<TD>"+to_string(super.s_free_inodes_count)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_mtime</TD>\n<TD>"+obtenerFecha(super.s_mtime)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_umtime</TD>\n<TD>"+obtenerFecha(super.s_unmtime)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_mnt_count</TD>\n<TD>"+to_string(super.s_block_count)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_magic</TD>\n<TD>"+to_string(super.s_magic)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_inode_size</TD>\n<TD>"+to_string(super.s_inode_size)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_block_size</TD>\n<TD>"+to_string(super.s_block_size)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_first_ino</TD>\n<TD>"+to_string(super.s_firts_ino)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_first_blo</TD>\n<TD>"+to_string(super.s_firts_blo)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_bm_inode_start</TD>\n<TD>"+to_string(super.s_bm_inode_start)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_bm_block_start</TD>\n<TD>"+to_string(super.s_bm_block_start)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_inode_start</TD>\n<TD>"+to_string(super.s_inode_start)+"</TD>\n</TR>";
                        file<<"<TR>\n<TD>s_block_start</TD>\n<TD>"+to_string(super.s_block_start)+"</TD>\n</TR>";
                        file<<"</TABLE>>]\n}";
                        file.close();
                        string extension = "dot -T"+comprobarExtension(path)+ " ./reportes/reporte.dot -o "+path;
                        system(extension.c_str());
                        fclose(archivo);

                        if(archivo= fopen(path.c_str(), "rb+")){
                            printf("Se creo correctamente el reporte sb\n");
                            fclose(archivo);
                        }else{
                            printf("Error al crear el reporte sb\n");
                        }
                    }else{
                        printf("Error no se encontro el disco\n");
                    }
                }else{
                    printf("Error no se encontro el disco\n");
                }
            }
        }
    }else{
        printf("Error no se encontro el mismo id montada\n");
    }
}
*/

int count_logica(string path, int NParticionE){
    int contar = 0;
    FILE *archivo;
    MBR master;
    archivo = fopen(path.c_str(), "rb+");
    fseek(archivo, 0 ,SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);
    EBR Extendida;
    fseek(archivo, master.mbr_partitiones[NParticionE].part_start, SEEK_SET);
    fread(&Extendida, sizeof(EBR), 1, archivo);
    if(Extendida.part_next != -1){
        while ((ftell(archivo)<master.mbr_partitiones[NParticionE].part_size + master.mbr_partitiones[NParticionE].part_start)) {
            contar++;
            contar++;
            if(Extendida.part_next == -1){
                 break;
            }
            fseek(archivo, Extendida.part_next, SEEK_SET);
            fread(&Extendida, sizeof(EBR), 1 , archivo);
        }
    }
    return contar;
}

string two_decimal(double num){
    string aux = to_string(num);
    char back = aux.back();
    while(back == '0'){
       aux = (aux).substr(0, (aux).size()-1);
       back = aux.back();
    }
    return aux;
}

string get_date(time_t creacion){
    string hora = "";
    struct tm *tm;
    char fecha[100];
    tm = localtime(&creacion);
    strftime(fecha,100,"%d/%m/%y %H:%S",tm);
    hora = fecha;
    return hora;
}

string get_extension(string path){
    string Carpeta = path;
    string ext ="";
    while(true){
        char aux = Carpeta.back();
        if (aux=='.'){
            break;
        }

        ext = Carpeta.back() + ext;
        Carpeta = Carpeta.substr(0, Carpeta.size()-1);
    }
    return ext;
}



void make_mbr(string pathRep, string path){
    //Primero obtenemos el mbr
    FILE *archivo = fopen((path).c_str(),"rb+");
    MBR master;
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);

    ofstream file;
    file.open("./reportes/reporte.dot",ios::out);
    if(file.fail()){
        cout<<"Error no se pudo abrir el archivo";
        return;
    }
    file<<"digraph G{\n"<<"node[shape=plaintext]\n"<<"concentrate=true\n"<<"ReporteMBR[label=<\n";
    file<<"<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" >\n";

    //Graficamos los datos del mbr
    file<<"<TR>\n"<<"<TD COLSPAN=\"4\" BORDER = \"1\">REPORTE DE MBR</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">mbr_tamano</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(master.mbr_tamano)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">mbr_fecha_creacion</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+get_date(master.mbr_fecha_creacion)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">mbr_disk_signature</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(master.mbr_disk_signature)+"</TD>\n"<<"</TR>\n";

    int NPartE = -1;
    for (int i = 0; i < 4; i++) {
        if(master.mbr_partitiones[i].part_start != -1){
            string status="";
            string type="";
            string fit;
            status = (master.mbr_partitiones[i].part_status);
            type = (master.mbr_partitiones[i].part_type);
            fit =(master.mbr_partitiones[i].part_type);
            string name(master.mbr_partitiones[i].part_name);
            //Graficamos las particiones logicas
            if(type == "P" || type == "E"){
                file<<"<TR bgcolor=\"blue3\">\n"<<"<TD COLSPAN=\"4\" BORDER = \"1\">Particion</TD>\n"<<"</TR>\n";
            } else {
                file<<"<TR>\n"<<"<TD COLSPAN=\"4\" BORDER = \"1\">Particion Logica</TD>\n"<<"</TR>\n";
            }
            file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_status</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+status+"</TD>\n"<<"</TR>\n";
            file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_type</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+type+"</TD>\n"<<"</TR>\n";
            file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_fit</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+fit+"</TD>\n"<<"</TR>\n";
            file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(master.mbr_partitiones[i].part_start)+"</TD>\n"<<"</TR>\n";
            file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_size</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(master.mbr_partitiones[i].part_size)+"</TD>\n"<<"</TR>\n";
            file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_name</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+name+"</TD>\n"<<"</TR>\n";
        }
        if (master.mbr_partitiones[i].part_type == 'E'){
            NPartE = i;
        }
    }
    file<<"</TABLE>>]\n";

    if (NPartE!= -1){
        EBR Extendida;
        fseek(archivo, master.mbr_partitiones[NPartE].part_start, SEEK_SET);
        fread(&Extendida, sizeof(EBR), 1, archivo);
        int i = 1;
        if (Extendida.part_next != -1){
            file<<"EBR[label=<\n";
            file<<"<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" >\n";
            while ((ftell(archivo)<(master.mbr_partitiones[NPartE].part_size + master.mbr_partitiones[NPartE].part_start))) {

                int comprobar = Extendida.part_next - (Extendida.part_start + Extendida.part_size);
                if (comprobar > 0){
                    string status="";
                    status = (Extendida.part_status);
                    string fit;
                    fit =(Extendida.part_fit);
                    string name(Extendida.part_name);
                    
                    file<<"<TR>\n"<<"<TD COLSPAN=\"4\" BORDER = \"1\">Particion</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_status</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+status+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_fit</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+fit+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_start)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_size</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_size)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_next</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_next-comprobar)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_name</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+name+"</TD>\n"<<"</TR>\n";
                    i++;

                    string fits;
                    fits =(Extendida.part_fit);
                    file<<"<TR>\n"<<"<TD COLSPAN=\"4\" BORDER = \"1\">Particion</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_status</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">1</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_fit</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+fits+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_next-Extendida.part_size)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_size</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(comprobar)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_next</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_next)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_name</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\"></TD>\n"<<"</TR>\n";

                }else{
                    string status="";
                    status = (Extendida.part_status);
                    string fit;
                    fit =(Extendida.part_fit);
                    string name(Extendida.part_name);
                    file<<"<TR>\n"<<"<TD COLSPAN=\"4\" BORDER = \"1\">Particion</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_status</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+status+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_fit</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+fit+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_start)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_size</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_size)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_next</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(Extendida.part_next)+"</TD>\n"<<"</TR>\n";
                    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">part_name</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+name+"</TD>\n"<<"</TR>\n";
                }
                if(Extendida.part_next == -1){
                        break;
                }
                i++;
                fseek(archivo, Extendida.part_next, SEEK_SET);
                fread(&Extendida, sizeof(EBR), 1 , archivo);
            }
            file<<"</TABLE>>]\n";
        }

    }
    file<<"}";
    file.close();
    string extension = "sudo dot -T " + get_extension(pathRep) + " ./reportes/reporte.dot -o " + pathRep;
    system(extension.c_str());
    fclose(archivo);

    if(archivo= fopen(pathRep.c_str(), "rb+")){
        printf("[Success] > Se creo correctamente el reporte mbr\n");
        fclose(archivo);
    }else{
        printf("[Error] > No fue posible crear el reporte mbr\n");
    }
}

void make_disk(string pathRep, string path){
    //Obtenemos el el mbr
    MBR master;
    FILE *archivo = fopen((path).c_str(), "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&master, sizeof(MBR), 1, archivo);

    ofstream file;
    file.open("./reportes/reporte.dot",ios::out);
    if(file.fail()){
        cout<<"Error no se pudo abrir el archivo";
        exit(1);
    }
    file<<"digraph G{\n"<<"node[shape=plaintext]\n"<<"concentrate=true\n"<<"ReporteDISK[label=<\n";
    file<<"<TABLE BORDER=\"2\" CELLBORDER=\"1\" CELLSPACING=\"2\" >\n";
    file<<"<TR>\n<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">MBR</TD>\n";
    int NParticionE = -1;
    int tamTotal = 0;
    for (int i = 0; i < 4; i++) {
        if (master.mbr_partitiones[i].part_type == 'P'){
            tamTotal += master.mbr_partitiones[i].part_size;
            double sizePrimaria = ((master.mbr_partitiones[i].part_size)*100)/master.mbr_tamano;
            file<<"<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">Primaria <BR/> "+two_decimal(sizePrimaria)+"\% del disco</TD>\n";
            if(i!=3){
                double comprobar = master.mbr_partitiones[i+1].part_start - (master.mbr_partitiones[i].part_size+master.mbr_partitiones[i].part_start);
                if (comprobar > 0){
                    double sizeLibre = (comprobar*100)/master.mbr_tamano;
                    file<<"<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">Libre <BR/> "+two_decimal(sizeLibre)+"\% del disco</TD>\n";
                }
            }else{
                double comprobar = (master.mbr_tamano+sizeof(MBR)) - (master.mbr_partitiones[i].part_size+master.mbr_partitiones[i].part_start);
                if(comprobar >0){
                    double libre = comprobar;
                    double sizeLibre = (libre*100)/master.mbr_tamano;
                    file<<"<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">Libre <BR/> "+two_decimal(sizeLibre)+"\% del disco</TD>\n";
                }
            }
        }else if (master.mbr_partitiones[i].part_type == 'E'){
            tamTotal += master.mbr_partitiones[i].part_size;
                int cont = count_logica(path, i);
                if (cont == 0){
                    double sizeExtendida = ((master.mbr_partitiones[i].part_size)*100)/master.mbr_tamano;
                    file<<"<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">Extendida <BR/> "+two_decimal(sizeExtendida)+"\% del disco</TD>\n";
                }else{
                file<<"<TD COLSPAN=\""+to_string(cont+1)+"\" ROWSPAN = \"1\" BORDER = \"1\">Extendida</TD>\n";
                NParticionE = i;
                }
                if(i!=3){
                    double comprobar = master.mbr_partitiones[i+1].part_start - (master.mbr_partitiones[i].part_size+master.mbr_partitiones[i].part_start);
                    if (comprobar > 0){
                        double sizeLibre = (comprobar*100)/master.mbr_tamano;
                        file<<"<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">Libre <BR/> "+two_decimal(sizeLibre)+"\% del disco</TD>\n";
                    }
                }else{
                    double comprobar = (master.mbr_tamano+sizeof(MBR)) - (master.mbr_partitiones[i].part_size+master.mbr_partitiones[i].part_start);

                    if(comprobar >0){
                        double libre = comprobar;
                        double sizeLibre = (libre*100)/master.mbr_tamano;
                        file<<"<TD COLSPAN=\"1\" ROWSPAN = \"2\" BORDER = \"1\">Libre <BR/> "+two_decimal(sizeLibre)+"\% del disco</TD>\n";
                    }
                }
        }


    }
        file<<"</TR>\n";
    if (NParticionE != -1){
        file<<"<TR>\n";
        EBR Extendida;
        int total = master.mbr_tamano;
        int totalEx = master.mbr_partitiones[NParticionE].part_size;
        fseek(archivo, master.mbr_partitiones[NParticionE].part_start, SEEK_SET);
        fread(&Extendida, sizeof(EBR), 1, archivo);
        double totalLogic = 0;
        if (Extendida.part_next != -1){
                while ((ftell(archivo)<master.mbr_partitiones[NParticionE].part_size + master.mbr_partitiones[NParticionE].part_start)) {
                totalLogic += double(Extendida.part_size);
                    if(Extendida.part_status == '1'){
                    double sizeLibre = double(Extendida.part_size*100)/total;
                    file<<"<TD>Libre <BR/> "+two_decimal(sizeLibre)+"\% del disco</TD>\n";
                }else{
                        file<<"<TD>EBR</TD>\n";
                        double sizeLogica = double(Extendida.part_size*100)/total;
                        file<<"<TD>Logica <BR/> "+two_decimal(sizeLogica)+"\% del disco</TD>\n";
                }
                if(Extendida.part_next == -1){
                        break;
                }
                double aux = Extendida.part_next - (Extendida.part_start + Extendida.part_size);
                if (aux > 0){
                    double sizeLibre = (aux*100)/total;
                    file<<"<TD>Libre <BR/> "+two_decimal(sizeLibre)+"\% del disco</TD>\n";
                }

                fseek(archivo, Extendida.part_next, SEEK_SET);
                fread(&Extendida, sizeof(EBR), 1 , archivo);
                }
            }
        double comprobar = double(totalEx) - totalLogic;
        if(comprobar>0){
            double sizeLibre = (comprobar*100)/(total);

            file<<"<TD>Libre <BR/>"+two_decimal(sizeLibre)+"\% del disco</TD>\n";
        }
        file<<"</TR>\n";
    }
    file<<"</TABLE>>]\n}";
    file.close();
    string extension = "dot -T " + get_extension(pathRep)+ " ./reportes/reporte.dot -o "+pathRep;
    system(extension.c_str());
    fclose(archivo);

    if(archivo= fopen(pathRep.c_str(), "rb+")){
        printf("[Success] > Se creo correctamente el reporte disk\n");
        fclose(archivo);
    }else{
        printf("[Error] > No fue posible crear el reporte mbr\n");
    }
}

void make_bm_block(string pathRep, string path, string name){
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

    //Abrimos el archivo donde escribimos el reporte
    ofstream file;
    file.open("./reportes/reporte.dot",ios::out);
    if(file.fail()){
        cout<<"Error no se pudo abrir el archivo";
        return;
    }

    //Encabezado
    file<<"digraph G{\n"<<"node[shape=plaintext]\n"<<"concentrate=true\n"<<"ReporteMBR[label=<\n";

    //Empezamos la tabla
    file<<"<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" >\n";

    //Iteramos el bm de bloques
    int indice = 0;
    char status;
    fseek(archivo, bloquesito.s_bm_block_start, SEEK_SET);
    for(int i = 0; i < bloquesito.s_block_count; i++){
        fread(&status, sizeof(char), 1, archivo);
        if(indice == 0) file << "<TR>" << endl;
        file << "<TD COLSPAN=\"2\" BORDER = \"1\">" << status << " | B | " << i+1 << "</TD>" << endl;
        
        indice++;
        if(indice == 10) {file << "</TR>" << endl; indice = 0;} 
    }
    if(indice > 0){
        file << "</TR>" << endl;
    }

    //Terminamos la tabla
    file<<"</TABLE>>]\n";
    file<<"}";
    file.close();

    //Procedemos a crear el archivo
    string extension = "sudo dot -T " + get_extension(pathRep) + " ./reportes/reporte.dot -o " + pathRep;
    system(extension.c_str());
    fclose(archivo);

    if(archivo= fopen(pathRep.c_str(), "rb+")){
        printf("[Success] > Se creo correctamente el reporte bm_block\n");
        fclose(archivo);
    }else{
        printf("[Error] > No fue posible crear el reporte bm_block\n");
    }
}

void make_bm_inode(string pathRep, string path, string name){
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

    //Abrimos el archivo donde escribimos el reporte
    ofstream file;
    file.open("./reportes/reporte.dot",ios::out);
    if(file.fail()){
        cout<<"Error no se pudo abrir el archivo";
        return;
    }

    //Encabezado
    file<<"digraph G{\n"<<"node[shape=plaintext]\n"<<"concentrate=true\n"<<"ReporteMBR[label=<\n";

    //Empezamos la tabla
    file<<"<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" >\n";

    //Iteramos el bm de bloques
    int indice = 0;
    char status;
    fseek(archivo, bloquesito.s_bm_inode_start, SEEK_SET);
    for(int i = 0; i < bloquesito.s_inodes_count; i++){
        fread(&status, sizeof(char), 1, archivo);
        if(indice == 0) file << "<TR>" << endl;
        file << "<TD COLSPAN=\"2\" BORDER = \"1\">" << status << " | I | " << i+1 << "</TD>" << endl;
        
        indice++;
        if(indice == 10) {file << "</TR>" << endl; indice = 0;} 
    }
    if(indice > 0){
        file << "</TR>" << endl;
    }

    //Terminamos la tabla
    file<<"</TABLE>>]\n";
    file<<"}";
    file.close();

    //Procedemos a crear el archivo
    string extension = "sudo dot -T " + get_extension(pathRep) + " ./reportes/reporte.dot -o " + pathRep;
    system(extension.c_str());
    fclose(archivo);

    if(archivo= fopen(pathRep.c_str(), "rb+")){
        printf("[Success] > Se creo correctamente el reporte bm_inode\n");
        fclose(archivo);
    }else{
        printf("[Error] > No fue posible crear el reporte bm_inode\n");
    }
}

void make_sb(string pathRep, string path, string name){
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

    //Abrimos el archivo donde escribimos el reporte
    ofstream file;
    file.open("./reportes/reporte.dot",ios::out);
    if(file.fail()){
        cout<<"Error no se pudo abrir el archivo";
        return;
    }

    //Encabezado
    file<<"digraph G{\n"<<"node[shape=plaintext]\n"<<"concentrate=true\n"<<"ReporteMBR[label=<\n";

    //Empezamos la tabla
    file<<"<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" >\n";

    //Contenido de la tabla
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_inodes_count</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_inodes_count)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_blocks_count</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_block_count)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_free_blocks_count</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_free_blocks_count)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_free_inodes_count</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_free_inodes_count)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_mtime</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+get_date(bloquesito.s_mtime)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_umtime</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+get_date(bloquesito.s_umtime)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_mnt_count</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_mnt_count)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_magic</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_magic)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_inode_size</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_inode_size)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_block_size</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_block_size)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_first_ino</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_first_ino)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_first_blo</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_first_blo)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_bm_inode_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_bm_inode_start)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_bm_block_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_bm_block_start)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_inode_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_inode_start)+"</TD>\n"<<"</TR>\n";
    file<<"<TR>\n"<<"<TD COLSPAN=\"2\" BORDER = \"1\">s_block_start</TD>\n"<<"<TD COLSPAN=\"2\" BORDER =\"1\">"+to_string(bloquesito.s_block_start)+"</TD>\n"<<"</TR>\n";

    //Terminamos la tabla
    file<<"</TABLE>>]" << endl;
    file<<"}";
    file.close();

    //Procedemos a crear el archivo
    string extension = "sudo dot -T " + get_extension(pathRep) + " ./reportes/reporte.dot -o " + pathRep;
    system(extension.c_str());
    fclose(archivo);

    if(archivo= fopen(pathRep.c_str(), "rb+")){
        printf("[Success] > Se creo correctamente el reporte sb\n");
        fclose(archivo);
    }else{
        printf("[Error] > No fue posible crear el reporte sb\n");
    }
}

void graph_inode(int start_inodo, string path, string pathRep){
    //Obtenemos el inodo
    TablaInodo inodo;
    FILE *archivo = fopen((path).c_str(), "rb+");
    fseek(archivo, start_inodo, SEEK_SET);
    fread(&inodo, sizeof(TablaInodo), 1, archivo);
    fclose(archivo);
    
    //Escribimos el inodo como tal
    outputfile << "\"node" << start_inodo << "\" [label = \"";
    outputfile << "<f0> INODO " << start_inodo << " |";
    for(int i = 0; i < 15; i++){
        outputfile << "<f" << i+1 << "> " << inodo.i_block[i];
        if(i < 14) outputfile << "|"; 
    }
    outputfile << "\"shape = \"record\"];" << endl;

    //Ahora tenemos que escribir los punteros
    for(int i = 0; i < 15; i++){
        if(i < 12){
            if(inodo.i_block[i] != -1){
                //Significa que encontramos un bloque
                if(inodo.i_type == '0'){
                    //Quiere decir que los bloques son de carpeta
                    int start_block = inodo.i_block[i];
                    //Obtenemos el bloque
                    BloqueCarpeta carpetita;
                    FILE *archivo = fopen((path).c_str(), "rb+");
                    fseek(archivo, start_block, SEEK_SET);
                    fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
                    fclose(archivo);
                    //Escribimos el bloque de inodo
                    outputfile << "\"node" << start_block << "\" [label = \"";
                    outputfile << "<f0> BLOQUE DE CARPETA " << start_block << " |";
                    for(int i = 0; i < 4; i++){
                        outputfile << "<f" << i+1 << "> [" << carpetita.b_content[i].b_name << "] ---- [" << carpetita.b_content[i].b_inodo << "]";
                        if(i < 3) outputfile << "|"; 
                    }
                    outputfile << "\"shape = \"record\"];" << endl;

                    //Ahora tenemos que enlazar el inodo actual al bloque
                    outputfile << "\"node" << start_inodo << "\":f" << i+1 << " -> \"node" << start_block << "\":f0;" << endl;

                    //Recursivamente pintamos los demas inodos
                    for(int i = 1; i < 4; i++) {
                        if(carpetita.b_content[i].b_inodo != -1){
                            //Graficamos el inodo
                            graph_inode(carpetita.b_content[i].b_inodo, path, pathRep);
                            //Lo enlazamos con el bloque
                            outputfile << "\"node" << start_block << "\":f" << i+1 << " -> \"node" << carpetita.b_content[i].b_inodo << "\":f0;" << endl;
                        } 
                    }
                } else {
                    //Quiere decir que los bloques son de archivos
                    int start_block = inodo.i_block[i];
                    //Obtenemos el bloque
                    BloqueArchivo archivito;
                    FILE *archivo = fopen((path).c_str(), "rb+");
                    fseek(archivo, start_block, SEEK_SET);
                    fread(&archivito, sizeof(BloqueArchivo), 1, archivo);
                    fclose(archivo);
                    //Escribimos el bloque de inodo
                    outputfile << "\"node" << start_block << "\" [label = \"";
                    outputfile << "<f0> BLOQUE DE ARCHIVO " << start_block << " |";
                    outputfile << "<f1> [" << archivito.b_name << "] |";
                    outputfile << "<f2> [" << archivito.b_content << "]";
                    outputfile << "\"shape = \"record\"];" << endl;
                    //Ahora enlazamos
                    outputfile << "\"node" << start_inodo << "\":f" << i+1 << " -> \"node" << start_block << "\":f0;" << endl;
                }
            }
        } else if(i == 12){
            if(inodo.i_block[i] != -1){
                //Primero conseguimos el bloque de apuntadores
                int start_pointer = inodo.i_block[i];
                //Obtenemos el bloque
                BloqueApuntador apuntadores;
                FILE *archivo = fopen((path).c_str(), "rb+");
                fseek(archivo, start_pointer, SEEK_SET);
                fread(&apuntadores, sizeof(BloqueApuntador), 1, archivo);
                fclose(archivo);

                //Escribimos el bloque de apuntadores
                outputfile << "\"node" << start_pointer << "\" [label = \"";
                outputfile << "<f0> BLOQUE DE APUNTADORES " << start_pointer << " |";
                for(int i = 0; i < 16; i++){
                    outputfile << "<f" << i+1 << "> " << apuntadores.b_pointers[i];
                    if(i < 15) outputfile << "|"; 
                }
                outputfile << "\"shape = \"record\"];" << endl;

                //Enlazamos el inodo al bloque de apuntadores
                outputfile << "\"node" << start_inodo << "\":f" << i+1 << " -> \"node" << start_pointer << "\":f0;" << endl;

                //Tenemos que iterar el bloque de apuntadores
                for(int i = 0; i < 16; i++){
                    if(apuntadores.b_pointers[i] != -1){
                        //Significa que encontramos un bloque
                        if(inodo.i_type == '0'){
                            //Quiere decir que los bloques son de carpeta
                            int start_block = apuntadores.b_pointers[i];
                            //Obtenemos el bloque
                            BloqueCarpeta carpetita;
                            FILE *archivo = fopen((path).c_str(), "rb+");
                            fseek(archivo, start_block, SEEK_SET);
                            fread(&carpetita, sizeof(BloqueCarpeta), 1, archivo);
                            fclose(archivo);
                            //Escribimos el bloque de inodo
                            outputfile << "\"node" << start_block << "\" [label = \"";
                            outputfile << "<f0> BLOQUE DE CARPETA " << start_block << " |";
                            for(int i = 0; i < 4; i++){
                                outputfile << "<f" << i+1 << "> [" << carpetita.b_content[i].b_name << "] ---- [" << carpetita.b_content[i].b_inodo << "]";
                                if(i < 3) outputfile << "|"; 
                            }
                            outputfile << "\"shape = \"record\"];" << endl;

                            //Ahora tenemos que enlazar el inodo actual al bloque
                            outputfile << "\"node" << start_inodo << "\":f" << i+1 << " -> \"node" << start_block << "\":f0;" << endl;

                            //Recursivamente pintamos los demas inodos
                            for(int i = 1; i < 4; i++) {
                                if(carpetita.b_content[i].b_inodo != -1){
                                    //Graficamos el inodo
                                    graph_inode(carpetita.b_content[i].b_inodo, path, pathRep);
                                    //Lo enlazamos con el bloque
                                    outputfile << "\"node" << start_block << "\":f" << i+1 << " -> \"node" << carpetita.b_content[i].b_inodo << "\":f0;" << endl;
                                } 
                            }
                        } else {
                            cout << "entraste aca?" << endl;

                            //Quiere decir que los bloques son de archivos
                            int start_block = apuntadores.b_pointers[i];
                            //Obtenemos el bloque
                            BloqueArchivo archivito;
                            FILE *archivo = fopen((path).c_str(), "rb+");
                            fseek(archivo, start_block, SEEK_SET);
                            fread(&archivito, sizeof(BloqueArchivo), 1, archivo);
                            fclose(archivo);
                            //Escribimos el bloque de inodo
                            outputfile << "\"node" << start_block << "\" [label = \"";
                            outputfile << "<f0> BLOQUE DE ARCHIVO " << start_block << " |";
                            outputfile << "<f1> [" << archivito.b_name << "] |";
                            outputfile << "<f2> [" << archivito.b_content << "]";
                            outputfile << "\"shape = \"record\"];" << endl;
                            //Ahora enlazamos
                            outputfile << "\"node" << start_inodo << "\":f" << i+1 << " -> \"node" << start_block << "\":f0;" << endl;
                        }
                    }
                }
            }


        } else if(i == 13){

        } else if(i == 14){

        }
    }

}

void make_tree(string pathRep, string path, string name){
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

    //Abrimos el archivo donde escribimos el reporte
    //ofstream file;
    outputfile.open("./reportes/reporte.dot",ios::out);
    if(outputfile.fail()){
        cout<<"Error no se pudo abrir el archivo";
        return;
    }

    //Encabezado
    outputfile << "digraph g {" << endl;
    outputfile << "fontname=\"Helvetica,Arial,sans-serif\"" << endl;
    outputfile << "node [fontname=\"Helvetica,Arial,sans-serif\"]" << endl;
    outputfile << "edge [fontname=\"Helvetica,Arial,sans-serif\"]" << endl;
    outputfile << "graph [rankdir = \"LR\"];" << endl;
    outputfile << "node [fontsize = \"16\" shape = \"ellipse\"];" << endl;
    outputfile << "edge [];" << endl;

    //Ahora tenemos que iterar todos los inodos desde el principio jajaja, que dios me ayude
    graph_inode(bloquesito.s_inode_start, path, pathRep);

    //Terminamos la tabla
    outputfile << "}";
    outputfile.close();

    //Procedemos a crear el archivo
    string extension = "sudo dot -T " + get_extension(pathRep) + " ./reportes/reporte.dot -o " + pathRep;
    system(extension.c_str());
    fclose(archivo);

    if(archivo= fopen(pathRep.c_str(), "rb+")){
        printf("[Success] > Se creo correctamente el reporte tree\n");
        fclose(archivo);
    }else{
        printf("[Error] > No fue posible crear el reporte tree\n");
    }
}





void rep::make_rep(rep *reportito){
    //Validaciones
    if(reportito->path == "") { cout << "[Error] > No se ingreso el parametro $path" << endl; return;}
    if(reportito->name == "") { cout << "[Error] > No se ingreso el parametro $name" << endl; return;}
    if(reportito->id == "") { cout << "[Error] > No se ingreso el parametro $id" << endl; return;}

    //Verificamos si existe la particion
    if(find_partition_in_mount(reportito->id)){
        //Obtenemos el nodo donde esta la particion
        nodoMount* nodo = get_partition_in_mount(reportito->id);
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

        //Revisamos que tipo de reporte es
        transform(reportito->name.begin(), reportito->name.end(), reportito->name.begin(), ::toupper);
        //Verificamos si la ruta existe, si no creamos las carpetas
        /*string rutaCarpeta = Carpeta;
        if( access( rutaCarpeta.c_str(), F_OK ) == -1 ) {
            string cmd = "mkdir -p \"" + rutaCarpeta + "\"";
            system(cmd.c_str());
        }*/

        if (reportito->name == "MBR"){
            make_mbr(reportito->path, realPath);
        }else if (reportito->name == "DISK"){
            make_disk(reportito->path, realPath);
        }else if (reportito->name == "BM_BLOCK"){
            make_bm_block(reportito->path, realPath, nodo->nombre);
        }else if (reportito->name == "BM_INODE"){
            make_bm_inode(reportito->path, realPath, nodo->nombre);
        }else if (reportito->name == "SB"){
            make_sb(reportito->path, realPath, nodo->nombre);
        }else if (reportito->name == "TREE"){
            make_tree(reportito->path, realPath, nodo->nombre);
        }/*else if (reportito->name == "INODE"){
            crearINODE(realPath, id);
        }else if (reportito->name == "BLOCK"){
            crearBLOCK(realPath, id);
        }else if (reportito->name == "SB"){
            crearSB(realPath, id);
        }else{
            cout << "[INFO] > Ya no me dio tiempo :c" << endl;
        }*/
    }

}