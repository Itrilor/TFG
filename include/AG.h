#ifndef AG_H
#define AG_H
#include "QKP.h"

class AG{
  public:
    
    AG();
    
    /**
     * @brief Constructor para reservar memoria
     * @param prob Objeto OKP del problema
     */
    AG(QKP &prob);
    
    /**
     * @brief Sobrecarga del operador de asignación
     * @param orig Parámetro rhs de la asignación
     * @return this para concatenación de asignaciones de derecha a izquierda
     */
    AG& operator=(const AG& orig);
    
    inline int getSize() const{return _qkp.getSize();};
    
    inline int getCapacidad() const{return _qkp.getCapacidad();};
    
    inline double getPeso(int i) const{return _qkp.getPeso(i);};
    
    /****FUNCIONES AUXILIARES*******/
    
    /**
     * @brief Transforma una solución en formato array binario en una en formato vector
     * @param sol Solución en formato array binario
     * @return Solución en formato vector
    */
    vector<int> intToVector(int sol[]);
    
    /**
     * @brief Consulta si se puede añadir un nuevo elemento a una solución
     * @param pos Índice del objeto que se piensa añadir
     * @param peso Peso del objeto que se piensa añadir
     * @param sol Posible solución a la que se le quiere añadir un elemento
     * @return @b false si no se puede, @b true en caso contrario
     */
    bool checkAdd(int pos, double peso, int sol[]);
    
    /**
     * @brief Devuelve el valor de añadir un objeto considerando su peso
     * @param pos Índice del elemento que estamos considerando añadir
     * @param sol Solución que estamos considerando
     * @return El valor de añadir un objeto considerando su peso
     */
    double valueIfAdded(int pos, int sol[]);
    
    /**
     * @brief Comprueba si se puede sustituir un elemento por otro sin sobrepasar el peso máximo
     * @param peso Peso actual
     * @param pos1 Peso del elemento que queremos eliminar
     * @param pos2 Peso del elemento que queremos añadir
     * @return @b false si no se puede, @b true en caso contrario
     */
    bool checkSustituir(double peso, int pos1, int pos2);
    
    /**
     * @brief Calcula el peso total de una posible solución
     * @param sol Solución a considerar
     * @return El peso total de la solución a considerar
     */
    double calcularPeso(int sol[]);
    
    /**
     * @brief Calcula el valor total de una posible solución
     * @param sol Solución a considerar
     * @return El valor total de la solución a considerar
     */
    double calcularValor(int sol[]);
    
    /**
     * @brief Calcula la proporción valor total (individual y combinado) que aporta un elemento a una solución / peso de dicho elemento
     * @param index Índice del elemento a considerar
     * @param sol Solución que estamos considerando
     * @return Proporción valor total que aporta un elemento a una solución / peso del elemento
     */
    double calcularRelValor(int index, int sol[]);
    
    /**
     * @brief De entre una lista de valores, se calcula cuál es el índice con el valor más elevado
     * @param valor Array de valores
     * @param tam Tamaño del array
     * @return Índice con el mayor valor
     */
    int calcularMejorValor(double valor[], int tam);
    
    /**
     * @brief De entre una lista de valores, se calcula cuál es el índice con el valor menos elevado
     * @param valor Array de valores
     * @param tam Tamaño del array
     * @return Índice con el menor valor
     */
		int calcularPeorValor(double valor[], int tam);
		
		/**
		 * @brief De entre una lista de valores, se calcula cuáles son los 2 índices con los valores menos elevados
     * @param valor Array de valores
     * @param tam Tamaño del array
     * @return Vector con los 2 índice con los menores valores
		 */
		vector<int> calcular2Peores(double valor[], int tam);
    
    /*****FUNCIONES PARA EL AG******/
    
    /**
     * @brief Genera una posible solución de forma aleatoria
     * @param sol Solución generada
     * @param val Valor de la solución generada
     */
    void generaSeleccionAleatoria(int sol[], double &val);
    
    /**
     * @brief Tras comparar los valores de ciertos pares de soluciones elegidos aleatoriamente varias veces, elegiremos aquellas soluciones que tengan mayor valor en cada uno de los enfrentamientos
     * @param numTorneos Número de torneos (comparaciones) que realizaremos
     * @param val Valores de los índices (valores de la población que estemos considerando en el momento de usar dicha función)
     * @param tam 
     * @return Índices de las soluciones que hayan ganado sus torneos
     */
     vector<int> torneoBinario(int numTorneos, double val[], int tam);
		
    /**
     * @brief Crea dos nuevas soluciones a partir de otras dos soluciones dadas
     * @param p1 Solución dada 1
     * @param p2 Solución dada 2
     * @param h1 Solución generada 1
     * @param h2 Solución generada 2
     */
    void cruceUniforme(int p1[], int p2[], int h1[], int h2[]);
		
    /**
     * @brief Elimina elementos de la solución si esta sobrepasa la capacidad o añade más elementos (si es posible) si no la sobrepasa
     * @param hijo Solución a considerar
     */
    void operadorReparacion(int hijo[]);
		
    /**
     * @brief Intercambia aleatoriamente un elemento que pertenece a la solución por uno que no (de forma que siga siendo una solución) y añade, si es posible, más elementos
     * @param bin
     */
    void cambioMutante(int bin[]);
		
    /**
     * @brief Sustituye un elemento perteneciente a una solución dada por otro que no esté contenido
     * @param bin Solución que estamos considerando
     * @param peso Peso de la solución que estamos considerando
     * @param pos1 Elemento perteneciente a la solución
     * @param pos2 Elemento no perteneciente a la solución
     */
    void sustituirCrom(int bin[], double &peso, int pos1, int pos2);
		
    /**
     * @brief Elimina el elemento con menor proporción valor_aportado/peso_aportado
     * @param hijo Solución que estamos considerando
     * @param peso Peso total de la solución que estamos considerando
     */
    void eliminarElemento(int hijo[], double &peso);
		
    /**
     * @brief Añade un nuevo elemento a la solución que estemos considerando
     * @param hijo Solución que estamos considerando
     * @param peso Peso total de la solución que estamos considerando
     * @param pos Elemento a añadir
     */
    void anadirElemento(int hijo[], double &peso, int pos);
		
    /**
     * @brief Añade el elemento disponible con mayor proporción valor_aportado/peso_aportado
     * @param hijo Solución que estamos considerando
     * @param peso Peso total de la solución que estamos considerando
     * @return @b false si no se ha podido, @b true en caso contrario
     */
    bool anadirElementoGreedy(int hijo[], double &peso);
		
  private: 
    QKP _qkp;
};

#endif
