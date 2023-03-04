#ifndef CHC_H
#define CHC_H
#include "AG.h"

class CHC{
  public:
    CHC();
  
    /**
     * @brief Constructor para reservar memoria
     * @param ag Objeto AG del problema
     */
    CHC(AG &ag);
    
    /**
     * @brief Consulta el tamaño de elementos existentes
     * @return El número de elementos con el que trabajamos
     */
    inline int getSize() const{return _ag.getSize();};
    
    /**
     * @brief Consulta el valor de la capacidad total de la mochila
     * @return Capacidad total de la mochila
     */
    inline int getCapacidad() const{return _ag.getCapacidad();};
    
    
    inline double getPeso(int i) const{return _ag.getPeso(i);};
    
    //Funciones propias de CHC
    
    int distanciaHamming(int p1, int p2);
    
    /**
     * @brief Comprobación si es posible el cruce entre dos soluciones
     * @param threshold Número minimo de elementos distintos necesario para que se produzca el cruce
     * @param p1 Solución dada 1
     * @param p2 Solución dada 2
     * @return @b true si cumplen las condiciones, @b false en otro caso
     */
    bool seleccion(int threshold, int p1[], int p2[]);
    
    /**
     * @brief Crea dos nuevas soluciones a partir de otras dos soluciones dadas
     * @param p1 Solución dada 1
     * @param p2 Solución dada 2
     * @param h1 Solución generada 1
     * @param h2 Solución generada 2
     */
    void cruceHUX(int p1[], int p2[], int h1[], int h2[]);
    
    vector<int> enfrentamiento(double valorP[], double valorH[], int tam1, int tam2);
    
  private:
    AG _ag;
};

#endif
