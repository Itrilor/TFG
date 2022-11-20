#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include "QKP.h"
#include "random.hpp"

using namespace std;

using Random = effolkronium::random_static;

/**
 * @brief Devuelve el valor de añadir un objeto considerando su peso
 * @param pos Índice del elemento que estamos considerando añadir
 * @param sol Solución que estamos considerando
 * @return El valor de añadir un objeto considerando su peso
 */

double QKP::valueIfAdded(int pos, vector<int> sol){
  double valor = getValor(pos,pos);
  for(int i = 0; i < sol.size(); ++i){
    valor += getValor(pos, sol[i]);
  }
  return valor/getPeso(pos);
}

double QKP::valueIfAdded(int pos){
  return valueIfAdded(pos, _solucion);
}

void QKP::Greedy(int max){
  vector<int> indicesDatos;
  int contador = 0;
  bool cond = false;
  double aux = 0;
  double max = 0;
  int pos_max = 0;
  
  for(int i = 0; i < getSize();++i){
    indicesDatos.push_back(i);
  }
  Random::shuffle(indicesDatos);
  
  for(int i = 0; i < getSize() && !cond; ++i){
    for(int j = 0; j < indicesDatos.size(); ++j){
      if(checkAdd(indicesDatos[j])){
        aux = valueIfAdded(indicesDatos[j]);
        if(aux > max){
          max = aux;
          pos_max = indicesDatos[j];
        }
      }
    }
    addSolucion(pos_max);
    contador++;
    indicesDatos.erase(indicesDatos.begin()+pos_max);
    max = 0;
    if(max!=0 && max==contador){
      cond=true;
    }
  }
}

void QKP::GRASP(){
  
}
