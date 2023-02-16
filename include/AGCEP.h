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
    
    /**
     * @brief Establece los mejores elementos a utilizar
     * @param perelem Porcentaje de mejores y peores elementos a considerar
     */
    void setBestElements(double perelem);
    
    /**
     * @brief Consulta el vector de los mejores elementos a utilizar
     * @return El vector de los mejores elementos a utilizar
     */
    inline vector<int> getBestElements(){return _bestElements;};
    
    /**
     * @brief Establece los peores elementos a utilizar
     * @param perelem Porcentaje de mejores y peores elementos a considerar
     */
    void setWorstElements(double perelem);
    
    /**
     * @brief Consulta el vector de los peores elementos a utilizar
     * @return El vector de los peores elementos a utilizar
     */
    inline vector<int> getWorstElements(){return _worstElements;};
    
    /**
     * @brief Calcula los elementos que se han usado en las mejores soluciones
     * @param perelem Porcentaje de las mejores soluciones a considerar
     * @return Elementos de las mejores soluciones
     */
    vector<int> getBestElements(double perelem);
    
    /**
     * @brief Calcula los elementos que se han usado en las peores soluciones
     * @param perelem Porcentaje de las peores soluciones a considerar
     * @return Elementos de las peores soluciones
     */
    vector<int> getWorstElements(double perelem);
    
    /**
     * @brief Calcula los elementos que se han usado solo en las mejores soluciones
     * @param best Vector con los elementos de las mejores soluciones
     * @param worst Vector con los elementos de las peores soluciones
     * @return Elementos que se encuentran solo en las mejores soluciones
     */
    vector<int> getBestElements(vector<int> best, vector<int> worst);
    
    /**
     * @brief Calcula los elementos que se han usado solo en las peores soluciones
     * @param best Vector con los elementos de las mejores soluciones
     * @param worst Vector con los elementos de las peores soluciones
     * @return Elementos que se encuentran solo en las peores soluciones
     */
    vector<int> getWorstElements(vector<int> best, vector<int> worst);
    
    void getBestPercentages(double perelem, vector<double> &perc, vector<int> &indices);
    void getWorstPercentages(double perelem, vector<double> &perc, vector<int> &indices);
    
    //Modificaciones del AG
    void cambioMutante(int bin[], bool etapa);
    
    void getYNfromList(int bin[], vector<int> &yes, vector<int> &no, vector<int> &wn, vector<int> &mn, vector<int> &bn);
    
    bool anadirFromListaGreedy(int bin[], vector<int> &index, double &peso);
    
    
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
    
    bool writeBestWorstElements(double perelem, const char* fichero);
    
    bool writeAllHistogramsFiles(const char* hist, const char* porc, const char* elem, double perelem);

  private:
    AG _ag;
    vector<vector<int>> _solucionesHistograma;
    vector<double> _valoresHistograma; //valores asociados a las soluciones almacenadas
    vector<int> _bestElements;
    vector<int> _worstElements;
};

#endif
