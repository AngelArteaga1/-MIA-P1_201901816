#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H */
#include <time.h>
#include "string"
#include <iostream>
using namespace std;

typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
}Particion;

typedef struct{
    int mbr_tamano;
    int mbr_disk_signature;
    time_t mbr_fecha_creacion;
    Particion mbr_partitiones[4];
}MBR;


/************************* NO CONFIRMADAS ****************************/

typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_block_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}SuperBloque;

typedef struct{
    char journal_tipo_operacion[14];
    char journal_tipo;
    char journal_nombre[14];
    char journal_contenido;
    time_t journal_fecha;
    char journal_propietario[14];
    char journal_permisos;
}Journaling;

typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
}TablaInodo;

typedef struct{
    char b_name[12];
    int b_inodo;
}Content;

typedef struct{
    Content b_content[4];
}BloqueCarpeta;

typedef struct{
    int b_pointers[16];
}BloqueApuntador;

typedef struct{
    char b_name[14];
    char b_content[50];
}BloqueArchivo;

#endif // PARTICION_H