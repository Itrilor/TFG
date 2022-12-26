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
		 * @brief Establece la densidad del problema
		 * @param den El valor de la densidad del problema
		 * @precond den es un valor real positivo
		 */
		//inline void setDensidad(double den){_density = den;};
		
		/**
		 * @brief Consulta la densidad del problema
		 * @return Densidad del problema
		 */		
		//inline double getDensidad() const{return _densidad;};
		
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
		void addSolucion(int pos, vector<int> sol, double val, double peso);
		
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
	   *
	   */
	  void calcularPeso();
	  
	  bool checkAdd(int pos, double peso, vector<int> sol);
	  
	  bool checkAdd(int pos, double peso, int sol[]);
	  
	  bool checkAdd(int pos);
	
		/**
		 * @brief Devuelve el valor de añadir un objeto considerando su peso
		 * @param pos Índice del elemento que estamos considerando añadir
		 * @param sol Solución que estamos considerando
		 * @return El valor de añadir un objeto considerando su peso
		 */

		double valueIfAdded(int pos, vector<int> sol);
		
		double valueIfAdded(int pos, int sol[]);	
		
		
		/**
		 * @brief Devuelve el valor de añadir un objeto a la solución actual considerando su peso
		 * @param pos Índice del elemento que estamos considerando añadir
		 * @return El valor de añadir un objeto considerando su peso
		 */
		double valueIfAdded(int pos);
	
	  /**
	   * @brief Recupera la información del problema desde un fichero y lo reconstruye
	   * @param fDatos Fichero que contiene los datos
	   * @return @b false si ha habido algún tipo de error, @b true en otro caso
 	   */
		bool leerFicheroDatos(const char* fDatos);
		
		
		//Funciones para Algoritmo Genético
		void generaSeleccionAleatoria(int sol[], double &val);		
		vector<int> torneoBinario(int numTorneos, double val[], int tam);
		void cruceUniforme(int p1[], int p2[], int h1[], int h2[]);
		void operadorReparacion(int hijo[]);
		double calcularPeso(int sol[]);
		double calcularValor(int sol[]);
		double calcularRelValor(int index, int sol[]);
		void eliminarElemento(int hijo[], double &peso);
		void anadirElemento(int hijo[], double &peso, int pos);
		// Si ha podido añadir elemento -> true, si no -> false
		bool anadirElementoGreedy(int hijo[], double &peso);
		void cambioMutante(int bin[]);
		bool checkSustituir(double peso, int pos1, int pos2);
		void sustituirCrom(int bin[], double &peso, int pos1, int pos2);
		int calcularMejorValor(double valor[], int tam);
		int calcularPeorValor(double valor[], int tam);
		vector<int> calcular2Peores(double valor[], int tam);
		
		vector<int> intToVector(int sol[]);
	
	
		//-----------------ALGORITMOS-----------------
		void RandomQKP(int max=0);
		void Greedy(int max_op=0);
		//int busquedaLocal(const int nEvaluacionesMAX);
		//void AGQKP();
		void AGEU(int numcro, double probm, const double tEvaluacionMAX);
	
	private:
		int _n;								//Nº elementos
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
