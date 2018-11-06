#include "objects.h"
#include "file_ply_stl.h"
#include "revolucion.h"

void _objects::peon() {
  _file_ply File_ply;
  File_ply.lee_ply(Vertices, Triangles, "data/peon.ply");
  _revolucion Revolucion(Vertices);
  Revolucion.generarPerfil(Vertices, Triangles);
}

void _objects::cube(float Size) {
  Vertices.resize(8);

  Vertices[0] = _vertex3f(-Size / 2, -Size / 2, Size / 2);
  Vertices[1] = _vertex3f(Size / 2, -Size / 2, Size / 2);
  Vertices[2] = _vertex3f(Size / 2, Size / 2, Size / 2);
  Vertices[3] = _vertex3f(-Size / 2, Size / 2, Size / 2);
  Vertices[4] = _vertex3f(-Size / 2, -Size / 2, -Size / 2);
  Vertices[5] = _vertex3f(Size / 2, -Size / 2, -Size / 2);
  Vertices[6] = _vertex3f(Size / 2, Size / 2, -Size / 2);
  Vertices[7] = _vertex3f(-Size / 2, Size / 2, -Size / 2);

  Triangles.resize(12);

  Triangles[0] = _vertex3i(0, 2, 3);
  Triangles[1] = _vertex3i(0, 1, 2);
  Triangles[2] = _vertex3i(1, 6, 2);
  Triangles[3] = _vertex3i(1, 5, 6);
  Triangles[4] = _vertex3i(6, 5, 4);
  Triangles[5] = _vertex3i(7, 6, 4);
  Triangles[7] = _vertex3i(3, 4, 0);
  Triangles[6] = _vertex3i(3, 7, 4);
  Triangles[8] = _vertex3i(2, 6, 3);
  Triangles[9] = _vertex3i(3, 6, 7);
  Triangles[10] = _vertex3i(0, 5, 1);
  Triangles[11] = _vertex3i(0, 4, 5);
}

void _objects::cono() {
  Vertices.push_back(_vertex3f(1.0, 0.0, 0.0));
  _revolucion Revolucion(Vertices);
  Revolucion.generarPerfil(Vertices, Triangles, 0);
  Vertices.push_back(_vertex3f(0.0, 1.0, 0.0));
  Revolucion.generarCaraArriba(Vertices, Triangles, 0);
}

void _objects::esfera() {
  double y, x, r = 1;
  int n = 24;

    Vertices.push_back(_vertex3f(0.0, -1.0, 0.0));
  double angle = (M_PI / n);
  _revolucion Revolucion(Vertices);
  for (unsigned int k = 0; k < n; k++)
    Vertices.push_back(Revolucion.rotateZ(Vertices[k], angle));

  Vertices.erase(Vertices.begin());
  Vertices.pop_back();

  Revolucion.nuevoPerfil(Vertices);
  Revolucion.generarPerfil(Vertices, Triangles, 0, 0);
  Vertices.push_back(_vertex3f(0.0, -1.0, 0.0));
  Revolucion.generarCaraAbajo(Vertices, Triangles, false);
  Vertices.push_back(_vertex3f(0.0, 1.0, 0.0));
  Revolucion.generarCaraArriba(Vertices, Triangles, false);
}

void _objects::PLY() {
  _file_ply File_ply;
  File_ply.lee_ply(Vertices, Triangles, "data/beethoven.ply");
}

void _objects::tetrahedron(float Size) {
  Vertices.resize(4);

  Vertices[0] = _vertex3f(-Size / 2, -Size / 2, -Size / 2);
  Vertices[1] = _vertex3f(0, -Size / 2, Size / 2);
  Vertices[2] = _vertex3f(Size / 2, -Size / 2, -Size / 2);
  Vertices[3] = _vertex3f(0, Size / 2, 0);

  Triangles.resize(4);

  Triangles[0] = _vertex3i(0, 1, 3);
  Triangles[1] = _vertex3i(1, 2, 3);
  Triangles[2] = _vertex3i(2, 0, 3);
  Triangles[3] = _vertex3i(0, 2, 1);
}

void _objects::cilindro() {
  Vertices.push_back(_vertex3f(0.5, 0.0, 0.0));
  Vertices.push_back(_vertex3f(0.5, 1.0, 0.0));
  _revolucion Revolucion(Vertices);
  Revolucion.generarPerfil(Vertices, Triangles);
}
