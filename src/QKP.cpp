/**
 * @file QKP.cpp
 * @author Irene Trigueros Lorca
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include "QKP.h"
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
  //_valoresActual.clear();
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
    cerr << "Error en el acceso a la matriz, posición errónea.";
    cerr << "\n " << i << "  "<< j << "\n";
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
    cerr << "Error en el acceso a la matriz, posición errónea.";
    cerr << "\n " << i << "  "<< j << "\n";
    return -1;
  }
  else{
    return _MatrizCoef[i][j];
  }
}

void QKP::setPeso(int i, double peso){
  if(i<0 || i>=_n){
    cerr << "Error en el acceso a la matriz, posición errónea.";
    cerr << "\n " << i << "\n";
  }
  else{
    _pesos[i] = peso;
  }
}

double QKP::getPeso(int i) const{
  if(i<0 || i>=_n){
    cerr << "Error en el acceso a la matriz, posición errónea.";
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

/*void QKP::setDensidad(double den){
  if(den>0){
    _density = den;
  }
  else{
    cerr <<"Error de asignación de densidad: Valor negativo";
  }
}*/

void QKP::setSeed(int seed){
  if(seed>0){
    _seed = seed;
  }
  else{
    cerr <<"Error de asignación de semilla: Valor negativo";
  }
}

/********FUNCIONES ADICIONALES**************/
void QKP::calcularPeso(){
  double aux = 0;
  for(int i = 0; i < _solucion.size(); ++i){
    aux += getPeso(_solucion[i]);
  }
  setPesoSolucion(aux);
}

bool QKP::checkAdd(int pos, double peso, vector<int> sol){
  bool contained = false;
  for(int i = 0; i < sol.size(); ++i){
    if(pos == sol[i])
      contained = true;
  }
  if(contained == true)
    return false;
  else
    return(getPeso(_solucion[pos])+peso < _capacidad);
}

bool QKP::checkAdd(int pos){
  return checkAdd(pos, _pesoSolucion, _solucion);
}

void QKP::addSolucion(int pos, vector<int> sol, double val, double peso){
  for(int i = 0; i<sol.size(); ++i){
    val += getValor(pos,sol[i]);
  }
  val+=getValor(pos,pos);
  peso+=getPeso(pos);
  sol.push_back(pos);
}

void QKP::addSolucion(int pos){
  addSolucion(pos, _solucion, _valorSolucion, _pesoSolucion);
}


/***********ALGORITMOS***********************/
void QKP::RandomQKP(int max){
  vector<int> indicesDatos;
  int contador = 0;
  bool cond = false;
  Random::seed(getSeed());

  for(int i = 0; i < getSize();++i){
    indicesDatos.push_back(i);
  }
  Random::shuffle(indicesDatos);

  for(int i = 0; i<getSize() && !cond; ++i){
    if(checkAdd(indicesDatos[i])){
      addSolucion(indicesDatos[i]);
      contador++;
    }
    if(max!=0 && max==contador){
      cond = true;
    }
  }
}

void QKP::busquedaLocal(const int nEvaluacionesMAX){
  vector<int> indicesDatos;
  Random::seed(getSeed());

  for(int i = 0; i<getSize();++i){
    indicesDatos.push_back(i);
  }
  Random::shuffle(indicesDatos);


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
    cout << "He abierto el fichero\n";
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
                cout << "Leido correctamente\n";
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
  /*if(!_valoresActual.empty()){
    _valoresActual.clear();
  }*/
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
  //_valoresActual = otro.getValoresActual();
  _valorSolucion = otro.getValorSolucion();
  _pesoSolucion = otro.getPesoSolucion();
}
