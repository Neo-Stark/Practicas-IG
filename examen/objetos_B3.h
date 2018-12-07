#include <GL/gl.h>
#include <stdlib.h>
#include <vector>
#include "file_ply_stl.h"
#include "vertex.h"

using namespace std;

const float AXIS_SIZE = 5000;
typedef enum { POINTS, EDGES, SOLID_CHESS, SOLID } _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D {
 public:
  _puntos3D();
  void draw_puntos(float r, float g, float b, int grosor);

  vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D : public _puntos3D {
 public:
  _triangulos3D();
  void draw_aristas(float r, float g, float b, int grosor);
  void draw_solido(float r, float g, float b);
  void draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2,
                           float b2);
  virtual void draw(_modo modo, float r1, float g1, float b1, float r2,
                    float g2, float b2, float grosor);

  virtual void process_key(unsigned char Tecla){};

  vector<_vertex3i> caras;
};

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D {
 public:
  _cubo(float tam = 1);
};

//*************************************************************************
// clase casa
//*************************************************************************

class _casa : public _triangulos3D {
 public:
  _casa(float tam = 1);
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D {
 public:
  _piramide(float tam = 1, float al = 0.75);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _revolucion : public _triangulos3D {
 public:
  _revolucion(int steps = 24) : steps(steps){};
  _vertex3f rotateX(_vertex3f p, float alpha);
  _vertex3f rotateY(_vertex3f p, float alpha);
  _vertex3f rotateZ(_vertex3f p, float alpha);
  void nuevoPerfil(const vector<_vertex3f>& vertices);
  void generarPerfil(bool tapaArriba = 1, bool tapaAbajo = 1, char eje = 'Y');
  void generarCaraAbajo(bool generarPunto = 1);
  void generarCaraArriba(bool generarPunto = 1);
  void modificaSteps(int pasos) { steps = pasos; };
  void esfera();
  void limpiar();

 private:
  int steps;
  vector<_vertex3f> Perfil;
};

//*************************************************************************
// clase cilindro
//*************************************************************************

class _cilindro : public _revolucion {
 public:
  _cilindro();
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply : public _triangulos3D {
 public:
  _objeto_ply();

  int parametros(char* archivo);
};

//************************************************************************
// objeto articulado: tanque
//************************************************************************

class _chasis : public _triangulos3D {
 public:
  _chasis();
  void draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
            float b2, float grosor);

  float altura;

 protected:
  _revolucion rodamiento;
  _cubo base;
};

//************************************************************************

class _torreta : public _triangulos3D {
 public:
  _torreta();
  void draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
            float b2, float grosor);

  float altura;
  float anchura;

 protected:
  _cubo base;
  _piramide parte_trasera;
};

//************************************************************************

class _tubo : public _triangulos3D {
 public:
  _tubo();
  void draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
            float b2, float grosor);

 protected:
  _revolucion tubo_abierto;  // caña del cañón
};

//************************************************************************

class _tanque : public _triangulos3D {
 public:
  _tanque();
  void draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
            float b2, float grosor);

  float giro_tubo;
  float giro_torreta;

  float giro_tubo_min;
  float giro_tubo_max;

 protected:
  _chasis chasis;
  _torreta torreta;
  _tubo tubo;
};

class _coche : public _triangulos3D {
 public:
  _coche();
  void draw(_modo);
  void draw_ruedas(_modo);
  void draw_cabina(_modo);
  void draw_antena(_modo);
  void draw_chasis(_modo);

  float desplazamiento;
  float suspension;
  float giro_antena;
  float tope_suspension;
  float tope_suspension_min;
  int tope_desplazamiento;
  int tope_desplazamiento_min;

 protected:
  _cilindro rueda;
  _revolucion cabina;
  _revolucion antena;
  _cubo chasis;
  _piramide frontal;
  _cilindro tubo;
};


class _jerarquico : public _triangulos3D {
  public:
    _jerarquico(){};
    void draw(_modo modo);
    void draw_cubos(_modo modo);
    void draw_cilindros(_modo modo);

    _cilindro tubo;
    _cilindro antena;
    _cubo cubo1;
    _cubo cubo2;
    int rotacion_cubo;
    int rotacion_antena;
};
