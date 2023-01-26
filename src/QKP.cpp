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
void QKP::addSolucion(int pos, vector<int> &sol, double &val, double &peso){
  if(!sol.empty()){
    //cout << "Vacio?\n";
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
  /*if(!_solucion.empty()){
    for(int i = 0; i<_solucion.size(); ++i){
      _valorSolucion += getValor(pos,_solucion[i]);
    }
  }

  _valorSolucion+=getValor(pos,pos);
  _pesoSolucion+=getPeso(pos);
  _solucion.push_back(pos);*/
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
  /*vector<int> sol_aux;
  for(int i = 0; i < getSize(); ++i){
    if(sol[i]==1)
      sol_aux.push_back(i);
  }*/
  return checkAdd(pos, peso, intToVector(sol));
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

double QKP::valueIfAdded(int pos, int sol[]){
  return valueIfAdded(pos, intToVector(sol));
}

double QKP::valueIfAdded(int pos){
  return valueIfAdded(pos, _solucion);
}

void QKP::generaSeleccionAleatoria(int sol[], double &val){
  vector<int> indices;
  for(int i = 0; i < getSize(); ++i){
    sol[i]=0;
    indices.push_back(i);
  }
  double peso = 0;
  Random::shuffle(indices);
  for(int i = 0; i<getSize(); ++i){
    if(checkAdd(indices[i], peso, sol)){
      sol[indices[i]]=1;
      peso += getPeso(indices[i]);
    }
  }
  val = calcularValor(sol);
}

vector<int> QKP::torneoBinario(int numTorneos, double val[], int tam){
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

void QKP::cruceUniforme(int p1[], int p2[], int h1[], int h2[]){
  vector<int> indices;
  for(int i = 0; i<getSize(); ++i){
    indices.push_back(i);
  }
  Random::shuffle(indices);

  for(int i = 0; i<getSize(); ++i){
    if(i<getSize()/2){
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
  bool anadido = true;
  //Tenemos que eliminar elementos
  /*while(getCapacidad() < pesoHijo){
    anadido = false;
    eliminarElemento(hijo,pesoHijo);
  }
  while(anadido){
    anadido = anadirElementoGreedy(hijo,pesoHijo);
  }*/
  if(pesoHijo > getCapacidad()){
    while(pesoHijo > getCapacidad()){
      eliminarElemento(hijo,pesoHijo);}

  }
  //Tendríamos que añadir elementos (más adelante)
  else{
    while(anadido){anadido = anadirElementoGreedy(hijo, pesoHijo);}
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
        valor+=getValor(i,indices[j]);
      }
    }
  }
  return valor;
}

double QKP::calcularRelValor(int index, int sol[]){
  double relValor=0;
  if(sol[index==1]){
    for(int i = 0; i < getSize(); ++i){
      if(sol[i]==1){
        relValor+=getValor(i,index);
      }
    }
  }
  relValor = relValor/getPeso(index);
  return relValor;
}

void QKP::eliminarElemento(int hijo[], double &peso){
  double minRelValor = 0;
  int indexMinRelValor;
  double relValor = 0;
  double valor_aux=0;
  for(int i = 0; i < getSize(); ++i){
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
  hijo[indexMinRelValor]=0;
  peso = peso - getPeso(indexMinRelValor);
}

void QKP::anadirElemento(int hijo[], double &peso, int pos){
  hijo[pos] = 1;
  peso += getPeso(pos);
}

bool QKP::anadirElementoGreedy(int hijo[], double &peso){
  bool salida = false;
  vector<int> indices;
  double max = 0;
  double aux = 0;
  int pos_max = 0;

  for(int i = 0; i < getSize();++i){
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

void QKP::cambioMutante(int bin[]){
  double peso = calcularPeso(bin);
  vector<int> indexY;
  vector<int> indexN;
  for(int i = 0; i < getSize(); ++i){
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

bool QKP::checkSustituir(double peso, int pos1, int pos2){
  return (getCapacidad() > (peso-getPeso(pos1)+getPeso(pos2)));
}

void QKP::sustituirCrom(int bin[], double &peso, int pos1, int pos2){
  bin[pos1]=0;
  bin[pos2]=1;
  peso = peso - getPeso(pos1) + getPeso(pos2);
}

int QKP::calcularMejorValor(double valor[], int tam){
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

int QKP::calcularPeorValor(double valor[], int tam){
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

vector<int> QKP::calcular2Peores(double valor[], int tam){
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

vector<int> QKP::intToVector(int sol[]){
  vector<int> solAux;
  for(int i = 0; i < getSize(); ++i){
    if(sol[i]==1){
      solAux.push_back(i);
    }
  }
  return solAux;
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
  int numEsperadoCruces=1;
  int matrizSoluciones[numcro][getSize()];
  int matrizHijos[numEsperadoCruces*2][getSize()];
  double valorPadre[numcro];
  double valorHijo[numEsperadoCruces*2];

  //Empezamos el cronómetro
  auto start = std::chrono::high_resolution_clock::now();
  //Creamos la población inicial
  vector<int> indices;
  for(int i = 0; i < numcro; ++i){
    generaSeleccionAleatoria(matrizSoluciones[i],valorPadre[i]);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end-start;

  while(duration.count() < tEvaluacionMAX){
    //Estacionario
    /*
    Realizamos 2 torneos binarios aleatorios entre 4 elementos de la población
    para obtener a los 2 padres que vamos a cruzar
    */
    indices = torneoBinario(2, valorPadre, numcro);
    while(indices[0] == indices[1]){
      indices = torneoBinario(2, valorPadre, numcro);
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
      cruceUniforme(matrizSoluciones[indices[i]],matrizSoluciones[indices[i+1]],
                    matrizHijos[i], matrizHijos[i+1]);
      /*if(i==mutacion[0]){
        cambioMutante(matrizHijos[i]);
        mutacion.erase(mutacion.begin());
      }*/
      valorHijo[i] = calcularValor(matrizHijos[i]);
      i++;
      /*if(i==mutacion[0]){
        cambioMutante(matrizHijos[i]);
        mutacion.erase(mutacion.begin());
      }*/
      valorHijo[i] = calcularValor(matrizHijos[i]);
    }
    // Comprobamos si mutamos a qué padres mutar
    while(!mutacion.empty()){
      cambioMutante(matrizSoluciones[mutacion[0]]);
      valorPadre[mutacion[0]] = calcularValor(matrizSoluciones[mutacion[0]]);
      mutacion.erase(mutacion.begin());
    }

    // Tenemos que ver si podemos sustituir
    // POR COMPLETAR?? => calcular2Peores
    vector<int> peoresPadres = calcular2Peores(valorPadre, numcro);
    vector<int> peoresHijos = calcular2Peores(valorHijo, numEsperadoCruces*2);
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
    end = std::chrono::high_resolution_clock::now();
    duration = end -start;
  }
  // Elegir
  int index = calcularMejorValor(valorPadre, numcro);
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
