#ifndef QKP_H
#define QKP_H

#include <iostream>
#include <vector>

using namespace std;

class QKP{
  public:
    /**
     * @brief Constructor base
     */
    QKP();
		
    /**
     * @brief Constructor para reservar memoria
     * @param nel Número de elementos posibles reservados
     */
    QKP(int nel);
		 
    /**
     * @brief Constructor de copia
     * @param orig El QKP que se quiere copiar
    */
    QKP(const QKP& orig);
		
    /**
     * @brief Destructor
     */
    ~QKP(); 
		
    /**
     * @brief Sobrecarga del operador de asignación
     * @param orig Parámetro rhs de la asignación
     * @return this para concatenación de asignaciones de derecha a izquierda
     */
    QKP& operator=(const QKP& orig);
		
    /**
     * @brief Establece el tamaño del problema
     * @param size El nuevo valor del tamaño del problema
     */
    inline void setSize(int size){_n = size;};
		
    /**
     * @brief Consulta el tamaño de elementos existentes
     * @return El número de elementos con el que trabajamos
     */
    inline int getSize() const{return _n;};
		
    /**
     * @brief Establece el valor de un elemento al ser combinado con otro (si i==j => Devuelve el valor del propio elemento)
     * @param i El id de la primera posición (fila de la matriz)
     * @param j El id de la segunda posición (columna de la matriz)
     * @precond La posición (i,j) es correcta
     * @param valor El valor de el elemento i combinado con el elemento j
     */
    void setValor(int i, int j, double valor);
		
    /**
     * @brief Consulta el valor de un elemento combinado con otro
     * @param i El id de la primera posición (fila de la matriz)
     * @param j El id de la primera posición (fila de la matriz)
     * @return El valor del elemento i combinado con el elemento j
     */
    double getValor(int i, int j) const;
		
    /**
     * @brief Establece el peso de un elemento
     * @param i El id del elemento al que queremos poner el peso
     * @precond La posición i es correcta
     * @param peso El peso del elemento
     */
    void setPeso(int i, double peso);
		
    /**
     * @brief Establece el peso de un elemento
     * @param peso El peso del elemento
     */
    inline void setPesoSolucion(double peso){_pesoSolucion = peso;};
		
    /**
     * @brief Consulta el peso de un elemento
     * @param i El id del elemento
     * @return El peso del elemento i
     */
    double getPeso(int i) const;
		
    /**
     * @brief Establece la capacidad total de la mochila
     * @param cap El valor de la capacidad total
     * @precond cap es un valor real positivo	
     */
    void setCapacidad(double cap);
		
    /**
     * @brief Consulta el valor de la capacidad total de la mochila
     * @return Capacidad total de la mochila
     */		
    inline double getCapacidad() const{return _capacidad;};
		
    /**
     * @brief Establece la semilla
     * @param seed El valor de la semilla
     * @precond seed es un valor entero positivo	
     */
    void setSeed(int seed);
		
    /**
     * @brief Consulta el valor de la semilla
     * @return Semilla para la aleatoriedad
     */		
    inline int getSeed() const{return _seed;};
		
    /**
     * @brief Añade un elemento a la solución sol, actualizando los valores
     * @param pos El id del elemento a añadir a la solución
     * @param sol Vector que representa la solución que estamos considerando
     * @param val Valor total que tiene la solución que estamos considerando
     * @param peso Peso total que tiene la solución que estamos considerando
     */
    void addSolucion(int pos, vector<int> &sol, double &val, double &peso);
		
    //void addSolucion(int pos, int[] sol, double val, double peso);
		
    /**
     * @brief Añade un elemento a la solución actual, actualizando los valores
     * @param pos El id del elemento a añadir a la solución
     */
    void addSolucion(int pos);
		
    /**
     * @brief Consulta la solución actual
     * @return La solución actual
     */
    inline vector<int> getSolucion() const{return _solucion;};
		
    /**
     * @brief Consulta los valores totales de los elementos de la solución actual
     * @return Valores totales de los elementos de la solución actual
     */
    //inline vector<double> getValoresActual() const{return _valoresActual;};
		
    /**
     * @brief Consulta el valor total de la solución actual
     * @return Solución actual
     */
    inline double getValorSolucion() const{return _valorSolucion;};
		
    /**
     * @brief Consulta el peso total de la solución actual
     * @return Peso total de la solución actual
     */
    inline double getPesoSolucion() const{return _pesoSolucion;};		
	
	  
    /**
     * @brief Calcula el peso total de la solución actual y se lo asigna a _peso
     */
    void calcularPeso();
	  
    /**
     * @brief Consulta si se puede añadir un nuevo elemento a una solución
     * @param pos Índice del objeto que se piensa añadir
     * @param peso Peso del objeto que se piensa añadir
     * @param sol Posible solución a la que se le quiere añadir un elemento
     * @return @b false si no se puede, @b true en caso contrario
     */
    bool checkAdd(int pos, double peso, vector<int> sol);
	  
    /**
     * @brief Consulta si se puede añadir un nuevo elemento a la solución actual
     * @param pos Índice del objeto que se piensa añadir
     * @return @b false si no se puede, @b true en caso contrario
     */
    bool checkAdd(int pos);
	
    /**
     * @brief Devuelve el valor de añadir un objeto considerando su peso
     * @param pos Índice del elemento que estamos considerando añadir
     * @param sol Solución que estamos considerando
     * @return El valor de añadir un objeto considerando su peso
     */
    double valueIfAdded(int pos, vector<int> sol);
		
		
    /**
     * @brief Devuelve el valor de añadir un objeto a la solución actual considerando su peso
     * @param pos Índice del elemento que estamos considerando añadir
     * @return El valor de añadir un objeto considerando su peso
     */
    double valueIfAdded(int pos);
    
    void vectorToInt(vector<int> sol, int solbin[]);
    
    double diversity(vector<vector<int>> pob);
			
    /**
     * @brief Recupera la información del problema desde un fichero y lo reconstruye
     * @param fDatos Fichero que contiene los datos
     * @return @b false si ha habido algún tipo de error, @b true en otro caso
     */
    bool leerFicheroDatos(const char* fDatos);
	
	
    //-----------------ALGORITMOS-----------------
    void RandomQKP(vector<int> &sol, double &valor);
    vector<double> RandomQKP(double tEvaluacionMAX, int seed);
    void Greedy(int max_op=0);
    vector<double> AGEU(int numcro, double probm, const double tEvaluacionMAX, int seed);
    vector<double> GACEP(int numcro, double probm, const int EvaluacionMAX, int seed);
    vector<double> CHCGA(int numcro, const int EvaluacionMAX, int seed);
    vector<double> GACEPCHC(int numcro, double probm, const int EvaluacionMAX, int seed);
    vector<double> GACEP3103(int numcro, double probm, const int EvaluacionMAX, int seed);
	
  private:
    int _n;               //Nº elementos
    double **_MatrizCoef; //
    double _capacidad;
    double *_pesos;
    //int _density;
    int _seed;
    vector<int> _solucion;
    //vector<double> _valoresActual;
    double _valorSolucion;
    double _pesoSolucion;
		
    /**
     * @brief Reserva memoria para un conjunto de elementos
     * @param n El número de elementos a reservar
     */
    void reservarMemoria(int n);
		
    /**
     * @brief Libera la memoria
     */
    void liberarMemoria();
		
    /**
     * @brief Copia el objeto que se pasa como argumento
     * @param otro El objeto que se copia
     */
    void copiar(const QKP& otro);
};	

#endif
