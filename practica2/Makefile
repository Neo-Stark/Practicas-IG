# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= main

#
GLUT_DIR=/usr
GL_DIR=/usr/X11R6
SRC=./src
INCLUDE=./include

# banderas para el compilador (C y C++). Se indican los directorios donde se puenden
# encontrar ficheros para incluir mediante la opcion -I
# la opcion -g sirve para permitir la depuraci�n
#
CFLAGS=    -g -DXWINDOWS -DSHM -I/usr/include -I$(GLUT_DIR)/include -I$(INCLUDE)
CXXFLAGS=  -g -DXWINDOWS -DSHM -I/usr/include -I$(GLUT_DIR)/include -I$(INCLUDE)

# banderas pra el enlazador
# Se indica directorios donde encontrar las funciontecas con -L. Usando -l seguido del
# nombre se indica la funcionteca que se desea enlazar.
#
LDFLAGS=  -L$(GL_DIR)/lib64 -lGL -L$(GLUT_DIR)/lib64 -lglut 

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
    EXE = assignment1.exe
        LDFLAGS = -lopengl32 -lglu32 -lglut32
endif
# definicion del compilador
#
CC = g++

# sufijos
.SUFFIXES: .cc .c .o

# Reglas
# $< -> todas las dependencias que han cambiado
# $@ -> el objetivo
# $^ -> todas las dependencias

# como pasar de .cc a .o
%.o: $(SRC)/%.cc $(INCLUDE)/%.h
	$(CC) $(CXXFLAGS) -c $<

%.o: $(SRC)/%.cc
	$(CC) $(CXXFLAGS) -c $<



# como pasar de .o a ejecutable (sin sufijo)

.o:
	$(CC) -o $@ $^ $(LDFLAGS) 

# orden que sera ejecutada por defecto
#
default : $(TARGETS)

# regla de dependencias
#
$(TARGETS) : basic_object3d.o object3d.o axis.o main.o file_ply_stl.o objects.o revolucion.o

#
#
clean:
	rm -f *.o
	rm -f $(TARGETS)

#
#
redo:
	touch $(SRC)/*.cc
	make

#
#
superclean:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)

#
#
tgz:
	rm -f *.o
	rm -f $(TARGETS)
	# rm *.~
	tar -zcvf $(TARGETS).tgz *

