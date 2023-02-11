#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <vector>
#include "QKP.h"
#include "AG.h"
#include "AGCEP.h"
#include "random.hpp"

using namespace std;

void mensajeError();
void mensajeSolucion();

const int NEVALUACIONESMAX = 450;

int main(int argc, char ** argv){
  if(argc!=2){
    mensajeError();
  }

  QKP sequence;
  bool estado;
  vector<double> values;

  //------------RANDOM------------
  //cout << "Leemos " << argv[1] << "\n";
  /*estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }

  auto start = std::chrono::high_resolution_clock::now();
  sequence.RandomQKP(NEVALUACIONESMAX);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> float_s = end-start;
  double tiempoRandom = float_s.count();

  //mensajeSolucion("Algoritmo Random", sequence, tiempoRandom, "segundos");
  //cout << argv[1] << "," << sequence.getValorSolucion() << "," << tiempoRandom << ",";
  cout << sequence.getValorSolucion() << ",";*/

  //------------GREEDY------------
  //cout << "Leemos " << argv[1] << "\n";
  /*estado = sequence.leerFicheroDatos(argv[1]);
  if(!estado){
    cerr << "No se ha podido leer correctamente\n";
    exit(-1);
  }

  start = std::chrono::high_resolution_clock::now();
  sequence.Greedy();
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double,std::micro> float_ms = end-start;
  double tiempoGreedy = float_ms.count();

  //mensajeSolucion("Algoritmo Greedy", sequence, tiempoGreedy, "microsegundos");
  cout << sequence.getValorSolucion() << "," << tiempoGreedy << ",";*/

  //------------ALGORITMO GENETICO ESTACIONARIO UNIFORME ----------------
  //cout << "Leemos " << argv[1] << "\n";
  /*values.clear();
  for(int j = 0; j < 10; ++j){
    estado = sequence.leerFicheroDatos(argv[1]);
    if(!estado){
      cerr << "No se ha poddio leer correctamente\n";
      exit(-1);
    }
    sequence.AGEU(10,0.1,NEVALUACIONESMAX);
    values.push_back(sequence.getValorSolucion());
  }
  double tiempoAGEU = float_s.count();
  double meanValue = 0;
  for(int i = 0; i < values.size(); ++i){
    meanValue += values[i];
  }
  meanValue = meanValue/values.size();

  //mensajeSolucion("Algoritmo Genético Estacionario Uniforme", sequence, tiempoAGEU, "segundos");
  //cout << sequence.getValorSolucion() << "\n";
  //cout << meanValue << "," << tiempoAGEU << "\n";
  cout << meanValue << "\n";*/

  //---------GENETIC ALGORITHM FOR COMBINATORY EXPENSIVE PROBLEM----------
  //values.clear();
  for(int j = 0; j < 10; ++j){
    estado = sequence.leerFicheroDatos(argv[1]);
    if(!estado){
      cerr << "No se ha poddio leer correctamente\n";
      exit(-1);
    }
    sequence.GACEP(10,0.1,NEVALUACIONESMAX);
    values.push_back(sequence.getValorSolucion());
  }
  double meanValue = 0;
  for(int i = 0; i < values.size(); ++i){
    meanValue += values[i];
  }
  meanValue = meanValue/values.size();

  //mensajeSolucion("Algoritmo genético para problemas combinatorios expensive", sequence, tiempoAGEU, "segundos");
  //cout << sequence.getValorSolucion() << "\n";
  //cout << meanValue << "," << tiempoAGEU << "\n";
  cout << meanValue << "\n";
}

void mensajeSolucion(string name, QKP seq, double tiempo, string seg){
  cout << "La secuencia obtenida por el " << name << " es:\n {";
  for(int i = 0; i < seq.getSolucion().size(); ++i){
    cout << seq.getSolucion()[i] << ",";
  }
  cout << "}\nEl valor alcanzado sería: " << seq.getValorSolucion() << "\n";
  cout << "El peso alcanzado sería: " << seq.getPesoSolucion() << "\n";
  cout << "Tiempo de ejecución: " << tiempo << " " << seg << "\n";
}

void mensajeError(){
  cerr << "ERROR en la llamada" << endl;
  cerr << "./ejecutable [nombre_datos]" << endl;
  cerr << "Entiéndase [nombre_datos] como el nombre del archivo de los datos" << endl;
  exit(1);
}
