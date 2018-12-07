# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= practica4

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
$(TARGETS) : axis.o cylinder.o level3.o object_ply.o basic_object3d.o\
             file_ply_stl.o  sphere.o \
             chess_board.o  object3d.o \
	     tetrahedron.o cone.o level2.o \
             object3d_sweep_basic.o textura.o practica4.o

practica4.o: practica4.cc

basic_object3d.o: basic_object3d.cc basic_object3d.h

object3d.o: object3d.cc object3d.h

axis.o: basic_object3d.o axis.cc axis.h

tetrahedron.o: tetrahedron.cc tetrahedron.h

cylinder.o: cylinder.cc cylinder.h

level3.o: level3.cc level3.h

file_ply_stl.o: file_ply_stl.cc file_ply_stl.h

sphere.o: sphere.cc sphere.h

chess_board.o: chess_board.cc chess_board.h

cone.o: cone.cc cone.h

level2.o: level2.cc level2.h

textura.o: textura.cc textura.h



#
#
clean:
	$(RM) *.o
#	$(RM) $(TARGETS)

#
#
redo:
	touch *.cc
	make

#
#
superclean:
	$(RM) *.o
	$(RM) *~
	$(RM) $(TARGETS)

#
#
tgz:
	$(RM) *.o
	$(RM) *~
	$(RM) $(TARGETS)
	tar -zcvf $(TARGETS).tgz *

