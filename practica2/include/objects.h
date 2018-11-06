#ifndef OBJECTS_H
#define OBJECTS_H

#include "object3d.h"

class _objects : public _object3D {
 public:
  _objects(){};
  void cube(float Size = 1.0);
  void tetrahedron(float Size = 1.0);
  void PLY();
  void peon();
  void esfera();
  void cono();
  void cilindro();
};

#endif
