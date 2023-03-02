/**
 * @file GACEP.cpp
 * @author Irene Trigueros Lorca
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::fill
#include "AGCEP.h"
#include "random.hpp"

using namespace std;

using Random = effolkronium::random_static;
const int littlePercentage = 15;

/************CONSTRUCTORES************/

AGCEP::AGCEP(AG &ag){
  _ag = ag;
}

/***********SETS****************/
void AGCEP::setBestElements(double perelem){
  _bestElements = getBestElements(getBestElements(perelem), getWorstElements(perelem));
}

void AGCEP::setWorstElements(double perelem){
  _worstElements = getWorstElements(getBestElements(perelem), getWorstElements(perelem));
}

/*********FUNCIONES RELACIONADAS HISTOGRAMA********/

void AGCEP::addToHistograma(int sol[], double valor){
  vector<int> aux = _ag.intToVector(sol);
  bool presente = false;
  if(!_solucionesHistograma.empty()){
    for(int i = 0; i < _solucionesHistograma.size() && !presente; ++i){
      if(aux==_solucionesHistograma[i]){
        presente = true;
      }
    }
    if(!presente){
      _solucionesHistograma.push_back(aux);
      _valoresHistograma.push_back(valor);
    }
  }
  else{
    _solucionesHistograma.push_back(aux);
    _valoresHistograma.push_back(valor);
  }
}

void AGCEP::clearHistograma(){
  _solucionesHistograma.clear();
  _valoresHistograma.clear();
  _bestElements.clear();
  _worstElements.clear();
}

void AGCEP::sortHistograma(){
  //https://stackoverflow.com/questions/37368787/c-sort-one-vector-based-on-another-one
  vector<int> indices(_solucionesHistograma.size());
  iota(indices.begin(), indices.end(), 0);
  sort(indices.begin(), indices.end(),
    [&] (int A, int B) -> bool{
      return _valoresHistograma[A] > _valoresHistograma[B];
    }
  );
  //Tenemos ordenados los índices
  //Creamos vectores auxiliares donde estén ordenados y luego igualamos
  vector<vector<int>> soluciones_aux;
  vector<double> valores_aux;
  for(int i = 0; i < indices.size(); ++i){
    soluciones_aux.push_back(_solucionesHistograma[indices[i]]);
    valores_aux.push_back(_valoresHistograma[indices[i]]);
  }
  // Sustituimos el orden original por el que hemos obtenido
  _solucionesHistograma.clear();
  _valoresHistograma.clear();
  _solucionesHistograma = soluciones_aux;
  _valoresHistograma = valores_aux;
}

vector<int> AGCEP::getBestElements(double perelem){
  vector<int> indices(getSize());
  vector<double> percentages(indices.size());
  iota(indices.begin(), indices.end(), 0);
  fill(percentages.begin(), percentages.end(), 0);
  getBestPercentages(perelem, percentages, indices);
  bool alreadyLittle = false;
  int index = 0;
  for(int i = 0; i < percentages.size() && !alreadyLittle; ++i){
    if(percentages[i]<littlePercentage){
      alreadyLittle=true;
      index = i;
    }
  }
  indices.erase(indices.begin()+index, indices.end());
  return indices;
}

vector<int> AGCEP::getWorstElements(double perelem){
  vector<int> indices(getSize());
  vector<double> percentages(indices.size());
  iota(indices.begin(), indices.end(), 0);
  fill(percentages.begin(), percentages.end(), 0);
  getWorstPercentages(perelem, percentages, indices);
  bool alreadyLittle = false;
  int index = 0;
  for(int i = 0; i < percentages.size() && !alreadyLittle; ++i){
    if(percentages[i]<littlePercentage){
      alreadyLittle=true;
      index = i;
    }
  }
  indices.erase(indices.begin()+index, indices.end());
  return indices;
}

vector<int> AGCEP::getBestElements(vector<int> best, vector<int> worst){
  bool found = false;
  for(int i = 0; i < best.size(); ++i){
    for(int j = 0; j < worst.size() && !found; ++j){
      if(best[i]==worst[j]){
        found = true;
        best.erase(best.begin()+i);
        worst.erase(worst.begin()+j);
        --i;
      }
    }
    found = false;
  }
  return best;
}

vector<int> AGCEP::getWorstElements(vector<int> best, vector<int> worst){
  bool found = false;
  for(int i = 0; i < worst.size(); ++i){
    for(int j = 0; j < best.size() && !found; ++j){
      if(worst[i]==best[j]){
        found = true;
        best.erase(best.begin()+j);
        worst.erase(worst.begin()+i);
        --i;
      }
    }
    found = false;
  }
  return worst;
}

void AGCEP::getBestPercentages(double perelem, vector<double> &perc, vector<int> &indices){
  int max = round(_solucionesHistograma.size()*perelem/100);
  for(int i = 0; i < max; ++i){
    for(int j = 0; j < _solucionesHistograma[i].size(); ++j){
      perc[_solucionesHistograma[i][j]]++;
    }
  }
  for(int i = 0; i < perc.size(); ++i){
    perc[i] = perc[i]*100/max;
  }

  vector<int> newindices(indices.size());
  iota(newindices.begin(), newindices.end(), 0);
  sort(newindices.begin(), newindices.end(),
    [&] (int A, int B) -> bool{
      return perc[A] > perc[B];
    }
  );
  //Tenemos ordenados los índices
  //Creamos vectores auxiliares donde estén ordenados y luego igualamos
  vector<int> index;
  vector<double> percentages;
  for(int i = 0; i < indices.size(); ++i){
    index.push_back(newindices[i]);
    percentages.push_back(perc[newindices[i]]);
  }
  // Sustituimos el orden original por el que hemos obtenido
  perc.clear();
  indices.clear();
  perc = percentages;
  indices = index;
}

void AGCEP::getWorstPercentages(double perelem, vector<double> &perc, vector<int> &indices){
  int max = round(_solucionesHistograma.size()*perelem/100);
  int min = _solucionesHistograma.size()- max;
  for(int i = min; i < _solucionesHistograma.size(); ++i){
    for(int j = 0; j < _solucionesHistograma[i].size(); ++j){
      perc[_solucionesHistograma[i][j]]++;
    }
  }
  for(int i = 0; i < perc.size(); ++i){
    perc[i] = perc[i]*100/max;
  }

  vector<int> newindices(indices.size());
  iota(newindices.begin(), newindices.end(), 0);
  sort(newindices.begin(), newindices.end(),
    [&] (int A, int B) -> bool{
      return perc[A] > perc[B];
    }
  );
  //Tenemos ordenados los índices
  //Creamos vectores auxiliares donde estén ordenados y luego igualamos
  vector<int> index;
  vector<double> percentages;
  for(int i = 0; i < indices.size(); ++i){
    index.push_back(newindices[i]);
    percentages.push_back(perc[newindices[i]]);
  }
  // Sustituimos el orden original por el que hemos obtenido
  perc.clear();
  indices.clear();
  perc = percentages;
  indices = index;
}

vector<int> AGCEP::getMehElements(){
  vector<int> index;
  vector<int>::iterator it;
  bool mehEle;
  for(int i = 0; i < getSize(); ++i){
    mehEle = true;
    it = find(_bestElements.begin(), _bestElements.end(), i);
    if(it==_bestElements.end()){
      mehEle = false;
    }
    if(mehEle){
      it = find(_worstElements.begin(), _worstElements.end(), i);
      if(it==_worstElements.end()){
        mehEle = false;
      }
      if(mehEle){
        index.push_back(i);
      }
    }
  }
  return index;
}

/******MODIFICACIONES DEL AG************/
void AGCEP::cambioMutante(int bin[], bool etapa){
  if(etapa){
    _ag.cambioMutante(bin);
  }
  else{
    double peso = _ag.calcularPeso(bin);
    vector<int> indexY, indexN, bestN, mediumN, worstN, bestY, mediumY, worstY;
    getYNfromList(bin, indexY, indexN, worstN, mediumN, bestN, bestY, mediumY, worstY);
    //resultadosHistogramaMutacion
    /*bool sust = false;
    for(int i = 0; i < indexY.size() && !sust; ++i){
      for(int j = 0; j < indexN.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, indexY[i], indexN[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, indexY[i],indexN[j]);
          indexY.erase(indexY.begin()+i);
          indexN.erase(indexN.begin()+j);
        }
      }
    }*/
    //resultadosHistogramaMutacion2
    sustituirCrom(bin,peso,worstN,mediumN,bestN,bestY,mediumY,worstY);
    bool anadido = true;
    if(!bestN.empty()){
      while(anadido){
        anadido = anadirFromListaGreedy(bin, bestN, peso);
      }
    }
    anadido=true;
    if(!mediumN.empty()){
      while(anadido){
        anadido = anadirFromListaGreedy(bin, mediumN, peso);
      }
    }
    anadido=true;
    if(!worstN.empty()){
      while(anadido){
        anadido = anadirFromListaGreedy(bin, worstN, peso);
      }
    }
  }
}

void AGCEP::getYNfromList(int bin[], vector<int> &yes, vector<int> &no, vector<int> &wn, vector<int> &mn, vector<int> &bn,vector<int> &by, vector<int> &my, vector<int> &wy){
  wn.clear(); mn.clear(); bn.clear();
  by.clear(); my.clear(); wy.clear();
  vector<int>::iterator it;
  for(int i = 0; i < getSize(); ++i){
    if(bin[i]==1){ //Yes
      it = find(_bestElements.begin(), _bestElements.end(), i);
      if(it!=_bestElements.end()){
        by.push_back(i);
      }
      else{
        it = find(_worstElements.begin(), _worstElements.end(), i);
        if(it!=_worstElements.end()){
          wy.push_back(i);
        }
        else{
          my.push_back(i);
        }
      }
    }
    else{ //No
      it = find(_worstElements.begin(), _worstElements.end(), i);
      if(it != _worstElements.end()){
        wn.push_back(i);
      }
      else{
        it = find(_bestElements.begin(), _bestElements.end(), i);
        if(it != _bestElements.end()){
          bn.push_back(i);
        }
        else{
          mn.push_back(i);
        }
      }
    }
  }
  Random::shuffle(by);
  Random::shuffle(my);
  Random::shuffle(wy);
  Random::shuffle(bn);
  Random::shuffle(mn);
  Random::shuffle(wn);
  yes = wy;
  yes.insert(yes.end(), my.begin(), my.end());
  yes.insert(yes.end(), by.begin(), by.end());
  no = bn;
  no.insert(no.end(),mn.begin(), mn.end());
  no.insert(no.end(),wn.begin(), wn.end());
}

void AGCEP::sustituirCrom(int bin[], double &peso, vector<int> &wn, vector<int> &mn, vector<int> &bn, vector<int> &by, vector<int> &my, vector<int> &wy){
  bool sust = false;
  if(!bn.empty() && !wy.empty() && !sust){ //Sustituir peor yes por mejor no
    for(int i = 0; i < wy.size() && !sust; ++i){
      for(int j = 0; j < bn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, wy[i], bn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, wy[i],bn[j]);
          wn.push_back(wy[i]);
          by.push_back(bn[j]);
          wy.erase(wy.begin()+i);
          bn.erase(bn.begin()+j);
        }
      }
    }
  }
  if(!bn.empty() && !my.empty() && !sust){ //Sustituir meh yes por mejor no
    for(int i = 0; i < my.size() && !sust; ++i){
      for(int j = 0; j < bn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, my[i], bn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, my[i],bn[j]);
          mn.push_back(my[i]);
          by.push_back(bn[j]);
          my.erase(my.begin()+i);
          bn.erase(bn.begin()+j);
        }
      }
    }
  }
  if(!wy.empty() && !mn.empty() && !sust){ //Sustituir peor yes por meh no
    for(int i = 0; i < wy.size() && !sust; ++i){
      for(int j = 0; j < mn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, wy[i], mn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, wy[i],mn[j]);
          wn.push_back(wy[i]);
          my.push_back(mn[j]);
          wy.erase(wy.begin()+i);
          mn.erase(mn.begin()+j);
        }
      }
    }
  }
  if(!mn.empty() && !my.empty() && !sust){ //Sustituir meh yes por meh no
    for(int i = 0; i < my.size() && !sust; ++i){
      for(int j = 0; j < mn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, my[i], mn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, my[i],mn[j]);
          mn.push_back(my[i]);
          my.push_back(mn[j]);
          my.erase(my.begin()+i);
          mn.erase(mn.begin()+j);
        }
      }
    }
  }
  if(!bn.empty() && !by.empty() && !sust){ //Sustituir mejor yes por mejor no
    for(int i = 0; i < by.size() && !sust; ++i){
      for(int j = 0; j < bn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, by[i], bn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, by[i],bn[j]);
          bn.push_back(by[i]);
          by.push_back(bn[j]);
          by.erase(by.begin()+i);
          bn.erase(bn.begin()+j);
        }
      }
    }
  }
  if(!wn.empty() && !wy.empty() && !sust){ //Sustituir peor yes por peor no
    for(int i = 0; i < my.size() && !sust; ++i){
      for(int j = 0; j < mn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, my[i], mn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, my[i],mn[j]);
          mn.push_back(my[i]);
          my.push_back(mn[j]);
          my.erase(my.begin()+i);
          mn.erase(mn.begin()+j);
        }
      }
    }
  }
  if(!mn.empty() && !by.empty() && !sust){ //Sustituir mejor yes por meh no
    for(int i = 0; i < by.size() && !sust; ++i){
      for(int j = 0; j < mn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, by[i], mn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, by[i],mn[j]);
          bn.push_back(by[i]);
          my.push_back(mn[j]);
          by.erase(by.begin()+i);
          mn.erase(mn.begin()+j);
        }
      }
    }
  }
  if(!wn.empty() && !my.empty() && !sust){ //Sustituir meh yes por peor no
    for(int i = 0; i < my.size() && !sust; ++i){
      for(int j = 0; j < wn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, my[i], wn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, my[i],wn[j]);
          mn.push_back(my[i]);
          wy.push_back(wn[j]);
          my.erase(my.begin()+i);
          wn.erase(wn.begin()+j);
        }
      }
    }
  }
  if(!wn.empty() && !by.empty() && !sust){ //Sustituir mejor yes por peor no
    for(int i = 0; i < by.size() && !sust; ++i){
      for(int j = 0; j < wn.size() && !sust; ++j){
        sust = _ag.checkSustituir(peso, by[i], wn[j]);
        if(sust){
          _ag.sustituirCrom(bin, peso, by[i],wn[j]);
          bn.push_back(by[i]);
          wy.push_back(wn[j]);
          by.erase(by.begin()+i);
          wn.erase(wn.begin()+j);
        }
      }
    }
  }
}

bool AGCEP::anadirFromListaGreedy(int bin[], vector<int> &index, double &peso){
  bool salida = false;
  double max = 0;
  double aux = 0;
  int pos_max = 0;

  for(int i = 0; i < index.size(); ++i){
    if(_ag.checkAdd(index[i], peso, bin)){
      salida = true;
      aux = _ag.valueIfAdded(index[i],bin);
      if(aux > max){
        max = aux;
        pos_max = i;
      }
    }
  }

  if(salida == true){
    _ag.anadirElemento(bin, peso, index[pos_max]);
    index.erase(index.begin()+pos_max);
  }
  return salida;
}

void AGCEP::cruceUniforme(int p1[], int p2[], int h1[], int h2[], bool etapa){
  if(etapa){
    _ag.cruceUniforme(p1,p2,h1,h2);
  }
  else{
    vector<int> indices;
    for(int i = 0; i< getSize(); ++i){
      indices.push_back(i);
    }
    Random::shuffle(indices);

    for(int i = 0; i< getSize(); ++i){
      if(i< getSize()/2){
        h1[indices[i]] = p1[indices[i]];
        h2[indices[i]] = p2[indices[i]];
      }
      else{
        h1[indices[i]] = p2[indices[i]];
        h2[indices[i]] = p1[indices[i]];
      }
    }

    // Hacemos las soluciones factibles
    operadorReparacion(h1);
    operadorReparacion(h2);
  }
}

void AGCEP::operadorReparacion(int hijo[]){
  double pesoHijo = _ag.calcularPeso(hijo);
  bool anadido = true;
  //Tenemos que eliminar elementos
  if(pesoHijo > getCapacidad()){
    operadorReparacionEliminar(hijo, pesoHijo);
  }
  //Tenemos que añadir más elementos
  else{
    //while(anadido){anadido = _ag.anadirElementoGreedy(hijo, pesoHijo);}
    operadorReparacionAnadir(hijo,pesoHijo);
  }
}

void AGCEP::operadorReparacionEliminar(int hijo[], double &pesoHijo){
  bool eliminado = true;
  while(pesoHijo > getCapacidad() && eliminado){
    eliminado = eliminarElemento(hijo,pesoHijo);
  }
  if(pesoHijo > getCapacidad()){
    eliminado = true;
    vector<int> index = getMehElements();
    while(pesoHijo > getCapacidad() && eliminado){
      eliminado = eliminarElemento(hijo, pesoHijo, index);
    }
    eliminado = true;
    while(pesoHijo > getCapacidad() && eliminado){
      eliminado = eliminarElemento(hijo, pesoHijo, _bestElements);
    }
  }
}

void AGCEP::operadorReparacionAnadir(int hijo[], double &pesoHijo){
  bool anadido = true;
  while(anadido){anadido = anadirFromListaGreedy(hijo,_bestElements,pesoHijo);}
  anadido = true;
  vector<int> index = getMehElements();
  while(anadido){
    anadido = anadirFromListaGreedy(hijo, index, pesoHijo);
  }
  anadido = true;
  while(anadido){
    anadido = anadirFromListaGreedy(hijo, _worstElements, pesoHijo);
  }
}

bool AGCEP::eliminarElemento(int bin[], double &peso){
  return eliminarElemento(bin,peso,_worstElements);
}

bool AGCEP::eliminarElemento(int bin[], double &peso, vector<int> index){
  double minRelValor = 0;
  int indexMinRelValor;
  double relValor = 0;
  double valor_aux=0;
  bool salida = false;
  for(int i = 0; i < index.size(); ++i){
    if(bin[index[i]]==1){
      if(relValor==0){
        minRelValor = _ag.calcularRelValor(index[i],bin);
        indexMinRelValor=index[i];
        salida = true;
      }
      else{
        relValor = _ag.calcularRelValor(index[i],bin);
        if(minRelValor > relValor){
          indexMinRelValor = index[i];
          minRelValor = relValor;
        }
      }
    }
  }
  //Eliminamos el peor elemento
  if(salida){
    bin[indexMinRelValor]=0;
    peso = peso - getPeso(indexMinRelValor);
  }
  return salida;
}

/***********FICHEROS DE DATOS***********/
bool AGCEP::saveFichero(const char* fichero) const{
  bool salida = false;
  ofstream ofs;
  ofs.open(fichero);
  if(ofs){
    for(int i = 0; i < _solucionesHistograma.size(); ++i){
      ofs << _valoresHistograma[i] << " : ";
      for(int j = 0 ; j < _solucionesHistograma[i].size()-1; ++j){
        ofs << _solucionesHistograma[i][j] << ",";
      }
      ofs << _solucionesHistograma[i][_solucionesHistograma[i].size()-1] << "\n";
    }
    if(ofs){
      salida = true;
    }
    else cerr << "Error. No se ha podido escribir en el archivo\n\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}

bool AGCEP::saveFichero(int sol[], double valor, const char* fichero) const{
  // https://stackoverflow.com/questions/4186547/c-write-to-an-existing-file
  cout << getSize();
  bool salida = false;
  ofstream ofs;
  ofs.open(fichero, std::ios_base::app);
  if(ofs){
    ofs << valor << " : ";
    for(int i = 0; i < getSize()-1; ++i){
      ofs << sol[i] << ",";
    }
    ofs << sol[getSize()-1] << "\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}

bool AGCEP::clearFichero(const char* fichero) const{
  bool salida=false;
  ofstream ofs;
  //Open the file for writing with the truncate-option, you'll delete the content
  //https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c
  ofs.open(fichero, ofstream::out | ofstream::trunc);
  if(ofs){
    ofs.close();
    if(ofs){
      salida = true;
    }
    else cerr << "Error. El archivo no se ha podido cerrar\n\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}

bool AGCEP::writeElementsPercentages(double perelem, const char* fichero){
  bool salida = false;
  ofstream ofs;
  ofs.open(fichero);
  if(ofs){
    vector<int> indices(getSize());
    vector<double> porcentaje(getSize());
    iota(indices.begin(), indices.end(), 0);
    fill(porcentaje.begin(), porcentaje.end(), 0);
    getBestPercentages(perelem, porcentaje, indices);
    ofs << "Porcentaje de elementos en las mejores soluciones (" << perelem <<"%): \n";
    for(int i = 0; i < indices.size(); ++i){
      ofs << indices[i] << "  :  " <<  porcentaje[i] << "\n";
    }
    if(ofs){
      iota(indices.begin(), indices.end(), 0);
      fill(porcentaje.begin(), porcentaje.end(), 0);
      getWorstPercentages(perelem, porcentaje, indices);
      ofs << "\n\nPorcentaje de elementos de las peores soluciones (" << perelem <<"%):\n";
      for(int i = 0; i <indices.size(); ++i){
        ofs << indices[i] << "  :  " << porcentaje[i] << "\n";
      }
      if(ofs){
        salida = true;
      }
      else cerr << "Error. No se ha podido escribir en el archivo (Worst Solutions)\n\n";
    }
    else cerr << "Error. No se ha podido escribir en el archivo (Best Solutions)\n\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}

bool AGCEP::writeBestWorstElements(double perelem, const char* fichero){
  bool salida = false;
  ofstream ofs;
  ofs.open(fichero);
  if(ofs){
    setBestElements(perelem);
    setWorstElements(perelem);

    ofs << "Elementos de las mejores soluciones (" << perelem << "%): \n";
    if(_bestElements.empty()){
      ofs << "No hay";
    }
    else{
      for(int i = 0; i < _bestElements.size(); ++i){
        ofs << _bestElements[i] << ", ";
      }
    }
    if(ofs){
      ofs << "\n\nElementos de las peores soluciones(" << perelem << "%): \n";
      if(_worstElements.empty()){
        ofs << "No hay";
      }
      else{
        for(int i = 0; i < _worstElements.size(); ++i){
          ofs << _worstElements[i] << ", ";
        }
      }
      if(ofs){
        salida = true;
      }
      else cerr << "Error. No se ha podido escribir en el archivo (Worst Elements)\n\n";
    }
    else cerr << "Error. No se ha podido escribir en el archivo (Best Elements)\n\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}

bool AGCEP::writeAllHistogramsFiles(const char* hist, const char* porc, const char* elem, double perelem){
  bool salida = false;
  if(saveFichero(hist)){
    if(writeElementsPercentages(perelem,porc)){
      if(writeBestWorstElements(perelem, elem)){
        salida = true;
      }
      else cerr << "Error guardando los mejores y peores elementos\n\n";
    }
    else cerr << "Error guardando los porcentajes de uso de elementos\n\n";
  }
  else cerr << "Error guardando las soluciones\n\n";

  return salida;
}
