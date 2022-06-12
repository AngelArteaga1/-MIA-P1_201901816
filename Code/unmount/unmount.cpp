#include "unmount.h"
#include <stdio.h>

#include "../listadoble/nodomount.h"
#include "../listadoble/listamount.h"
#include "../mount/mount.cpp"

unmount::unmount(){ }

void unmount::make_unmount(unmount *unmontar){
    //Validaciones
    if(unmontar->id == "") { cout << "[ERROR] > [Error] > No se ingreso el parametro de $id#" << endl; return; }
    if(listita == nullptr) { cout << "[ERROR] > No se encontro ese id montado" << endl; return; }
    if(listita->primero == nullptr){ cout << "[ERROR] > No se encontro ese id montado" << endl; return; }

    //si la lista esta vacia 
    nodoMount *tmp = listita->primero;
    //Metodo normal para eliminar en una lista simplemente enlazada
    if (tmp->id == unmontar->id){
        listita->primero = tmp->sig;
        //para borrar el objeto
        free(tmp);
        cout << "[SUCCESS] > Se desmonto la particion" << endl;
        return;
    }else{
        nodoMount *aux = nullptr;
        while (tmp != nullptr){
            if (tmp->id == unmontar->id){
                aux->sig = tmp->sig;
                cout << "[SUCCESS] > Se desmonto la particion" << endl;
                break;
            }
            aux = tmp;
            tmp = tmp->sig;
        }
    }
    cout << "[ERROR] > No se encontro ese id montado" << endl;
}