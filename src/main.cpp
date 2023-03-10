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
const vector<int> seed = {95, 27, 83, 61, 4, 27, 14, 49, 92, 55};

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
  //values.clear();
  /*vector<vector<double>> milestones;
  for(int j = 0; j < 10; ++j){
    estado = sequence.leerFicheroDatos(argv[1]);
    if(!estado){
      cerr << "No se ha poddio leer correctamente\n";
      exit(-1);
    }
    milestones.push_back(sequence.AGEU(10,0.1,NEVALUACIONESMAX, seed[j]));
    //values.push_back(sequence.getValorSolucion());
  }*/
  //double tiempoAGEU = float_s.count();
  /*double meanValue = 0;
  for(int i = 0; i < values.size(); ++i){
    meanValue += values[i];
  }
  meanValue = meanValue/values.size();*/
  /*vector<double> meanMilestones = milestones[0];
  for(int i = 1; i < milestones.size(); ++i){
    for(int j = 0; j < milestones[i].size(); ++j){
      meanMilestones[j] += milestones[i][j];
    }
  }

  for(int i = 0; i < meanMilestones.size(); ++i){
    meanMilestones[i] = meanMilestones[i]/milestones.size();
    cout << meanMilestones[i] << " , ";
  }
  cout << "\n";*/

  //mensajeSolucion("Algoritmo Genético Estacionario Uniforme", sequence, tiempoAGEU, "segundos");
  //cout << sequence.getValorSolucion() << "\n";
  //cout << meanValue << "," << tiempoAGEU << "\n";
  //cout << meanValue << "\n";

  //---------GENETIC ALGORITHM FOR COMBINATORY EXPENSIVE PROBLEM----------
  //values.clear();
  /*vector<vector<double>> milestones;
  for(int j = 0; j < 10; ++j){
    estado = sequence.leerFicheroDatos(argv[1]);
    if(!estado){
      cerr << "No se ha podido leer correctamente\n";
      exit(-1);
    }
    milestones.push_back(sequence.GACEP(10,0.1,NEVALUACIONESMAX, seed[j]));
    //values.push_back(sequence.getValorSolucion());
  }
  vector<double> meanMilestones = milestones[0];
  for(int i = 1; i < milestones.size(); ++i){
    for(int j = 0; j < milestones[i].size(); ++j){
      meanMilestones[j] += milestones[i][j];
    }
  }

  for(int i = 0; i < meanMilestones.size(); ++i){
    meanMilestones[i] = meanMilestones[i]/milestones.size();
    cout << meanMilestones[i] << " , ";
  }
  cout << "\n";*/

  //mensajeSolucion("Algoritmo genético para problemas combinatorios expensive", sequence, tiempoAGEU, "segundos");
  //cout << sequence.getValorSolucion() << "\n";
  //cout << meanValue << "," << tiempoAGEU << "\n";
  //cout << meanValue << "\n";


  //---------CHC----------
  /*vector<vector<double>> milestones;
  for(int j = 0; j < 10; ++j){
    //cout << j << "\n";
    estado = sequence.leerFicheroDatos(argv[1]);
    if(!estado){
      cerr << "No se ha podido leer correctamente\n";
      exit(-1);
    }
    milestones.push_back(sequence.CHCGA(10,NEVALUACIONESMAX, seed[j]));
    //values.push_back(sequence.getValorSolucion());
  }
  vector<double> meanMilestones = milestones[0];
  for(int i = 1; i < milestones.size(); ++i){
    for(int j = 0; j < milestones[i].size(); ++j){
      meanMilestones[j] += milestones[i][j];
      //cout << milestones[i][j] << " , ";
    }
  }

  for(int i = 0; i < meanMilestones.size(); ++i){
    meanMilestones[i] = meanMilestones[i]/milestones.size();
    cout << meanMilestones[i] << " , ";
  }
  cout << "\n";*/

  //---------GACEP+CHC-------------------
  vector<vector<double>> milestones;
  for(int j = 0; j < 10; ++j){
    //cout << j << "\n";
    estado = sequence.leerFicheroDatos(argv[1]);
    if(!estado){
      cerr << "No se ha podido leer correctamente\n";
      exit(-1);
    }
    milestones.push_back(sequence.GACEPCHC(10,0.1,NEVALUACIONESMAX, seed[j]));
    //values.push_back(sequence.getValorSolucion());
  }
  vector<double> meanMilestones = milestones[0];
  for(int i = 1; i < milestones.size(); ++i){
    for(int j = 0; j < milestones[i].size(); ++j){
      meanMilestones[j] += milestones[i][j];
      //cout << milestones[i][j] << " , ";
    }
  }
  cout << "adfas\n";
  for(int i = 0; i < meanMilestones.size(); ++i){
    meanMilestones[i] = meanMilestones[i]/milestones.size();
    cout << meanMilestones[i] << " , ";
  }
  cout << "\n";

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
