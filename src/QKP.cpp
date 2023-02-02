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
#include "QKP.h"
#include "AG.h"
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

  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end-start;

  while(duration.count() < tEvaluacionMAX){
    RandomQKP(sol, valor);
    if(valor > bestvalor){
      bestsol = sol;
      bestvalor = valor;
    }
    sol.clear();
    valor = 0;
    end = std::chrono::high_resolution_clock::now();
    duration = end -start;
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

void QKP::AGEU(int numcro, double probm, const double tEvaluacionMAX){
  //Inicializamos la semilla
  Random::seed(getSeed());

  //Variables
  AG ag(*this);
  int numEsperadoCruces=1;
  int matrizSoluciones[numcro][getSize()];
  int matrizHijos[numEsperadoCruces*2][getSize()];
  double valorPadre[numcro];
  double valorHijo[numEsperadoCruces*2];
  int index;
  vector<double> milestones = {1,2,3,5,10,20,30,40,50,60,70,80,90,100};
  for(int i = 0; i < milestones.size(); ++i){
    milestones[i] = tEvaluacionMAX*milestones[i]/100;
  }
  int contador=0;

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
      cout << valorPadre[index] << " , ";
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
    // Cruzamos a los peoresPadres
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
  for(int i = 0; i < getSize(); ++i){
    if(matrizSoluciones[index][i]==1){
      addSolucion(i);
    }
  }
  //return solucion
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
