/**
 * @file CHC.cpp
 * @author Irene Trigueros Lorca
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::fill
#include "CHC.h"
#include "random.hpp"

using namespace std;

using Random = effolkronium::random_static;

/************CONSTRUCTORES************/
CHC::CHC(){}

CHC::CHC(AG &ag){
  _ag = ag;
}

CHC & CHC::operator=(const CHC& orig){
  if(&orig != this){
    _ag = orig._ag;
  }
  return *this;
}

/***********FUNCIONES CHC*************/

int CHC::distanciaHamming(int p1[], int p2[]){
  int contador = 0;
  for(int i = 0; i < getSize(); ++i){
    if(p1[i]!=p2[i]){
      contador++;
    }
  }
  return contador;
}

bool CHC::seleccion(int threshold, int p1[], int p2[]){
  return (distanciaHamming(p1,p2)>threshold);
}

void CHC::cruceHUX(int p1[], int p2[], int h1[], int h2[]){
  vector<int> indices;
  //Rellenamos los elementos comunes de ambos padres
  for(int i = 0; i < getSize(); ++i){
    if(p1[i] == p2[i]){
      h1[i] = p1[i];
      h2[i] = p2[i];
    }
    else{
      indices.push_back(i);
    }
  }
  //Rellenamos el resto
  Random::shuffle(indices);
  for(int i = 0; i < indices.size()/2; ++i){
    h1[indices[i]] = p1[indices[i]];
    h2[indices[i]] = p2[indices[i]];
  }
  for(int i = indices.size()/2; i < indices.size(); ++i){
    h1[indices[i]] = p2[indices[i]];
    h2[indices[i]] = p1[indices[i]];
  }

  // Hacemos las soluciones factibles
  _ag.operadorReparacion(h1);
  _ag.operadorReparacion(h2);
}

vector<int> CHC::enfrentamiento(double valorP[], double valorH[], int tam1, int tam2){
  vector<int> indices(tam1 + tam2);
  iota(indices.begin(), indices.end(), 0);

  vector<int> auxValor;
  for(int i = 0; i < tam1; ++i){
    auxValor.push_back(valorP[i]);
  }
  for(int i = 0; i < tam2; ++i){
    auxValor.push_back(valorH[i]);
  }

  sort(indices.begin(), indices.end(),
    [&] (int A, int B) -> bool{
      return auxValor[A] > auxValor[B];
    }
  );

  indices.erase(indices.begin()+tam1,indices.end());

  return indices;
}
