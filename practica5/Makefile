# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= practica5

#
GLUT_DIR=/usr
GL_DIR=/usr/X11R6

# banderas para el compilador (C y C++). Se indican los directorios donde se puenden
# encontrar ficheros para incluir mediante la opcion -I
# la opcion -g sirve para permitir la depuraci�n
#
CFLAGS=    -g -DXWINDOWS -DSHM -I/usr/include -I$(GLUT_DIR)/include -I. -I./CImg-2.1.5 --std=c++11
CXXFLAGS=  -g -DXWINDOWS -DSHM -I/usr/include -I$(GLUT_DIR)/include -I. -I./CImg-2.1.5 --std=c++11

# banderas pra el enlazador
# Se indica directorios donde encontrar las funciontecas con -L. Usando -l seguido del
# nombre se indica la funcionteca que se desea enlazar.
LDFLAGS= -L/usr/X11R6/lib/ -lGL -lGLU -lglut -lm -lX11 -lpthread

# definicion del compilador
#
CC = g++

# sufijos
.SUFFIXES: .cc .c .o

# Reglas
# $< -> todas las dependencias que han cambiado
# $@ -> el objetivo
# $^ -> todas las dependencias


# como pasar de .o a ejecutable (sin sufijo)

.o:
	$(CC) -o $@ $^ $(LDFLAGS) 

# orden que sera ejecutada por defecto
#
default : $(TARGETS)

# regla de dependencias
#

$(TARGETS) : file_ply_stl.o objetos.o textura.o practica5.o
	$(CC) -o $@ $(CXXFLAGS) file_ply_stl.o objetos.o practica5.o textura.o $(LDFLAGS)


#
#
clean:
	rm -f *.o
	rm -f $(TARGETS)

#
#
redo:
	touch *.cc
	make

#
#
superclean:
	rm -f *.o
	rm -f *~
	rm -f $(TARGETS)

#
#
tgz:
	rm -f *.o
	rm -f *~
	rm -f $(TARGETS)
	tar -zcvf $(TARGETS).tgz *

