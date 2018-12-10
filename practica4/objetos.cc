#include "objetos.h"

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D() {}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor) {
  //**** usando vertex_array ****
  glPointSize(grosor);
  glColor3f(r, g, b);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glDrawArrays(GL_POINTS, 0, vertices.size());

  /*int i;
  glPointSize(grosor);
  glColor3f(r,g,b);
  glBegin(GL_POINTS);
  for (i=0;i<vertices.size();i++){
          glVertex3fv((GLfloat *) &vertices[i]);
          }
  glEnd();*/
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D() {}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor) {
  //**** usando vertex_array ****
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(grosor);
  glColor3f(r, g, b);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, &caras[0]);

  /*int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(grosor);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++){
          glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
          glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
          glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
          }
  glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b) {
  int i;

  glPolygonMode(GL_FRONT, GL_FILL);
  glColor3f(r, g, b);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    glVertex3fv((GLfloat*)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2,
                                        float g2, float b2) {
  int i;
  glPolygonMode(GL_FRONT, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (i % 2 == 0)
      glColor3f(r1, g1, b1);
    else
      glColor3f(r2, g2, b2);
    glVertex3fv((GLfloat*)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat*)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2,
                         float g2, float b2, float grosor) {
  switch (modo) {
    case POINTS:
      draw_puntos(r1, g1, b1, grosor);
      break;
    case EDGES:
      draw_aristas(r1, g1, b1, grosor);
      break;
    case SOLID_CHESS:
      draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);
      break;
    case SOLID:
      draw_solido(r1, g1, b1);
      drawNormales();
      break;
  }
}

void _triangulos3D::generarNormalesCaras() {
  for (int i = 0; i < caras.size(); i++) {
    _vertex3f A, B, C;
    A = vertices[caras[i]._0];
    B = vertices[caras[i]._1];
    C = vertices[caras[i]._2];

    _vertex3f ab, bc, normal;

    ab.x = B.x - A.x;
    ab.y = B.y - A.y;
    ab.z = B.z - A.z;

    bc.x = C.x - B.x;
    bc.y = C.y - B.y;
    bc.z = C.z - B.z;

    normal = ab.cross_product(bc);
    normal = normal.normalize();

    normales_caras.push_back(normal);
  }
}

void _triangulos3D::generarNormalesVertices() {
  // Recorremos los vertices
  for (int i = 0; i < vertices.size(); i++) {
    _vertex3f normal(0, 0, 0);

    // Recorremos las caras
    for (int j = 0; j < caras.size(); j++) {
      // Comprobamos si el vertice esta en otra cara (por la posicion)
      if (caras[j]._0 == i || caras[j]._1 == i || caras[j]._2 == i) {
        normal = _vertex3f(normal.x + normales_caras[j].x,
                           normal.y + normales_caras[j].y,
                           normal.z + normales_caras[j].z);
      }
    }

    normal = normal.normalize();
    normales_vertices.push_back(normal);
  }
}

void _triangulos3D::generarNormales(){
  generarNormalesCaras();
  generarNormalesVertices();
}

void _triangulos3D::drawNormales() {
  if (normales_caras.size() != 0) {
    glPointSize(2);
    glColor3f(0, 0, 1);

    glBegin(GL_LINES);

    for (int i = 0; i < vertices.size(); i++) {
      _vertex3f _1 = vertices[i];
      _vertex3f _2;

      _2.x = _1.x + normales_vertices[i].x;
      _2.y = _1.y + normales_vertices[i].y;
      _2.z = _1.z + normales_vertices[i].z;

      glVertex3f(_1.x, _1.y, _1.z);
      glVertex3f(_2.x, _2.y, _2.z);
    }

    glEnd();
  } else
    cout << "Normales no calculadas" << endl;
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam) {
  // vertices
  vertices.resize(8);
  vertices[0].x = -tam;
  vertices[0].y = -tam;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = -tam;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = tam;
  vertices[2].z = tam;
  vertices[3].x = -tam;
  vertices[3].y = tam;
  vertices[3].z = tam;
  vertices[4].x = -tam;
  vertices[4].y = -tam;
  vertices[4].z = -tam;
  vertices[5].x = tam;
  vertices[5].y = -tam;
  vertices[5].z = -tam;
  vertices[6].x = tam;
  vertices[6].y = tam;
  vertices[6].z = -tam;
  vertices[7].x = -tam;
  vertices[7].y = tam;
  vertices[7].z = -tam;

  // triangulos
  caras.resize(12);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 3;
  caras[1]._0 = 3;
  caras[1]._1 = 1;
  caras[1]._2 = 2;
  caras[2]._0 = 1;
  caras[2]._1 = 5;
  caras[2]._2 = 2;
  caras[3]._0 = 5;
  caras[3]._1 = 6;
  caras[3]._2 = 2;
  caras[4]._0 = 5;
  caras[4]._1 = 4;
  caras[4]._2 = 6;
  caras[5]._0 = 4;
  caras[5]._1 = 7;
  caras[5]._2 = 6;
  caras[6]._0 = 0;
  caras[6]._1 = 7;
  caras[6]._2 = 4;
  caras[7]._0 = 0;
  caras[7]._1 = 3;
  caras[7]._2 = 7;
  caras[8]._0 = 3;
  caras[8]._1 = 2;
  caras[8]._2 = 7;
  caras[9]._0 = 2;
  caras[9]._1 = 6;
  caras[9]._2 = 7;
  caras[10]._0 = 0;
  caras[10]._1 = 1;
  caras[10]._2 = 4;
  caras[11]._0 = 1;
  caras[11]._1 = 5;
  caras[11]._2 = 4;

  generarNormales();
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al) {
  // vertices
  vertices.resize(5);
  vertices[0].x = -tam;
  vertices[0].y = 0;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = 0;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = 0;
  vertices[2].z = -tam;
  vertices[3].x = -tam;
  vertices[3].y = 0;
  vertices[3].z = -tam;
  vertices[4].x = 0;
  vertices[4].y = al;
  vertices[4].z = 0;

  caras.resize(6);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 4;
  caras[1]._0 = 1;
  caras[1]._1 = 2;
  caras[1]._2 = 4;
  caras[2]._0 = 2;
  caras[2]._1 = 3;
  caras[2]._2 = 4;
  caras[3]._0 = 3;
  caras[3]._1 = 0;
  caras[3]._2 = 4;
  caras[4]._0 = 3;
  caras[4]._1 = 1;
  caras[4]._2 = 0;
  caras[5]._0 = 3;
  caras[5]._1 = 2;
  caras[5]._2 = 1;

  generarNormales();
}

//*************************************************************************
// clase cilindro
//*************************************************************************

_cilindro::_cilindro() {
  // perfil para un cilindro
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = 1.0;
  aux.y = -0.35;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 1.0;
  aux.y = 0.35;
  aux.z = 0.0;
  perfil.push_back(aux);
  nuevoPerfil(perfil);
  generarPerfil();
  generarNormales();
}
//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply() {
  // leer lista de coordenadas de vértices y lista de indices de vértices
}

int _objeto_ply::parametros(char* archivo) {

  lector.lee_ply(vertices, caras, archivo);
  generarNormales();

  return (0);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

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

void _revolucion::generarPerfil(bool tapaArriba, bool tapaAbajo, char eje) {
  int i, j;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int num_aux;
  eje = toupper(eje);

  num_aux = Perfil.size();
  double angle = (2.0 * M_PI) / steps;

  for (int step = 0; step < steps; step++) {
    vector<_vertex3f> siguientePerfil(Perfil.size());
    for (unsigned int k = 0; k < Perfil.size(); k++)
      if (eje == 'Y')
        siguientePerfil[k] = rotateY(Perfil[k], angle);
      else if (eje == 'X')
        siguientePerfil[k] = rotateX(Perfil[k], angle);
      else if (eje == 'Z')
        siguientePerfil[k] = rotateZ(Perfil[k], angle);
      else {
        std::cerr << "Eje incorrecto." << std::endl;
        exit(1);
      }

    vertices.insert(vertices.end(), siguientePerfil.begin(),
                    siguientePerfil.end());
    // Formamos las caras
    //   unsigned int inicioPerfil, finPerfil;
    //   inicioPerfil = step * Perfil.size();
    //   finPerfil = inicioPerfil + Perfil.size();

    //   for (unsigned int i = inicioPerfil + 1, k = finPerfil + 1; i <
    //   finPerfil;
    //        i++, k++) {
    //     caras.push_back(_vertex3i(i - 1, k - 1, k));
    //     caras.push_back(_vertex3i(i - 1, k, i));
    //   }

    Perfil = siguientePerfil;
    // }
    // tratamiento de las caras
  }
  for (j = 0; j < steps; j++) {
    for (i = 0; i < num_aux - 1; i++) {
      cara_aux._0 = i + ((j + 1) % steps) * num_aux;
      cara_aux._1 = i + 1 + ((j + 1) % steps) * num_aux;
      cara_aux._2 = i + 1 + j * num_aux;
      caras.push_back(cara_aux);

      cara_aux._0 = i + 1 + j * num_aux;
      cara_aux._1 = i + j * num_aux;
      cara_aux._2 = i + ((j + 1) % steps) * num_aux;
      caras.push_back(cara_aux);
    }
  }

  if (tapaAbajo) generarCaraAbajo();
  if (tapaArriba) generarCaraArriba();

  generarNormales();
}

void _revolucion::generarCaraAbajo(bool generarPunto) {
  if (vertices.front().x and generarPunto) {
    _vertex3f puntoCentral(0.0, vertices.front().y, 0.0);
    vertices.push_back(puntoCentral);
  }

  for (int step = 0; step < steps; step++) {
    int perfilactual = step * Perfil.size();
    int siguientePerfil = perfilactual + Perfil.size();
    if (step == (steps - 1)) siguientePerfil = 0;
    caras.push_back(
        _vertex3i(vertices.size() - 1, siguientePerfil, perfilactual));
  }
}

void _revolucion::generarCaraArriba(bool generarPunto) {
  if (vertices[Perfil.size() - 1].x and generarPunto) {
    _vertex3f puntoCentral(0.0, vertices[Perfil.size() - 1].y, 0.0);
    vertices.push_back(puntoCentral);
  }

  for (int step = 0; step < steps; step++) {
    int perfilactual = (step + 1) * Perfil.size() - 1;
    int siguientePerfil = perfilactual + Perfil.size();
    if (step == (steps - 1)) siguientePerfil = Perfil.size() - 1;
    caras.push_back(
        _vertex3i(vertices.size() - 1, perfilactual, siguientePerfil));
  }
}

//************************************************************************
// objeto articulado: tanque
//************************************************************************

_chasis::_chasis() : base(0.5) {
  // perfil para un cilindro
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = 0.107;
  aux.y = -0.5;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 0.107;
  aux.y = 0.5;
  aux.z = 0.0;
  perfil.push_back(aux);
  rodamiento.nuevoPerfil(perfil);
  rodamiento.generarPerfil();
  altura = 0.22;
};

void _chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
                   float b2, float grosor) {
  glPushMatrix();
  glScalef(1.0, 0.22, 0.95);
  base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glRotatef(90.0, 1, 0, 0);
  rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.25, 0.0, 0.0);
  glRotatef(90.0, 1, 0, 0);
  rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.5, 0.0, 0.0);
  glRotatef(90.0, 1, 0, 0);
  rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.25, 0.0, 0.0);
  glRotatef(90.0, 1, 0, 0);
  rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5, 0.0, 0.0);
  glRotatef(90.0, 1, 0, 0);
  rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

//************************************************************************

_torreta::_torreta() : base(0.5), parte_trasera(0.5, 0.75) {
  altura = 0.18;
  anchura = 0.65;
};

void _torreta::draw(_modo modo, float r1, float g1, float b1, float r2,
                    float g2, float b2, float grosor) {
  glPushMatrix();
  glScalef(0.65, 0.18, 0.6);
  base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.325, 0, 0);
  glRotatef(90.0, 0, 0, 1);
  glScalef(0.18, 0.16, 0.6);
  parte_trasera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

//************************************************************************

_tubo::_tubo() {
  // perfil para un cilindro
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = 0.04;
  aux.y = -0.4;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 0.04;
  aux.y = 0.4;
  aux.z = 0.0;
  perfil.push_back(aux);
  tubo_abierto.nuevoPerfil(perfil);
  tubo_abierto.generarPerfil(false, false);
};

void _tubo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
                 float b2, float grosor) {
  glPushMatrix();
  glTranslatef(0.4, 0, 0);
  glRotatef(90.0, 0, 0, 1);
  tubo_abierto.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
}

//************************************************************************

_tanque::_tanque() {
  giro_tubo = 2.0;
  giro_torreta = 0.0;
  giro_tubo_min = -9;
  giro_tubo_max = 20;
};

void _tanque::draw(_modo modo, float r1, float g1, float b1, float r2, float g2,
                   float b2, float grosor) {
  glPushMatrix();
  chasis.draw(modo, r1, g1, b1, r2, g2, b2, grosor);

  glRotatef(giro_torreta, 0, 1, 0);
  glPushMatrix();
  glTranslatef(0.0, (chasis.altura + torreta.altura) / 2.0, 0.0);
  torreta.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(torreta.anchura / 2.0, (chasis.altura + torreta.altura) / 2.0,
               0.0);
  glRotatef(giro_tubo, 0, 0, 1);
  // std::cout << giro_tubo << std::endl;
  tubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
  glPopMatrix();
  glPopMatrix();
};

//************************************************************************
// objeto articulado: coche
//************************************************************************

_coche::_coche() {
  desplazamiento = 0;
  suspension = 0;
  giro_antena = 0;
  tope_suspension = 1.2;
  tope_suspension_min = -0.8;
  tope_desplazamiento = 15;
  tope_desplazamiento_min = -25;
  generarNormales();
};

void _coche::draw(_modo modo) {
  glPushMatrix();
  glTranslatef(10.0, 0.0, 0.0);
  glTranslatef(desplazamiento, 0.0, 0.0);
  draw_ruedas(modo);
  glPushMatrix();
  glTranslatef(0.0, suspension, 0.0);
  draw_chasis(modo);
  draw_cabina(modo);
  glPushMatrix();
  glTranslatef(4.5, 5.0, 0.6);
  glRotatef(giro_antena, 0, 1, 0);
  glTranslatef(-4.5, -5.0, -0.6);
  draw_antena(modo);
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
}
void _coche::draw_ruedas(_modo modo) {
  rueda.generarPerfil(true, true);

  // Pintamos las cuatro ruedas
  glPushMatrix();
  glRotatef(90.0, 1, 0, 0);

  glPushMatrix();
  glTranslatef(-4, 3, 0.0);
  rueda.draw(modo, 0, 0, 0, 0, 0, 0, 2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(4, -3, 0.0);
  rueda.draw(modo, 0, 0, 0, 0, 0, 0, 2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-4, -3, 0.0);
  rueda.draw(modo, 0, 0, 0, 0, 0, 0, 2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(4, 3, 0.0);
  rueda.draw(modo, 0, 0, 0, 0, 0, 0, 2);
  glPopMatrix();

  glPopMatrix();
};

void _coche::draw_chasis(_modo modo) {
  glPushMatrix();
  glScalef(5.5, 0.3, 3);
  chasis.draw(modo, 0, 1, 0, 0.6, 1, 0.4, 2);
  glPopMatrix();

  glPushMatrix();
  glScalef(0.9, 2.5, 3.4);
  glTranslatef(4.5, 1.0, 0.0);
  chasis.draw(modo, 0, 1, 0, 0.6, 1, 0.4, 2);
  glPopMatrix();

  glPushMatrix();
  glScalef(2.0, 1.0, 3);
  glTranslatef(1.0, 1.2, 0.0);
  chasis.draw(modo, 0, 1, 0, 0.6, 1, 0.4, 2);
  glPopMatrix();

  glPushMatrix();
  glScalef(2.0, 0.5, 3);
  glTranslatef(-1.0, 1.2, 0.0);
  chasis.draw(modo, 0, 1, 0, 0.6, 1, 0.4, 2);
  glPopMatrix();

  glPushMatrix();
  glScalef(2.0, 0.5, 3);
  glTranslatef(-2.0, 1.2, 0.0);
  glRotatef(90, 0, 0, 1);
  frontal.draw(modo, 0.6, 0.6, 0.4, 0.6, 0.6, 0.4, 2);
  glPopMatrix();
}

void _coche::draw_cabina(_modo modo) {
  int n = 12;

  double angle = (M_PI / 2 / n);
  vector<_vertex3f> perfil;
  perfil.push_back(_vertex3f(0.0, 1.0, 0.0));
  cabina.nuevoPerfil(perfil);
  for (unsigned int k = 0; k < n; k++)
    perfil.push_back(cabina.rotateZ(perfil[k], angle));

  cabina.nuevoPerfil(perfil);
  cabina.generarPerfil(false, false);

  glPushMatrix();
  glScalef(2, 2.5, 3);
  glTranslatef(1, 0.8, 0.0);
  cabina.draw(modo, 0.8, 0.8, 0.8, 0.9, 0.9, 0.9, 2);
  glPopMatrix();
  glPushMatrix();
  glScalef(2, 2.5, 3);
  glTranslatef(-1, 0.3, 0.0);
  cabina.draw(modo, 0.8, 0.8, 0.8, 0.9, 0.9, 0.9, 2);
  glPopMatrix();
}

void _coche::draw_antena(_modo modo) {
  int n = 12;

  double angle = (M_PI / 2 / n);
  vector<_vertex3f> perfil;
  perfil.push_back(_vertex3f(0.0, -1.0, 0.0));
  antena.nuevoPerfil(perfil);
  for (unsigned int k = 0; k < n; k++)
    perfil.push_back(antena.rotateZ(perfil[k], angle));

  antena.nuevoPerfil(perfil);
  antena.generarPerfil(false, false, 'y');
  glPushMatrix();
  glTranslatef(5.0, 7.0, 0.0);
  glRotatef(-45, 1, 0, 1);
  antena.draw(modo, 0.8, 0.8, 0.8, 0.9, 0.9, 0.9, 2);
  glPopMatrix();

  tubo.generarPerfil(false, false);
  glPushMatrix();
  glTranslatef(4.5, 5.0, 0.6);
  glScalef(0.1, 4.0, 0.1);
  tubo.draw(modo, 0.8, 0.8, 0.8, 0.9, 0.9, 0.9, 2);
  glPopMatrix();
}
