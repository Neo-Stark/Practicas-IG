#ifndef __TEXTURA__
#define __TEXTURA__

#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
#include "CImg.h"

class _textura {
  public:

   _textura(const char* fname);   // Desmontar constructor --- Primero: id=0

   GLuint id(void);

   ~_textura();                   // Cuando destruyamos una textura poner identificador a 0

  protected:
   GLuint _id;

};

#endif
