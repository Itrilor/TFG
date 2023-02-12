#ifndef AGCEP_H
#define AGCEP_H
#include "AG.h"

class AGCEP{
  public:
  
    /**
     * @brief Constructor para reservar memoria
     * @param prob Objeto AG del problema
     */
    AGCEP(AG &ag);
    
    inline int getSize() const{return _ag.getSize();};
    
    /**
     * @brief Se añade, si no está ya incluida, una solución y su respectivo valor para el histograma
     * @param sol Solución a introducir en _solucionesHistograma
     * @param valor Valor a introducir en _valoresHistograma
     */
    void addToHistograma(int sol[], double valor);
    
    void clearHistograma();
    
    /**
     * @brief Ordenar los vectores del histograma dependiendo del valor
     */
    void sortHistograma();
    
    vector<int> getBestElements(double perelem);
    vector<int> getWorstElements(double perelem);
    vector<int> getBestElements(vector<int> best, vector<int> worst);
    vector<int> getWorstElements(vector<int> best, vector<int> worst);
    
    void getBestPercentages(double perelem, vector<double> &perc, vector<int> &indices);
    void getWorstPercentages(double perelem, vector<double> &perc, vector<int> &indices);
    
    /**
     * @brief
     * @param fichero
     * @return @b false si ha habido algún tipo de error, @b true en otro caso
     */
    bool saveFichero(const char* fichero) const;
    
    /**
     * @brief
     * @param sol
     * @param valor
     * @param fichero
     * @return @b false si ha habido algún tipo de error, @b true en otro caso
     */
    bool saveFichero(int sol[], double valor, const char* fichero) const;
    
    /**
     * @brief
     */
    bool clearFichero(const char* fichero) const;
    
    /**
     * @brief Se guarda en un fichero el porcentaje de aparición de elementos para las mejores y peores soluciones encontradas
     * @param perelem
     * @param fichero Fichero que contendrá los datos
     * @return @b false si ha habido algún tipo de error, @b true en otro caso
     */
    bool writeElementsPercentages(double perelem, const char* fichero);

  private:
    AG _ag;
    vector<vector<int>> _solucionesHistograma;
    vector<double> _valoresHistograma; //valores asociados a las soluciones almacenadas
};

#endif
