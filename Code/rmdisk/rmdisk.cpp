#include "rmdisk.h"
#include <stdio.h>

rmdisk::rmdisk(){}

void rmdisk::make_rmdisk(rmdisk *disco){
    //Pedir confirmacion
    string confirmacion;
    cout << "[Info] > Estas seguro de eliminar el disco S/N >";
    cin >> confirmacion;
    if(!(confirmacion == "S" || confirmacion == "s")) { cout << "[Info] > Se cancelo la eliminacion del disco" << endl;  return;}

    //Validaciones
    FILE *file;
    const char * ruta = disco->path.c_str();
    if (!(file = fopen(ruta, "r"))) { cout << "[Error] > No se ha encontrado el disco" << endl; fclose(file); return;} else {fclose(file);}

    //Eliminamos el disco
    remove(ruta);
    cout << "[Success] > Se elimino correctamente el disco" << endl;
}