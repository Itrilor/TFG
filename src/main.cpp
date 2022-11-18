#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include "QKP.h"
#include "random.hpp"

using namespace std;

void mensajeError();

int main(int argc, char ** argv){
  if(argc!=2){
    mensajeError();
  }

  QKP sequence;
  bool estado;

  cout << "Leemos " << argv[1] << "\n";
  estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }
}

void mensajeError(){
  cerr << "ERROR en la llamada" << endl;
  cerr << "./ejecutable [nombre_datos]" << endl;
  cerr << "Entiéndase [nombre_datos] como el nombre del archivo de los datos" << endl;
  exit(1);
}
