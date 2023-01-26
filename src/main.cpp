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
  //cout << "Leemos " << argv[1] << "\n";
  estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }

  auto start = std::chrono::high_resolution_clock::now();
  if(sequence.getSize()<=100){
    sequence.RandomQKP(5);
  }
  else{
    sequence.RandomQKP(30);
  }
  //sequence.RandomQKP();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> float_s = end-start;
  double tiempoRandom = float_s.count();

  /*cout << "La secuencia obtenida por el algoritmo Random es:\n { ";
  for(int i = 0; i < sequence.getSolucion().size(); ++i){
    cout << sequence.getSolucion()[i] << ",";
  }
  cout << "}\nEl valor alcanzado sería: " << sequence.getValorSolucion() << "\n";
  cout << "El peso alcanzado sería: " << sequence.getPesoSolucion() << "\n";
  cout << "Tiempo de ejecución: " << tiempoRandom << "microsegundos\n";*/
  cout << argv[1] << "," << sequence.getValorSolucion() << "," << tiempoRandom << ",";

  //------------GREEDY------------
  //cout << "Leemos " << argv[1] << "\n";
  estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }

  start = std::chrono::high_resolution_clock::now();
  sequence.Greedy();
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double,std::micro> float_ms = end-start;
  double tiempoGreedy = float_ms.count();

  /*cout << "La secuencia obtenida por el algoritmo Greedy es:\n { ";
  for(int i = 0; i < sequence.getSolucion().size(); ++i){
    cout << sequence.getSolucion()[i] << ",";
  }
  cout << "}\nEl valor alcanzado sería: " << sequence.getValorSolucion() << "\n";
  cout << "El peso alcanzado sería: " << sequence.getPesoSolucion() << "\n";
  cout << "Tiempo de ejecución: " << tiempoGreedy << "microsegundos\n";*/

  cout << sequence.getValorSolucion() << "," << tiempoGreedy << ",";

  //------------ALGORITMO GENETICO ESTACIONARIO UNIFORME ----------------
  //cout << "Leemos " << argv[1] << "\n";
  estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha poddio leer correctamente\n";
    exit(-1);
  }

  start = std::chrono::high_resolution_clock::now();
  if(sequence.getSize()<=100){
    sequence.AGEU(10,0.1,5);
  }
  else{
    sequence.AGEU(10, 0.1, 30);
  }
  end = std::chrono::high_resolution_clock::now();
  float_s = end-start;//seconds
  double tiempoAGEU = float_s.count();

  /*cout << "La secuencia obtenida por el Algoritmo Genético Estacionario Uniforme es:\n {";
  for(int i = 0; i < sequence.getSolucion().size(); ++i){
    cout << sequence.getSolucion()[i] << ",";
  }
  cout << "}\nEl valor alcanzado sería: " << sequence.getValorSolucion() << "\n";
  cout << "El peso alcanzado sería: " << sequence.getPesoSolucion() << "\n";
  cout << "Tiempo de ejecución: " << tiempoAGEU << "segundos\n";*/
  cout << sequence.getValorSolucion() << "," << tiempoAGEU << "\n";
}

void mensajeError(){
  cerr << "ERROR en la llamada" << endl;
  cerr << "./ejecutable [nombre_datos]" << endl;
  cerr << "Entiéndase [nombre_datos] como el nombre del archivo de los datos" << endl;
  exit(1);
}
