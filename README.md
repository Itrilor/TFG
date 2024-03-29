# TFG
*Estructura de carpetas*

  - bin/ -> carpeta que contiene el ejecutable
	
  - data/ -> carpeta donde se encuentran los ficheros de datos que utilizaremos
  
  - Histogramas/ -> carpeta donde se encuentran los ficheros relacionados con los histogramas
	
  - include/ -> carpeta que contiene los archivos de cabecera (QKP.h y random.hpp)

  - Notas/ -> carpeta que contiene archivos con notas sobre las fases de implementación
	
  - src/ -> carpeta que contiene los archivos fuente (QKP.cpp y main.cpp)
	
  - obj/ -> carpeta que contiene los archivos objetos
  
  - Subresultados/ -> carpeta que contiene distintos archivos sobre los resultados obtenidos en las distintas ejecuciones
	
*Contenido de los distintos archivos*

  - Makefile -> Implementado con el fin de poder compilar el programa y generar el ejecutable
  
  - script.sh -> Implementado con el fin de guardar los resultados de todas las ejecuciones en un archivo .txt

  - scriptMilestone.sh -> Implementado con el fin de guardar los resultados de cada porcentaje de ejecución de todos los archivos (se tuvo que modificar el QKP.cpp para que devolviese dichos valores) 
	
  - include/random.hpp -> Archivo aportado por el profesor Daniel Molina Cabrera durante la asignatura Metaheurísticas para poder generar números aleatorios
	
  - include/QKP.h -> archivo de cabecera que define todas las funciones propias de la clase que se utilizará en este trabajo
  
  - include/AG.h -> archivo de cabecera que define las funciones propias del algoritmo genético
	
  - src/QKP.cpp -> archivo fuente que incluye la implementación de todas las funciones de la clase y que se utilizarán para resolver el problema

  - src/AG.cpp -> archivo fuente que incluye la implementación de las funciones propias del algoritmo genético
	
  - src/main.cpp
	
  - bin/QKP -> ejecutable

*Resultados obtenidos*
  - resultados.txt -> Archivo obtenido de la ejecución inicial de script.sh. Contiene los resultados (valor y tiempo) de las ejecuciones de todos los datos con los distintos algoritmos implementados. Utilizando como parámetro de parada el tiempo.
  - resultados.csv -> Hoja de cálculo que almacena el contenido del archivo anterior.
  - resultados10Ejecuciones.txt -> Contiene los resultados (valor) de ejecutar 10 veces el AGEU y hacerles la media.
  - resultadosAGEU450.txt -> Contiene los resultados (valor) de Random y AGEU (10 veces) con 450 iteraciones como criterio de parada.
  - diferenciaAGEU.ods -> Hoja de cálculo que contiene las ejecuciones del AGEU cuando el parámetro de parada es el tiempo y cuando es el número de ejecuciones, para demostrar que son equivalentes. Luego se ha añadido los resultados para cuando se le pasa un menor número de iteraciones y también se muestra la diferencia.
  - resultadosAGEUv.txt -> Contiene los resultados (valor) de las ejecuciones del AGEU, utilizando como criterio de parada el número de ejecuciones.
  - resultadosHistogramaMutacion.txt -> Contiene los resultados (valor) de las ejecuciones del GACEP utilizando los elementos encontrados en las mejores y peores soluciones solo en la mutación.
  - resultadosHistogramaMutacion.xlsx -> Hoja de cálculo para comparar los resultados del AGEU y el GACEP (por ahora solo modificado la mutación) en formato Tacolab.
  - resultadosMilestones.txt -> Contiene los resultados (valor) de las distintas fases (porcentajes de completitud) en las que se encuentra el AGEU en el momento.
  - resultadosMilestones.xlsx -> Hoja de cálculo que contiene los valores almacenados en resultadosMilestones.txt, junto con algunas gráficas mostrando los cambios producidos, las diferencias proemdios y totales.
  - ST1.xlsx -> Resultados de los distintos algoritmos almacenados de la forma necesaria para pasarlo a tacolab.

*Compilación*

Ejecutar 'make' en el directorio donde se encuentra este archivo
	
*Ejecución*

Ejecutar lo siguiente en el directorio 'bin/', es decir, donde está el ejecutable

	./QKP [nombre_datos]
	
 Donde [nombre_datos] es el nombre del archivo de los datos
