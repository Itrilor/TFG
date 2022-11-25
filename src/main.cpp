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

  //------------RANDOM------------
  cout << "Leemos " << argv[1] << "\n";
  estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }

  auto start = std::chrono::high_resolution_clock::now();
  sequence.RandomQKP();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double,std::micro> float_ms = end-start;
  double tiempoRandom = float_ms.count();

  cout << "La secuencia obtenida por el algoritmo Random es:\n { ";
  for(int i = 0; i < sequence.getSolucion().size(); ++i){
    cout << sequence.getSolucion()[i] << ",";
  }
  cout << "}\nEl valor alcanzado sería: " << sequence.getValorSolucion() << "\n";
  cout << "El peso alcanzado sería: " << sequence.getPesoSolucion() << "\n";
  cout << "Tiempo de ejecución: " << tiempoRandom << "microsegundos\n";

  //------------GREEDY------------
  cout << "Leemos " << argv[1] << "\n";
  estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }

  start = std::chrono::high_resolution_clock::now();
  sequence.Greedy();
  end = std::chrono::high_resolution_clock::now();
  float_ms = end-start;
  double tiempoGreedy = float_ms.count();

  cout << "La secuencia obtenida por el algoritmo Greedy es:\n { ";
  for(int i = 0; i < sequence.getSolucion().size(); ++i){
    cout << sequence.getSolucion()[i] << ",";
  }
  cout << "}\nEl valor alcanzado sería: " << sequence.getValorSolucion() << "\n";
  cout << "El peso alcanzado sería: " << sequence.getPesoSolucion() << "\n";
  cout << "Tiempo de ejecución: " << tiempoGreedy << "microsegundos\n";

}

void mensajeError(){
  cerr << "ERROR en la llamada" << endl;
  cerr << "./ejecutable [nombre_datos]" << endl;
  cerr << "Entiéndase [nombre_datos] como el nombre del archivo de los datos" << endl;
  exit(1);
}
