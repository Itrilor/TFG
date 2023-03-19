/**
 * @file QKP.cpp
 * @author Irene Trigueros Lorca
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <chrono>
#include <string>
#include "QKP.h"
#include "AG.h"
#include "AGCEP.h"
#include "CHC.h"
#include "random.hpp"

using namespace std;

using Random = effolkronium::random_static;

/*******CONSTRUCTORES Y DESTRUCTOR********/

QKP::QKP(){
  _n = 0;
  _MatrizCoef = NULL;
  _capacidad = 0;
  _pesos = NULL;
  _seed = 0;
  _solucion.clear();
  _valorSolucion = 0;
  _pesoSolucion = 0;
}

QKP::QKP(int nel){
  _n = nel;
  reservarMemoria(nel);
}

QKP::QKP(const QKP& orig){
  copiar(orig);
}

QKP::~QKP(){
  liberarMemoria();
}

QKP & QKP::operator=(const QKP& orig){
  if(&orig != this){
    liberarMemoria();
    copiar(orig);
  }
  return *this;
}

/*******SETS Y GETS************************/

void QKP::setValor(int i, int j, double valor){
  if(i<0 || i>=_n || j<0 || j>= _n){
    cerr << "Error en el acceso a la matriz, posición errónea. (setValor)";
    cerr << "\n " << i << "      "<< j << "\n";
  }
  else{
    _MatrizCoef[i][j] = valor;
    if(i!=j){
      _MatrizCoef[j][i] = valor;
    }
  }
}

double QKP::getValor(int i, int j) const{
  if(i<0 || i>=_n || j<0 || j>= _n){
    cerr << "Error en el acceso a la matriz, posición errónea. (getValor)";
    cerr << "\n " << i << "      "<< j << "\n";
    return -1;
  }
  else{
    return _MatrizCoef[i][j];
  }
}

void QKP::setPeso(int i, double peso){
  if(i<0 || i>=_n){
    cerr << "Error en el acceso a la matriz, posición errónea (setPeso).";
    cerr << "\n " << i << "\n";
  }
  else{
    _pesos[i] = peso;
  }
}

double QKP::getPeso(int i) const{
  if(i<0 || i>=_n){
    cerr << "Error en el acceso a la matriz, posición errónea (getPeso).";
    cerr << "\n " << i << "\n";
    return -1;
  }
  else{
    return _pesos[i];
  }
}

void QKP::setCapacidad(double cap){
  if(cap>0){
    _capacidad = cap;
  }
  else{
    cerr <<"Error de asignación de capacidad: Valor negativo";
  }
}

void QKP::setSeed(int seed){
  if(seed>0){
    _seed = seed;
  }
  else{
    cerr <<"Error de asignación de semilla: Valor negativo";
  }
}

/********FUNCIONES ADICIONALES**************/
void QKP::addSolucion(int pos, vector<int> &sol, double &val, double &peso){
  if(!sol.empty()){
    for(int i = 0; i<sol.size(); ++i){
      val += getValor(pos,sol[i]);
    }
  }
  val+=getValor(pos,pos);
  peso+=getPeso(pos);
  sol.push_back(pos);
}

void QKP::addSolucion(int pos){
  addSolucion(pos, _solucion, _valorSolucion, _pesoSolucion);
}

void QKP::calcularPeso(){
  double aux = 0;
  for(int i = 0; i < _solucion.size(); ++i){
    aux += getPeso(_solucion[i]);
  }
  setPesoSolucion(aux);
}

bool QKP::checkAdd(int pos, double peso, vector<int> sol){
  bool contained = false;
  if(sol.empty()){
    return true;
  }
  for(int i = 0; i < sol.size(); ++i){
    if(pos == sol[i])
      contained = true;
  }
  if(contained == true)
    return false;
  else{
    return(getPeso(pos)+peso < _capacidad);
  }
}

bool QKP::checkAdd(int pos){
  return checkAdd(pos, _pesoSolucion, _solucion);
}

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

void QKP::vectorToInt(vector<int> sol, int solbin[]){
  for(int i = 0; i < getSize(); ++i){
    solbin[i] = 0;
  }
  for(int i = 0; i < sol.size(); ++i){
    solbin[sol[i]] = 1;
  }
}

double QKP::diversity(vector<vector<int>> pob){
  AG ag(*this);
  CHC chc(ag);

  int auxPob[pob.size()][getSize()];
  for(int i = 0; i < pob.size(); ++i){
    vectorToInt(pob[i],auxPob[i]);
  }
  //cout << "1/d = " << (getSize()*pob.size()*(pob.size()-1)) << "\n";
  double d = (getSize()*pob.size()*(pob.size()-1));
  double aux=0;
  for(int i = 0; i < pob.size()-1; ++i){
    for(int j = i+1; j < pob.size(); ++j){
      aux += chc.distanciaHamming(auxPob[i],auxPob[j]);
    }
  }
  aux = aux*2;
//  cout << " d = " << d << "\n";
  //cout << "diversity = " << aux/d << "\n";
  return aux/d;
}

/***********ALGORITMOS***********************/
void QKP::RandomQKP(vector<int> &sol, double &valor){
  vector<int> indicesDatos;
  int contador = 0;
  double peso = 0;
  bool cond = false;

  for(int i = 0; i < getSize();++i){
    indicesDatos.push_back(i);
  }
  Random::shuffle(indicesDatos);

  for(int i = 0; i<getSize() && !cond; ++i){
    if(checkAdd(indicesDatos[i], peso, sol)){
      addSolucion(indicesDatos[i], sol, valor, peso);
      contador++;
    }
  }
}

void QKP::RandomQKP(double tEvaluacionMAX){
  Random::seed(getSeed());
  vector<int> sol;
  double valor=0;
  vector<int> bestsol;
  double bestvalor=0;
  int contador = 0;

  /*auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end-start;*/

  while(contador < tEvaluacionMAX){
    RandomQKP(sol, valor);
    if(valor > bestvalor){
      bestsol = sol;
      bestvalor = valor;
    }
    sol.clear();
    valor = 0;
    /*end = std::chrono::high_resolution_clock::now();
    duration = end -start;*/
    contador++;
  }
  for(int i = 0; i < bestsol.size(); ++i){
    addSolucion(bestsol[i]);
  }
}

void QKP::Greedy(int max_op){
  vector<int> indicesDatos;
  int contador = 0;
  bool cond = false;
  double aux = 0;
  double max = 0;
  int pos_max = 0;
  int aux_j=0;
  bool inser=false;

  for(int i = 0; i < getSize();++i){
    indicesDatos.push_back(i);
  }
  //Random::shuffle(indicesDatos);

  for(int i = 0; i < getSize() && !cond; ++i){
    for(int j = 0; j < indicesDatos.size(); ++j){
      if(checkAdd(indicesDatos[j])){
        inser=true;
        aux = valueIfAdded(indicesDatos[j]);
        if(aux > max){
          max = aux;
          pos_max = indicesDatos[j];
          aux_j = j;
        }
      }
    }
    if(inser==true){
      addSolucion(pos_max);
      contador++;
      indicesDatos.erase(indicesDatos.begin()+aux_j);
    }
    max = 0;
    if(max_op!=0 && max_op==contador){
      cond=true;
    }
    inser=false;
  }
}

vector<double> QKP::AGEU(int numcro, double probm, const double tEvaluacionMAX, int seed){
  //Inicializamos la semilla
  Random::seed(seed);

  //Variables
  AG ag(*this);
  int numEsperadoCruces=1;
  int matrizSoluciones[numcro][getSize()];
  int matrizHijos[numEsperadoCruces*2][getSize()];
  double valorPadre[numcro];
  double valorHijo[numEsperadoCruces*2];
  int index;
  int contador=0;

  //Gráfico de convergencia
  vector<double> milestones = {1,2,3,5,10,20,30,40,50,60,70,80,90,100};
  for(int i = 0; i < milestones.size(); ++i){
    milestones[i] = tEvaluacionMAX*milestones[i]/100;
  }
  vector<double> resMilestones;

  //Empezamos el cronómetro
  //auto start = std::chrono::high_resolution_clock::now();
  //Creamos la población inicial
  vector<int> indices;
  for(int i = 0; i < numcro; ++i){
    ag.generaSeleccionAleatoria(matrizSoluciones[i],valorPadre[i]);
  }

  //auto end = std::chrono::high_resolution_clock::now();
  //std::chrono::duration<double> duration = end-start;

  //while(duration.count() < tEvaluacionMAX){
  while(contador < tEvaluacionMAX){
    if(contador > milestones[0]){
      index = ag.calcularMejorValor(valorPadre,numcro);
      resMilestones.push_back(valorPadre[index]);
      milestones.erase(milestones.begin());
    }
    //Estacionario
    /*
    Realizamos 2 torneos binarios aleatorios entre 4 elementos de la población
    para obtener a los 2 padres que vamos a cruzar
    */
    indices = ag.torneoBinario(2, valorPadre, numcro);
    while(indices[0] == indices[1]){
      indices = ag.torneoBinario(2, valorPadre, numcro);
    }
    //Generamos aleatoriamente dónde se producen las mutaciones
    vector<int> mutacion;
    int element;
    for(int i = 0; i < (int)numcro*0.1; ++i){
      element = Random::get(0,numcro-1);
      //Si el elemento se encuentra ya en el vector de mutación, se genera otro
      if(std::find(mutacion.begin(), mutacion.end(), element) == mutacion.end()){
          mutacion.push_back(element);
      }
    }
    sort(mutacion.begin(),mutacion.end());
    // Cruzamos a los padres
    for(int i = 0; i < numEsperadoCruces*2; ++i){
      ag.cruceUniforme(matrizSoluciones[indices[i]],matrizSoluciones[indices[i+1]],
                    matrizHijos[i], matrizHijos[i+1]);
      /*if(i==mutacion[0]){
        cambioMutante(matrizHijos[i]);
        mutacion.erase(mutacion.begin());
      }*/
      valorHijo[i] = ag.calcularValor(matrizHijos[i]);
      i++;
      /*if(i==mutacion[0]){
        cambioMutante(matrizHijos[i]);
        mutacion.erase(mutacion.begin());
      }*/
      valorHijo[i] = ag.calcularValor(matrizHijos[i]);
    }
    // Comprobamos si mutamos a qué padres mutar
    while(!mutacion.empty()){
      ag.cambioMutante(matrizSoluciones[mutacion[0]]);
      valorPadre[mutacion[0]] = ag.calcularValor(matrizSoluciones[mutacion[0]]);
      mutacion.erase(mutacion.begin());
    }

    // Tenemos que ver si podemos sustituir
    vector<int> peoresPadres = ag.calcular2Peores(valorPadre, numcro);
    vector<int> peoresHijos = ag.calcular2Peores(valorHijo, numEsperadoCruces*2);
    // Hi supera a Pi
    if(valorPadre[peoresPadres[0]] < valorHijo[peoresHijos[0]] &&
       valorPadre[peoresPadres[1]] < valorHijo[peoresHijos[1]]){
       //Sustituimos Pi por Hi
       for(int i = 0; i < getSize(); ++i){
         matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[0]][i];
         matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[1]][i];
       }
       valorPadre[peoresPadres[0]] = valorHijo[peoresHijos[0]];
       valorPadre[peoresPadres[1]] = valorHijo[peoresHijos[1]];
    }
    // H0 supera a P0 y H1 no supera a P1
    // H0 no supera a P0, pero H1 supera a P0
    else if(valorPadre[peoresPadres[0]] < valorHijo[peoresHijos[1]]){
      //Sustituimos P0 por H1
      for(int i = 0; i < getSize(); ++i){
        matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[1]][i];
      }
      valorPadre[peoresPadres[0]] = valorHijo[peoresHijos[1]];
    }
    //end = std::chrono::high_resolution_clock::now();
    //duration = end -start;
    contador++;
  }
  // Elegir
  index = ag.calcularMejorValor(valorPadre, numcro);
  resMilestones.push_back(valorPadre[index]);
  for(int i = 0; i < getSize(); ++i){
    if(matrizSoluciones[index][i]==1){
      addSolucion(i);
    }
  }
  return resMilestones;
}

vector<double> QKP::GACEP(int numcro, double probm, const int EvaluacionMAX, int seed){
  //Inicializar la semilla
  Random::seed(seed);
  //Variables
  AG ag(*this);
  AGCEP agcep(ag);
  int numEsperadoCruces=1;
  int matrizSoluciones[numcro][getSize()];
  int matrizHijos[numEsperadoCruces*2][getSize()];
  double valorPadre[numcro];
  double valorHijo[numEsperadoCruces*2];
  int index;
  int contador=0;
  int contadorFichero=1;
  bool keepSaving = true;

  //Diversidad
  //vector<vector<int>> pob;


  //Gráfico de convergencia
  vector<double> milestones = {1,2,3,5,10,20,30,40,50,60,70,80,90,100};
  for(int i = 0; i < milestones.size(); ++i){
    milestones[i] = EvaluacionMAX*milestones[i]/100;
  }
  vector<double> resMilestones;


  //Creamos la población inicial y la añadimos al vector para el histograma
  vector<int> indices;
  for(int i = 0; i < numcro; ++i){
    ag.generaSeleccionAleatoria(matrizSoluciones[i],valorPadre[i]);
    agcep.addToHistograma(matrizSoluciones[i],valorPadre[i]);
  }

  while(contador < EvaluacionMAX){
    if(contador > milestones[0]){
      index = ag.calcularMejorValor(valorPadre,numcro);
      resMilestones.push_back(valorPadre[index]);
      //Diversidad
      /*pob.clear();
      for(int i = 0; i < numcro; ++i){
        pob.push_back(ag.intToVector(matrizSoluciones[i]));
      }
      resMilestones.push_back(diversity(pob));*/
      milestones.erase(milestones.begin());
    }
    if(contador!=0 && (contador%50==0)){
      if(contador%100==0){
        keepSaving=true;
        agcep.clearHistograma();
        for(int i = 0; i < numcro; ++i){
          agcep.addToHistograma(matrizSoluciones[i], valorPadre[i]);
        }
      }
      else{
        keepSaving=false;
        string temp_str = "../Histogramas/Histograma"+to_string(contadorFichero);
        char const* number_array = temp_str.c_str();
        agcep.sortHistograma();
        agcep.saveFichero(number_array);
        temp_str = "../Histogramas/Porcentajes"+to_string(contadorFichero);
        char const* porcentaje_array = temp_str.c_str();
        agcep.writeElementsPercentages(20,porcentaje_array);
        temp_str = "../Histogramas/Elementos"+to_string(contadorFichero);
        char const* elementos_array = temp_str.c_str();
        agcep.writeBestWorstElements(20, elementos_array);
        contadorFichero++;
      }
    }
    //Estacionario
    /*
    Realizamos 2 torneos binarios aleatorios entre 4 elementos de la población
    para obtener a los 2 padres que vamos a cruzar
    */
    indices = ag.torneoBinario(2, valorPadre, numcro);
    while(indices[0] == indices[1]){
      indices = ag.torneoBinario(2, valorPadre, numcro);
    }
    //Generamos aleatoriamente dónde se producen las mutaciones
    vector<int> mutacion;
    int element;
    for(int i = 0; i < (int)numcro*0.1; ++i){
      element = Random::get(0,numcro-1);
      //Si el elemento se encuentra ya en el vector de mutación, se genera otro
      if(std::find(mutacion.begin(), mutacion.end(), element) == mutacion.end()){
          mutacion.push_back(element);
      }
    }
    sort(mutacion.begin(),mutacion.end());
    // Cruzamos a los padres
    for(int i = 0; i < numEsperadoCruces*2; ++i){
      agcep.cruceUniforme(matrizSoluciones[indices[i]],matrizSoluciones[indices[i+1]],
                    matrizHijos[i], matrizHijos[i+1], keepSaving);
      /*agcep.crucePorcentual(matrizSoluciones[indices[i]], valorPadre[indices[i]],
                    matrizSoluciones[indices[i+1]], valorPadre[indices[i+1]],
                    matrizHijos[i], matrizHijos[i+1], keepSaving,80);*/

      /*if(i==mutacion[0]){
        cambioMutante(matrizHijos[i]);
        mutacion.erase(mutacion.begin());
      }*/
      valorHijo[i] = ag.calcularValor(matrizHijos[i]);
      if(keepSaving==true){
        agcep.addToHistograma(matrizHijos[i],valorHijo[i]);
      }
      i++;
      /*if(i==mutacion[0]){
        cambioMutante(matrizHijos[i]);
        mutacion.erase(mutacion.begin());
      }*/
      valorHijo[i] = ag.calcularValor(matrizHijos[i]);
      if(keepSaving==true){
        agcep.addToHistograma(matrizHijos[i],valorHijo[i]);
      }
    }
    // Comprobamos si mutamos a qué padres mutar
    while(!mutacion.empty()){
      agcep.cambioMutante(matrizSoluciones[mutacion[0]],keepSaving);
      valorPadre[mutacion[0]] = ag.calcularValor(matrizSoluciones[mutacion[0]]);
      if(keepSaving==true){
        agcep.addToHistograma(matrizSoluciones[mutacion[0]],valorPadre[mutacion[0]]);
      }
      mutacion.erase(mutacion.begin());
    }

    // Tenemos que ver si podemos sustituir
    vector<int> peoresPadres = ag.calcular2Peores(valorPadre, numcro);
    vector<int> peoresHijos = ag.calcular2Peores(valorHijo, numEsperadoCruces*2);
    // Hi supera a Pi
    if(valorPadre[peoresPadres[0]] < valorHijo[peoresHijos[0]] &&
       valorPadre[peoresPadres[1]] < valorHijo[peoresHijos[1]]){
       //Sustituimos Pi por Hi
       for(int i = 0; i < getSize(); ++i){
         matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[0]][i];
         matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[1]][i];
       }
       valorPadre[peoresPadres[0]] = valorHijo[peoresHijos[0]];
       valorPadre[peoresPadres[1]] = valorHijo[peoresHijos[1]];
    }
    // H0 supera a P0 y H1 no supera a P1
    // H0 no supera a P0, pero H1 supera a P0
    else if(valorPadre[peoresPadres[0]] < valorHijo[peoresHijos[1]]){
      //Sustituimos P0 por H1
      for(int i = 0; i < getSize(); ++i){
        matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[1]][i];
      }
      valorPadre[peoresPadres[0]] = valorHijo[peoresHijos[1]];
    }
    contador++;
  }
  // Elegir
  index = ag.calcularMejorValor(valorPadre, numcro);
  resMilestones.push_back(valorPadre[index]);
  for(int i = 0; i < getSize(); ++i){
    if(matrizSoluciones[index][i]==1){
      addSolucion(i);
    }
  }
  //return solucionç
  //Diversidad
  /*pob.clear();
  for(int i = 0; i < numcro; ++i){
    pob.push_back(ag.intToVector(matrizSoluciones[i]));
  }
  resMilestones.push_back(diversity(pob));*/
  return resMilestones;
}

vector<double> QKP::CHCGA(int numcro, const int EvaluacionMAX, int seed){
  //Inizializamos la semilla
  Random::seed(seed);

  //Variables
  AG ag(*this);
  CHC chc(ag);

  int matrizSoluciones[numcro][getSize()];
  double valorPadre[numcro];

  int index;
  int contador = 0;
  int threshold = getSize()/4;

  //Gráfico de convergencia
  vector<double> milestones = {1,2,3,5,10,20,30,40,50,60,70,80,90,100};
  for(int i = 0; i < milestones.size(); ++i){
    milestones[i] = EvaluacionMAX*milestones[i]/100;
  }
  vector<double> resMilestones;
  vector<int> posiblesPadres;
  vector<int> indicesParejas;
  vector<int> aux;

  //Empezamos el cronómetro
  auto start = std::chrono::high_resolution_clock::now();

  //Creamos la población inicial
  vector<int> indices;
  for(int i = 0; i < numcro; ++i){
    ag.generaSeleccionAleatoria(matrizSoluciones[i],valorPadre[i]);
    indices.push_back(i);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end-start;

  //Empieza el algoritmo
  //while(duration.count() < EvaluacionMAX){
  while(contador < EvaluacionMAX){
    //cout << contador << "\n";
    //Guarda la mejor solucion del porcentaje de ejecución
    //while(duration.count() > milestones[0]){
    if(contador > milestones[0]){
      index = ag.calcularMejorValor(valorPadre,numcro);
      resMilestones.push_back(valorPadre[index]);
      milestones.erase(milestones.begin());
    }
    //Elegimos aleatoriamente los posibles padres
    for(int i = 0; i < numcro; ++i){
      posiblesPadres.push_back(Random::get(0, numcro));
    }

    bool cruce;
    for(int i = 1; i < posiblesPadres.size(); ++i){
      cruce = chc.seleccion(threshold, matrizSoluciones[posiblesPadres[i-1]], matrizSoluciones[posiblesPadres[i]]);
      if(cruce){
        indicesParejas.push_back(posiblesPadres[i-1]);
        indicesParejas.push_back(posiblesPadres[i]);
        i++;
      }
    }
    posiblesPadres.clear();

    if(!indicesParejas.empty()){
      int matrizHijos[indicesParejas.size()][getSize()];
      double valorHijo[indicesParejas.size()];

      // Cruzamos a los padres
      for(int i = 0; i < indicesParejas.size(); ++i){
        chc.cruceHUX(matrizSoluciones[indicesParejas[i]], matrizSoluciones[indicesParejas[i+1]],
                     matrizHijos[i], matrizHijos[i+1]);
        valorHijo[i] = ag.calcularValor(matrizHijos[i]);
        i++;
        valorHijo[i] = ag.calcularValor(matrizHijos[i]);
      }

      // En CHC no se muta

      // Enfrentamos a la población inicial con los hijos para quedarnos con los mejores
      aux = chc.enfrentamiento(valorPadre,valorHijo,numcro,indicesParejas.size());
      indicesParejas.clear();

      int auxSoluciones[numcro][getSize()];
      double auxValor[numcro];
      //Establecemos la nueva población
      for(int i = 0; i < numcro; ++i){
        if(aux[i]<numcro){
          for(int j = 0; j < getSize(); ++j){
            auxSoluciones[i][j] = matrizSoluciones[aux[i]][j];
          }
          auxValor[i]=valorPadre[aux[i]];
        }
        else{
          for(int j = 0; j < getSize(); ++j){
            auxSoluciones[i][j] = matrizHijos[aux[i]-numcro][j];
          }
          auxValor[i]=valorHijo[aux[i]-numcro];
        }
      }

      //Sustituimos la población inicial por la nueva
      for(int i = 0; i < numcro; ++i){
        for(int j = 0; j < getSize(); ++j){
          matrizSoluciones[i][j] = auxSoluciones[i][j];
        }
        valorPadre[i] = auxValor[i];
      }
    }

    end = std::chrono::high_resolution_clock::now();
    duration = end -start;
    contador++;
  }
  // Elegir
  index = ag.calcularMejorValor(valorPadre, numcro);
  while(!milestones.empty()){
    resMilestones.push_back(valorPadre[index]);
    milestones.erase(milestones.begin());
  }

  for(int i = 0; i < getSize(); ++i){
    if(matrizSoluciones[index][i]==1){
      addSolucion(i);
    }
  }
  return resMilestones;
}

vector<double> QKP::GACEPCHC(int numcro, double probm, const int EvaluacionMAX, int seed){
  //Inicializar la semilla
  Random::seed(seed);
  //Algoritmos base
  AG ag(*this);
  CHC chc(ag);
  AGCEP agcep(ag,chc);
  //Variables

  int numEsperadoCruces=1;
  int matrizSoluciones[numcro][getSize()];
  int matrizHijos[numEsperadoCruces*2][getSize()];
  double valorPadre[numcro];
  double valorHijo[numEsperadoCruces*2];
  int index;
  int contador=0;
  int contadorFichero=1;
  bool keepSaving = true;
  int threshold = getSize()/4;

  vector<int> indicesParejas;

  //Gráfico de convergencia
  vector<double> milestones = {1,2,3,5,10,20,30,40,50,60,70,80,90,100};
  for(int i = 0; i < milestones.size(); ++i){
    milestones[i] = EvaluacionMAX*milestones[i]/100;
  }
  vector<double> resMilestones;

  //Creamos la población inicial y la añadimos al vector para el histograma
  vector<int> indices;
  for(int i = 0; i < numcro; ++i){
    ag.generaSeleccionAleatoria(matrizSoluciones[i],valorPadre[i]);
    agcep.addToHistograma(matrizSoluciones[i],valorPadre[i]);
  }

  while(contador < EvaluacionMAX){
    if(contador > milestones[0]){
      index = ag.calcularMejorValor(valorPadre,numcro);
      resMilestones.push_back(valorPadre[index]);
      milestones.erase(milestones.begin());
    }
    if(contador!=0 && (contador%50==0)){
      if(contador%100==0){
        keepSaving=true;
        agcep.clearHistograma();
        for(int i = 0; i < numcro; ++i){
          agcep.addToHistograma(matrizSoluciones[i], valorPadre[i]);
        }
      }
      else{
        keepSaving=false;
        string temp_str = "../Histogramas/Histograma"+to_string(contadorFichero);
        char const* number_array = temp_str.c_str();
        agcep.sortHistograma();
        agcep.saveFichero(number_array);
        temp_str = "../Histogramas/Porcentajes"+to_string(contadorFichero);
        char const* porcentaje_array = temp_str.c_str();
        agcep.writeElementsPercentages(20,porcentaje_array);
        temp_str = "../Histogramas/Elementos"+to_string(contadorFichero);
        char const* elementos_array = temp_str.c_str();
        agcep.writeBestWorstElements(20, elementos_array);
        contadorFichero++;
      }
    }

    //Buscamos padres a los que cruzar
    for(int i = 0; i < numcro; ++i){
      indices.push_back(Random::get(0, numcro));
    }
    bool cruce = false;
    int contadorParejas = 0;
    for(int i = 1; i < indices.size() && (contadorParejas<numEsperadoCruces); ++i){
      cruce = chc.seleccion(threshold, matrizSoluciones[indices[i-1]], matrizSoluciones[indices[i]]);
      if(cruce){
        indicesParejas.push_back(indices[i-1]);
        indicesParejas.push_back(indices[i]);
        i++;
        contadorParejas++;
      }
    }
    indices.clear();

    //Generamos aleatoriamente dónde se producen las mutaciones
    vector<int> mutacion;
    int element;
    for(int i = 0; i < (int)numcro*0.1; ++i){
      element = Random::get(0,numcro-1);
      //Si el elemento se encuentra ya en el vector de mutación, se genera otro
      if(std::find(mutacion.begin(), mutacion.end(), element) == mutacion.end()){
          mutacion.push_back(element);
      }
    }
    sort(mutacion.begin(),mutacion.end());

    // Cruzamos a los padres
    for(int i = 0; i < contadorParejas*2; ++i){
      agcep.cruceHUX(matrizSoluciones[indicesParejas[i]],matrizSoluciones[indicesParejas[i+1]],
                    matrizHijos[i], matrizHijos[i+1], keepSaving);

      valorHijo[i] = ag.calcularValor(matrizHijos[i]);
      if(keepSaving==true){
        agcep.addToHistograma(matrizHijos[i],valorHijo[i]);
      }
      i++;
      valorHijo[i] = ag.calcularValor(matrizHijos[i]);
      if(keepSaving==true){
        agcep.addToHistograma(matrizHijos[i],valorHijo[i]);
      }
    }
    indicesParejas.clear();
    // Comprobamos si mutamos a qué padres mutar
    while(!mutacion.empty()){
      agcep.cambioMutante(matrizSoluciones[mutacion[0]],keepSaving);
      valorPadre[mutacion[0]] = ag.calcularValor(matrizSoluciones[mutacion[0]]);
      if(keepSaving==true){
        agcep.addToHistograma(matrizSoluciones[mutacion[0]],valorPadre[mutacion[0]]);
      }
      mutacion.erase(mutacion.begin());
    }

    // Tenemos que ver si podemos sustituir
    if(contadorParejas!=0){
      vector<int> peoresPadres = ag.calcular2Peores(valorPadre, numcro);
      vector<int> peoresHijos = ag.calcular2Peores(valorHijo, numEsperadoCruces*2);
      // Hi supera a Pi
      if(valorPadre[peoresPadres[0]] < valorHijo[peoresHijos[0]] &&
         valorPadre[peoresPadres[1]] < valorHijo[peoresHijos[1]]){
         //Sustituimos Pi por Hi
         for(int i = 0; i < getSize(); ++i){
           matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[0]][i];
           matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[1]][i];
         }
         valorPadre[peoresPadres[0]] = valorHijo[peoresHijos[0]];
         valorPadre[peoresPadres[1]] = valorHijo[peoresHijos[1]];
      }
      // H0 supera a P0 y H1 no supera a P1
      // H0 no supera a P0, pero H1 supera a P0
      else if(valorPadre[peoresPadres[0]] < valorHijo[peoresHijos[1]]){
        //Sustituimos P0 por H1
        for(int i = 0; i < getSize(); ++i){
          matrizSoluciones[peoresPadres[0]][i] = matrizHijos[peoresHijos[1]][i];
        }
        valorPadre[peoresPadres[0]] = valorHijo[peoresHijos[1]];
      }
    }

    contador++;
  }
  // Elegir
  index = ag.calcularMejorValor(valorPadre, numcro);
  resMilestones.push_back(valorPadre[index]);
  for(int i = 0; i < getSize(); ++i){
    if(matrizSoluciones[index][i]==1){
      addSolucion(i);
    }
  }
  //return solucionç
  return resMilestones;
}

/********LEER FICHERO DE DATOS*****************/
bool QKP::leerFicheroDatos(const char* fDatos){
  ifstream fin;
  bool salida = false;
  string aux;
  vector<string> ele;
  int n;
  double valor;

  fin.open(fDatos);
  if(fin){
    //cout << "He abierto el fichero\n";
    fin >> aux;
    if(fin){
      string part;
      stringstream sstream(aux);
      while(getline(sstream,part,'_')){
        ele.push_back(part);
      }
      setSeed(stoi(ele.back()));
      fin >> n;
      if(fin){
        setSize(n);
        reservarMemoria(n);
        for(int i = 0; i < n; ++i){
          fin >> valor;
          setValor(i,i,valor);
        }
        if(fin){
          for(int i = 0; i < n-1;++i){
            for(int j = i+1; j<n;++j){
              fin >> valor;
              setValor(i,j,valor);
            }
          }
          if(fin){
            fin >> n; //0
            fin >> valor;
            if(fin){
              setCapacidad(valor);
              for(int i = 0; i < _n; ++i){
                fin >> valor;
                setPeso(i,valor);
              }
              if(fin){
                //cout << "Leido correctamente\n";
                salida = true;
              }
              else cerr << "Error. No se ha podido cargar el archivo. Pesos\n";
            }
            else cerr << "Error. No se ha podido cargar el archivo. Capacidad\n";
          }
          else cerr << "Error. No se ha podido cargar el archivo. Matriz\n";
        }
        else cerr << "Error. No se ha podido cargar el archivo. Diagonal\n";
      }
      else cerr << "Error. No se ha podido cargar el archivo. Segunda linea\n";
    }
    else cerr << "Error. No se ha podido cargar el archivo. Primera linea\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}

/********RESERVAR Y LIBERAR MEMORIA************/
void QKP::reservarMemoria(int n){
  liberarMemoria();
  setSize(n);
  _MatrizCoef = new double*[n];
  _pesos = new double[n];
  for(int i = 0; i<n; ++i){
    _MatrizCoef[i] = new double[n];
    _pesos[i]=0;
    for(int j = 0; j < n; ++j){
      _MatrizCoef[i][j] = 0;
    }
  }
}

void QKP::liberarMemoria(){
  if(_MatrizCoef != NULL){
    delete [] _MatrizCoef;
    _MatrizCoef = NULL;
  }
  if(_pesos != NULL){
    delete [] _pesos;
    _pesos = NULL;
  }
  if(!_solucion.empty()){
    _solucion.clear();
  }
  _valorSolucion=0;
  _pesoSolucion=0;
}

void QKP::copiar(const QKP& otro){
  reservarMemoria(otro.getSize());
  for(int i = 0; i <getSize(); ++i){
    for(int j = 0; j < getSize(); ++j){
      setValor(i,j,otro.getValor(i,j));
    }
    setPeso(i,otro.getPeso(i));
  }
  setCapacidad(otro.getCapacidad());
  setSeed(otro.getSeed());
  _solucion = otro.getSolucion();
  _valorSolucion = otro.getValorSolucion();
  _pesoSolucion = otro.getPesoSolucion();
}
