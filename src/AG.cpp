/**
 * @file AG.cpp
 * @author Irene Trigueros Lorca
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include "AG.h"
#include "random.hpp"

using namespace std;

using Random = effolkronium::random_static;

/*******CONSTRUCTORES Y DESTRUCTOR********/

AG::AG(){

}

AG::AG(QKP &prob){
  _qkp = prob;
}

AG & AG::operator=(const AG& orig){
  if(&orig != this){
    _qkp = orig._qkp;
  }
  return *this;
}


/******FUNCIONES AUXILIARES*******/

vector<int> AG::intToVector(int sol[]){
  vector<int> solAux;
  for(int i = 0; i < _qkp.getSize(); ++i){
    if(sol[i]==1){
      solAux.push_back(i);
    }
  }
  return solAux;
}

double AG::valueIfAdded(int pos, int sol[]){
  return _qkp.valueIfAdded(pos, intToVector(sol));
}

bool AG::checkAdd(int pos, double peso, int sol[]){
  return _qkp.checkAdd(pos, peso, intToVector(sol));
}

bool AG::checkSustituir(double peso, int pos1, int pos2){
  return (_qkp.getCapacidad() > (peso-_qkp.getPeso(pos1)+_qkp.getPeso(pos2)));
}

double AG::calcularPeso(int sol[]){
  double peso = 0;
  for(int i = 0; i < _qkp.getSize(); ++i){
    if(sol[i]==1){
      peso += _qkp.getPeso(i);
    }
  }
  return peso;
}

double AG::calcularValor(int sol[]){
  double valor = 0;
  vector<int> indices;

  for(int i = 0; i < _qkp.getSize(); ++i){
    if(sol[i]==1){
      indices.push_back(i);
      for(int j = 0; j<indices.size(); ++j){
        valor+=_qkp.getValor(i,indices[j]);
      }
    }
  }
  return valor;
}

double AG::calcularRelValor(int index, int sol[]){
  double relValor=0;
  if(sol[index==1]){
    for(int i = 0; i < _qkp.getSize(); ++i){
      if(sol[i]==1){
        relValor+=_qkp.getValor(i,index);
      }
    }
  }
  relValor = relValor/_qkp.getPeso(index);
  return relValor;
}

int AG::calcularMejorValor(double valor[], int tam){
  int index = 0;
  double mejorValor = valor[0];
  for(int i = 1; i < tam; ++i){
    if(valor[i] > mejorValor){
      index = i;
      mejorValor = valor[i];
    }
  }
  return index;
}

int AG::calcularPeorValor(double valor[], int tam){
  int index = 0;
  double peorValor = valor[0];
  for(int i = 1; i < tam; ++i){
    if(valor[i] < peorValor){
      index = i;
      peorValor = valor[i];
    }
  }
  return index;
}

vector<int> AG::calcular2Peores(double valor[], int tam){
  vector<int> index;
  index.push_back(calcularPeorValor(valor,tam));
  double aux[tam-1];
  int contador = 0;
  for(int i = 0; i < tam; ++i){
    if(i!=index[0]){
      aux[contador] = valor[i];
      contador++;
    }
  }
  index.push_back(calcularPeorValor(aux,tam-1));
  return index;
}

/*********FUNCIONES PARA EL AG************/
void AG::generaSeleccionAleatoria(int sol[], double &val){
  vector<int> indices;
  for(int i = 0; i < _qkp.getSize(); ++i){
    sol[i]=0;
    indices.push_back(i);
  }
  double peso = 0;
  Random::shuffle(indices);
  for(int i = 0; i<_qkp.getSize(); ++i){
    if(checkAdd(indices[i], peso, sol)){
      sol[indices[i]]=1;
      peso += _qkp.getPeso(indices[i]);
    }
  }
  val = calcularValor(sol);
}

vector<int> AG::torneoBinario(int numTorneos, double val[], int tam){
  vector<int> index;
  vector<int> res;

  for(int i = 0; i<numTorneos*2; ++i){
    index.push_back(Random::get(0,tam-1));
  }

  for(int i = 0; i < numTorneos*2; i+=2){
    if(val[index[i]]>val[index[i+1]]){
      res.push_back(index[i]);
    }
    else{
      res.push_back(index[i+1]);
    }
  }
  return res;
}

void AG::cruceUniforme(int p1[], int p2[], int h1[], int h2[]){
  vector<int> indices;
  for(int i = 0; i<_qkp.getSize(); ++i){
    indices.push_back(i);
  }
  Random::shuffle(indices);

  for(int i = 0; i<_qkp.getSize(); ++i){
    if(i<_qkp.getSize()/2){
      h1[indices[i]] = p1[indices[i]];
      h2[indices[i]] = p2[indices[i]];
    }
    else{
      h1[indices[i]] = p2[indices[i]];
      h2[indices[i]] = p1[indices[i]];
    }
  }

  // Hacemos las soluciones factibles
  // operadorReparacion(h1);
  // operadorReparacion(h2);
  operadorReparacionGRASP(h1);
  operadorReparacionGRASP(h2);
}

void AG::crucePorcentual(int p1[], double v1, int p2[], double v2, int h1[], int h2[], double p){
  vector<int> indices;
  vector<int> indices2;
  for(int i = 0; i<_qkp.getSize(); ++i){
    indices.push_back(i);
    indices2.push_back(i);
  }
  Random::shuffle(indices);
  Random::shuffle(indices2);
  double porcentaje = getSize()*p/100;
  if(v2>v1){
    porcentaje = getSize()-porcentaje;
  }

  for(int i = 0; i < getSize(); ++i){
    if(i<porcentaje){
      h1[indices[i]] = p1[indices[i]];
      h2[indices2[i]] = p1[indices2[i]];
    }
    else{
      h1[indices[i]] = p2[indices[i]];
      h2[indices2[i]] = p2[indices2[i]];
    }
  }

  // Hacemos las soluciones factibles
  operadorReparacion(h1);
  operadorReparacion(h2);
}

void AG::operadorReparacion(int hijo[]){
  double pesoHijo = calcularPeso(hijo);
  bool anadido = true;
  //Tenemos que eliminar elementos
  if(pesoHijo > _qkp.getCapacidad()){
    while(pesoHijo > _qkp.getCapacidad()){
      eliminarElemento(hijo,pesoHijo);}
  }
  //Tenemos que añadir más elementos
  else{
    while(anadido){anadido = anadirElementoGreedy(hijo, pesoHijo);}
  }
}

void AG::operadorReparacionGRASP(int hijo[]){
  double pesoHijo = calcularPeso(hijo);
  bool anadido = true;
  int aux;
  //Tenemos que eliminar elementos
  if(pesoHijo > _qkp.getCapacidad()){
    while(pesoHijo > _qkp.getCapacidad()){
      aux = GRASP(hijo,pesoHijo,true);
      eliminarElemento(hijo,pesoHijo,aux);
    }
  }
  //Tenemos que añadir más elementos
  else{
    while(anadido){
      aux = GRASP(hijo,pesoHijo,false);
      if(aux!=-1){
        anadirElemento(hijo,pesoHijo,aux);
      }
      else{
        anadido = false;
      }
    }
  }
}

void AG::cambioMutante(int bin[]){
  double peso = calcularPeso(bin);
  vector<int> indexY;
  vector<int> indexN;
  for(int i = 0; i < _qkp.getSize(); ++i){
    if(bin[i]==1)
      indexY.push_back(i);
    else
      indexN.push_back(i);
  }
  Random::shuffle(indexY);
  Random::shuffle(indexN);
  bool sust = false;
  for(int i = 0; i < indexY.size() && !sust; ++i){
    for(int j = 0; j < indexN.size() && !sust; ++j){
      sust = checkSustituir(peso, indexY[i], indexN[j]);
      if(sust){
        sustituirCrom(bin, peso, indexY[i],indexN[j]);
      }
    }
  }
  // Comprobamos si podemos rellenarlo aun mas
  bool anadido = true;
  while(anadido){
    anadido = anadirElementoGreedy(bin, peso);
  }
}

void AG::sustituirCrom(int bin[], double &peso, int pos1, int pos2){
  bin[pos1]=0;
  bin[pos2]=1;
  peso = peso - _qkp.getPeso(pos1) + _qkp.getPeso(pos2);
}

void AG::eliminarElemento(int hijo[], double &peso, int pos){
  hijo[pos] = 0;
  peso -= _qkp.getPeso(pos);
}

void AG::eliminarElemento(int hijo[], double &peso){
  double minRelValor = 0;
  int indexMinRelValor;
  double relValor = 0;
  double valor_aux=0;
  for(int i = 0; i < _qkp.getSize(); ++i){
    if(hijo[i]==1){
      if(relValor==0){
        minRelValor=calcularRelValor(i,hijo);
        indexMinRelValor=i;
      }
      else{
        relValor = calcularRelValor(i,hijo);
        if(minRelValor > relValor){
          indexMinRelValor = i;
          minRelValor = relValor;
        }
      }
    }
  }
  //Eliminamos el peor elemento
  eliminarElemento(hijo,peso,indexMinRelValor);
}

void AG::anadirElemento(int hijo[], double &peso, int pos){
  hijo[pos] = 1;
  peso += _qkp.getPeso(pos);
}

bool AG::anadirElementoGreedy(int hijo[], double &peso){
  bool salida = false;
  vector<int> indices;
  double max = 0;
  double aux = 0;
  int pos_max = 0;

  for(int i = 0; i < _qkp.getSize();++i){
    if(hijo[i]==0)
      indices.push_back(i);
  }

  for(int i = 0; i < indices.size(); ++i){
    if(checkAdd(indices[i], peso, hijo)){
      salida = true;
      aux = valueIfAdded(indices[i],hijo);
      if(aux > max){
        max = aux;
        pos_max = indices[i];
      }
    }
  }

  if(salida == true){
    anadirElemento(hijo, peso, pos_max);
  }
  return salida;
}

int AG::GRASP(int sol[], double &peso, bool minimizar){
  vector<int> indices;
  vector<int> valores;
  double sigma = 0.1;
  if(minimizar){
    for(int i = 0; i < getSize(); ++i){
      if(sol[i] == 1){
        indices.push_back(i);
        valores.push_back(calcularRelValor(i,sol));
      }
    }
  }
  else{
    for(int i = 0; i < getSize(); ++i){
      if(sol[i] == 0){
        if(checkAdd(i, peso, sol)){
          indices.push_back(i);
          valores.push_back(valueIfAdded(i,sol));
        }
      }
    }
  }
  vector<int> aux(indices.size());
  if(minimizar){ //Encontrar peores elementos
    //Ordenamos los elementos de peor a mejor
    iota(aux.begin(), aux.end(), 0);
    sort(aux.begin(), aux.end(),
      [&] (int A, int B) -> bool{
        return valores[A] < valores[B];
      }
    );
    //Nos quedamos con los elementos cuyo valor relativo <= (1+sigma)*valor_peor
    bool found = false;
    for(int i = 1; i < aux.size() && !found; ++i){
      if(valores[aux[i]] > (1+sigma)*valores[aux[0]]){
        found = true;
        aux.erase(aux.begin()+i,aux.end());
      }
    }
  }
  else{ //Encontrar mejores elementos a añadir
    //Ordenamos los elementos de mejor a peor
    iota(aux.begin(), aux.end(), 0);
    sort(aux.begin(), aux.end(),
      [&] (int A, int B) -> bool{
        return valores[A] > valores[B];
      }
    );
    //Nos quedamos con los elementos cuyo valor añadido >= (1-sigma)*valor_mejor
    bool found = false;
    for(int i = 1; i < aux.size() && !found; ++i){
      if(valores[aux[i]] < (1-sigma)*valores[aux[0]]){
        found = true;
        aux.erase(aux.begin()+i,aux.end());
      }
    }
  }

  //Elegimos aleatoriamente uno de los elementos considerados
  if(!aux.empty()){
    Random::shuffle(aux);
    return indices[aux[0]];
  }
  else{
    return -1;
  }
}
