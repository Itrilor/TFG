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
void QKP::addSolucion(int pos, vector<int> sol, double val, double peso){
  if(!sol.empty()){
    cout << "Vacio?\n";
    for(int i = 0; i<sol.size(); ++i){
      val += getValor(pos,sol[i]);
    }
  }
  val+=getValor(pos,pos);
  peso+=getPeso(pos);
  sol.push_back(pos);
}

void QKP::addSolucion(int pos){
  //addSolucion(pos, _solucion, _valorSolucion, _pesoSolucion);
  if(!_solucion.empty()){
    for(int i = 0; i<_solucion.size(); ++i){
      _valorSolucion += getValor(pos,_solucion[i]);
    }
  }

  _valorSolucion+=getValor(pos,pos);
  _pesoSolucion+=getPeso(pos);
  _solucion.push_back(pos);
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

bool QKP::checkAdd(int pos, double peso, int sol[]){
  vector<int> sol_aux;
  for(int i = 0; i < getSize(); ++i){
    if(sol[i]==1)
      sol_aux.push_back(i);
  }
  return checkAdd(pos, peso, sol_aux);
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

void QKP::generaSeleccionAleatoria(int sol[], double val){
  vector<int> indices;
  for(int i = 0; i < getSize(); ++i){
    sol[i]=0;
    indices.push_back(i);
  }
  val = 0;
  Random::shuffle(indices);
  for(int i = 0; i<getSize(); ++i){
    if(checkAdd(indicesDatos[i], val, sol)){
      sol[indicesDatos[i]]=1;
      val += getPeso(indicesDatos[i]);
    }
  }
}

vector<int> QKP::torneoBinario(int numTorneos, double val[], int tam){
  vector<int> index;
  vector<int> res;

  for(int i = 0; i<tam*2; ++i){
    index.push_back(Random::get(0,tam-1));
  }

  for(int i = 0; i < numTorneos*2; i+=2){
    if(val[index[i]]<val[index[i+1]]){
      res.push_back(index[i]);
    }
    else{
      res.push_back(index[i+1]);
    }
  }
  return res;
}

void QKP::cruceUniforme(int p1[], int p2[], int h1[], int h2[]){
  vector<int> indices;
  for(int i = 0; i<getSize(); ++i){
    indices.push_back(i);
  }
  indices.shuffle();

  for(int i = 0; i<getSize; ++i){
    if(i<getSize/2){
      h1[indices[i]] = p1[indices[i]];
      h2[indices[i]] = p2[indices[i]];
    }
    else{
      h1[indices[i]] = p2[indices[i]];
      h2[indices[i]] = p1[indices[i]];
    }
  }

  // Rellenamos los hijos de forma aleatoria
  //rellenaHijoAleatorio(h1,0.6);
  //rellenaHijoAleatorio(h2,0.4);

  // Hacemos las soluciones factibles
  operadorReparacion(h1);
  operadorReparacion(h2);
}

void QKP::operadorReparacion(int hijo[]){
  double pesoHijo = calcularPeso(hijo);

  //Tenemos que eliminar elementos
  if(pesoHijo > getCapacidad()){

  }
  //Tendríamos que añadir elementos (más adelante)
  else{

  }
}

double QKP::calcularPeso(int sol[]){
  double peso = 0;
  for(int i = 0; i < getSize(); ++i){
    if(sol[i]==1){
      peso += getPeso(i);
    }
  }
  return peso;
}

double QKP::calcularValor(int sol[]){
  double valor = 0;
  vector<int> indices;

  for(int i = 0; i < getSize(); ++i){
    if(sol[i]==1){
      indices.push_back(i);
      for(int j = 0; j<indices.size(); ++j){
        valor+=getValor(i,j);
      }
    }
  }
  return valor;
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
  Random::shuffle(indicesDatos);

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

/*void QKP::busquedaLocal(const int nEvaluacionesMAX){
  vector<int> indicesDatos;
  Random::seed(getSeed());

  for(int i = 0; i<getSize();++i){
    indicesDatos.push_back(i);
  }
  Random::shuffle(indicesDatos);


}*/

void QKP::AGEU(int numcro, double probm, const int tEvaluacionMAX){
  //Inicializamos la semilla
  Random::seed(getSeed());

  //Variables
  int numEsperadoCruces=1;
  int matrizSoluciones[numcro][getSize()];
  int matrizHijos[numEsperadoCruces*2][getSize()];
  double dispersionPadre[numcro];

  //Empezamos el cronómetro
  auto start = std::chrono::high_resolution_clock::now();
  //Creamos la población inicial
  vector<int> indices;
  for(int i = 0; i < numcro; ++i){
    generaSeleccionAleatoria(matrizSoluciones[i],dispersionPadre[i]);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end-start;

  while(duration < tEvaluacionMAX){
    //Estacionario
    /*
    Realizamos 2 torneos binarios aleatorios entre 4 elementos de la población
    para obtener a los 2 padres que vamos a cruzar
    */
    indices = torneoBinario(2, dispersionPadre, numcro);
    while(indices[0] == indices[1]){
      indices = torneoBinario(2, dispersionPadre, numcro);
    }

    for(int i = 0; i < numcro; ++i){
      if(i<numEsperadoCruces*2){
        cruceUniforme(matrizSoluciones[indices[i]],matrizSoluciones[indices[i+1]],
                      matrizHijos[i], matrizHijos[i+1]);
      }
    }

  }
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
  _valorSolucion=0;
  _pesoSolucion=0;
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
