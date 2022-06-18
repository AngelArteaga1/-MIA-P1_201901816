#ifndef MKFILE_H
#define MKFILE_H

#include "string"
#include "../estructura/estructura.h"
#include <iostream>
using namespace std;

int get_count_tokens(char * token, int count);
int search_path(char * token, int inodo_start, string path, int count_token, int total_token);
int make_path(char * token, int inodo_start, string path, SuperBloque bloquesito, char fit, int count_token, int total_token);

class mkfile
{
public:
    mkfile();

    string id="";
    string path="";
    bool p = false;
    int size = 0;
    string cont="";

    void make_mkfile(mkfile *Archivito);

};

#endif