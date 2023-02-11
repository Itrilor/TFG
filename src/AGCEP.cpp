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
  vector<int> indices(_solucionesHistograma[0].size());
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
  vector<int> indices(_solucionesHistograma[0].size());
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
    for(int j = 0; j < _solucionesHistograma[0].size(); ++j){
      if(_solucionesHistograma[i][j]==1){
        perc[j]++;
      }
    }
  }
  for(int i = 0; i < indices.size(); ++i){
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
    percentages.push_back(perc[indices[i]]);
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
    for(int j = 0; j < _solucionesHistograma[0].size(); ++j){
      if(_solucionesHistograma[i][j]==1){
        perc[j]++;
      }
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
    percentages.push_back(perc[indices[i]]);
  }
  // Sustituimos el orden original por el que hemos obtenido
  perc.clear();
  indices.clear();
  perc = percentages;
  indices = index;
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
    vector<int> indices(_solucionesHistograma[0].size());
    vector<double> porcentaje(_solucionesHistograma[0].size());
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
      else cerr << "Error. No se ha podido escribir en el archivo(Worst Solutions)\n\n";
    }
    else cerr << "Error. No se ha podido escribir en el archivo(Best Solutions)\n\n";
  }
  else cerr << "Error. El archivo no se ha podido abrir\n\n";

  return salida;
}
