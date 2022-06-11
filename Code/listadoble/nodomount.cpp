#include "nodomount.h"

nodoMount::nodoMount(string id, string path, string name, int numero, char letra)
{
    this->id = id;
    this->path = path;
    this->nombre = name;
    this->numero = numero;
    this->letra =letra;
    this->sig = nullptr;
}   