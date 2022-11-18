# TFG
*Estructura de carpetas*

  - bin/ -> carpeta que contiene el ejecutable
	
  - data/ -> carpeta donde se encuentran los ficheros de datos que utilizaremos
	
  - include/ -> carpeta que contiene los archivos de cabecera (QKP.h y random.hpp)
	
  - src/ -> carpeta que contiene los archivos fuente (QKP.cpp y main.cpp)
	
  - obj/ -> carpeta que contiene los archivos objetos
	
*Contenido de los distintos archivos*

  - Makefile -> Implementado con el fin de poder compilar el programa y generar el ejecutable
	
  - include/random.hpp -> Archivo aportado por el profesor Daniel Molina Cabrera durante la asignatura Metaheurísticas para poder generar números aleatorios
	
  - include/QKP.h -> archivo de cabecera que define todas las funciones propias de la clase que se utilizará en este trabajo
	
  - include/QKP.cpp -> archivo fuente que incluye la implementación de todas las funciones de la clase y que se utilizarán para resolver el problema
	
  - src/main.cpp
	
  - bin/QKP -> ejecutable
	
*Compilación*

Ejecutar 'make' en el directorio donde se encuentra este archivo
	
*Ejecución*

Ejecutar lo siguiente en el directorio 'bin/', es decir, donde está el ejecutable

	./QKP [seed] [nombre_datos]
	
 Donde [nombre_datos] es el nombre del archivo de los datos
