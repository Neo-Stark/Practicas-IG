#include "revolucion.h"
#include "iostream"

_vertex3f _revolucion::rotateX(_vertex3f p, float alpha) {
  _vertex3f point;

  point.x = p.x;
  point.y = cos(alpha) * p.y - sin(alpha) * p.z;
  point.z = sin(alpha) * p.y + cos(alpha) * p.z;

  return point;
}

_vertex3f _revolucion::rotateY(_vertex3f p, float alpha) {
  _vertex3f point;

  point.x = cos(alpha) * p.x + sin(alpha) * p.z;
  point.y = p.y;
  point.z = -sin(alpha) * p.x + cos(alpha) * p.z;

  return point;
}

_vertex3f _revolucion::rotateZ(_vertex3f p, float alpha) {
  _vertex3f point;

  point.x = cos(alpha) * p.x - sin(alpha) * p.y;
  point.y = sin(alpha) * p.x + cos(alpha) * p.y;
  point.z = p.z;

  return point;
}

void _revolucion::nuevoPerfil(const vector<_vertex3f>& vertices) {
  Perfil.erase(Perfil.begin(), Perfil.end());
  Perfil = vertices;
}

void _revolucion::generarPerfil(vector<_vertex3f>& Vertices,
                                vector<_vertex3i>& Triangles, bool tapaArriba,
                                bool tapaAbajo) {
  double angle = (2.0 * M_PI) / steps;

  for (int step = 0; step < steps; step++) {
    vector<_vertex3f> siguientePerfil(Perfil.size());
    for (unsigned int k = 0; k < Perfil.size(); k++)
      siguientePerfil[k] = rotateY(Perfil[k], angle);

    Vertices.insert(Vertices.end(), siguientePerfil.begin(),
                    siguientePerfil.end());

    // Formamos las caras
    unsigned int inicioPerfil, finPerfil;
    inicioPerfil = step * Perfil.size();
    finPerfil = inicioPerfil + Perfil.size();

    for (unsigned int i = inicioPerfil + 1, k = finPerfil + 1; i < finPerfil;
         i++, k++) {
      Triangles.push_back(_vertex3i(i - 1, k - 1, k));
      Triangles.push_back(_vertex3i(i - 1, k, i));
    }

    Perfil = siguientePerfil;
  }

  if (tapaAbajo) generarCaraAbajo(Vertices, Triangles);
  if (tapaArriba) generarCaraArriba(Vertices, Triangles);
}

void _revolucion::generarCaraAbajo(vector<_vertex3f>& Vertices,
                                   vector<_vertex3i>& Triangles,
                                   bool generarPunto) {
  if (Vertices.front().x and generarPunto) {
    _vertex3f puntoCentral(0.0, Vertices.front().y, 0.0);
    Vertices.push_back(puntoCentral);
  }

  for (int step = 0; step < steps; step++) {
    int perfilactual = step * Perfil.size();
    int siguientePerfil = perfilactual + Perfil.size();
    Triangles.push_back(
        _vertex3i(Vertices.size() - 1, siguientePerfil, perfilactual));
  }
}

void _revolucion::generarCaraArriba(vector<_vertex3f>& Vertices,
                                    vector<_vertex3i>& Triangles,
                                    bool generarPunto) {
  if (Vertices[Perfil.size() - 1].x and generarPunto) {
    _vertex3f puntoCentral(0.0, Vertices[Perfil.size() - 1].y, 0.0);
    Vertices.push_back(puntoCentral);
  }

  for (int step = 0; step < steps; step++) {
    int perfilactual = (step + 1) * Perfil.size() - 1;
    int siguientePerfil = perfilactual + Perfil.size();
    Triangles.push_back(
        _vertex3i(Vertices.size() - 1, perfilactual, siguientePerfil));
  }
}