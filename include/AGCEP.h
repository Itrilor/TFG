#ifndef AGCEP_H
#define AGCEP_H
#include "AG.h"
#include "CHC.h"

class AGCEP{
  public:
    
    AGCEP();
  
    /**
     * @brief Constructor para reservar memoria
     * @param ag Objeto AG del problema
     */
    AGCEP(AG &ag);
    
    /**
     * @brief Constructor para reservar memoria
     * @param ag Objeto AG del problema
     * @param chc Objeto CHC del problema
     */
    AGCEP(AG &ag, CHC &chc);
    
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
    
    /**
     * @brief Consulta el peso de un elemento
     * @param i El id del elemento 
     * @return El peso del elemento i
     */
    inline double getPeso(int i) const{return _ag.getPeso(i);};
    
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
    
    vector<int> getMehElements();
    
    //Modificaciones del AG
    void cambioMutante(int bin[], bool etapa);
    
    void getYNfromList(int bin[], vector<int> &yes, vector<int> &no, vector<int> &wn, vector<int> &mn, vector<int> &bn, vector<int> &by, vector<int> &my, vector<int> &wy);
    
    void sustituirCrom(int bin[], double &peso, vector<int> &wn, vector<int> &mn, vector<int> &bn, vector<int> &by, vector<int> &my, vector<int> &wy);
    
    bool anadirFromListaGreedy(int bin[], vector<int> &index, double &peso);
    
    void cruceUniforme(int p1[], int p2[], int h1[], int h2[], bool etapa);
    
    void cruceHUX(int p1[], int p2[], int h1[], int h2[], bool etapa);
    
    void crucePorcentual(int p1[], double v1, int p2[], double v2, int h1[], int h2[], bool etapa, double p);
    
    void operadorReparacion(int hijo[]);
    void operadorReparacionEliminar(int hijo[], double &pesoHijo);
    void operadorReparacionAnadir(int hijo[], double &pesoHijo);
    
    bool eliminarElemento(int bin[], double &peso);
    bool eliminarElemento(int bin[], double &peso, vector<int> index);
    
    
    //Ficheros
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
    CHC _chc;
    vector<vector<int>> _solucionesHistograma;
    vector<double> _valoresHistograma; //valores asociados a las soluciones almacenadas
    vector<int> _bestElements;
    vector<int> _worstElements;
};

#endif
