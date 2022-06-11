#include "exec.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../analizador/analizador.h"

exec::exec(){ }

void exec::make_exec(exec *exectutito){
    //Creamos el archivo para leer los comandos
    ifstream file(exectutito->path);
    string line;

    int primera = true;
    while (getline(file, line)){
        string lineCopy = line;
        transform(lineCopy.begin(), lineCopy.end(), lineCopy.begin(), :: tolower);
        if (lineCopy.compare("pause")== 0){
            string pausar;
            cout << "[PAUSE] > Presione enter para continuar >";
            cin.get();
            continue;
        }
        if(line != ""){
            //Analizamos la linea
            char * comando = &line[0];
            analizar(comando);
        }
        primera = false;
    }
    file.close();
}