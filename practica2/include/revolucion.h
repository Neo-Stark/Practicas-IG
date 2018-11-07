#ifndef REVOLUCION_H
#define REVOLUCION_H

#include "object3d.h"

class _revolucion {
 public:
  _revolucion(vector<_vertex3f> perfil, int steps = 24)
      : steps(steps), Perfil(perfil){};
  _vertex3f rotateX(_vertex3f p, float alpha);
  _vertex3f rotateY(_vertex3f p, float alpha);
  _vertex3f rotateZ(_vertex3f p, float alpha);
  void nuevoPerfil(const vector<_vertex3f>& vertices);
  void generarPerfil(vector<_vertex3f>& Vertices, vector<_vertex3i>& Triangles,
                     bool tapaArriba = 1, bool tapaAbajo = 1);
  void generarCaraAbajo(vector<_vertex3f>& Vertices,
                        vector<_vertex3i>& Triangles, bool generarPunto = 1);
  void generarCaraArriba(vector<_vertex3f>& Vertices,
                         vector<_vertex3i>& Triangles, bool generarPunto = 1);
  void modificaSteps(int pasos) { steps = pasos; };

 private:
  int steps;
  vector<_vertex3f> Perfil;
};

#endif