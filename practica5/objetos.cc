#include "objetos.h"
using namespace std;
using namespace _colors_ne;

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

void _triangulos3D::generarVectorSeleccion() {
  seleccionados.resize(caras.size());
  cout << caras.size() << endl;
  for (int i = 0; i < caras.size(); i++) {
    seleccionados[i] = false;
  }
}

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
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (seleccionados[i] == true)
      glColor3f(1, 1, 0);
    else
      glColor3f(r, g, b);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

void _triangulos3D::draw_selection() {
  float rgb[3];

  glBegin(GL_TRIANGLES);
  for (auto i = 0; i < caras.size(); i++) {
    rgb[0] = ((i & 0x00FF0000) >> 16) / 255.0;
    rgb[1] = ((i & 0x0000FF00) >> 8) / 255.0;
    rgb[2] = (i & 0x000000FF) / 255.0;
    glColor3f(rgb[0], rgb[1], rgb[2]);
    glPolygonMode(GL_FRONT, GL_FILL);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
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

    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

void _triangulos3D::draw_illumination_flat_shading() {
  int i;
  glPolygonMode(GL_FRONT, GL_FILL);
  glColor3fv((GLfloat *)&WHITE);
  glShadeModel(GL_FLAT);
  glEnable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_caras[i]);
    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(0, 1, 0);
  for (i = 0; i < caras.size(); i++) {
    auto v =
        vertices[caras[i]._0] + vertices[caras[i]._1] + vertices[caras[i]._2];
    v = v / 3.0;

    glVertex3fv((GLfloat *)&v);
    auto n = normales_caras[i] + v;
    glVertex3fv((GLfloat *)&n);
  }
  glEnd();
}

void _triangulos3D::draw_illumination_smooth_shading() {
  int i;
  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glColor3fv((GLfloat *)&WHITE);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

void _triangulos3D::textura_general() {
  GLfloat plano_s[4] = {1.0, 0.0, 0.0, 0.0}, plano_t[4] = {0.0, 1.0, 0.0, 0.0};
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv(GL_S, GL_OBJECT_PLANE, plano_s);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, plano_t);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
}
void _triangulos3D::draw_texture() {
  int i;
  if (Vertices_texture_coordinates.size() != vertices.size() and
      caras_texture_coordinates.size() != caras.size())
    textura_general();
  glPolygonMode(GL_FRONT, GL_FILL);
  glColor3fv((GLfloat *)&WHITE);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._0]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._1]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._2]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
}
void _triangulos3D::draw_texture_illumination_flat_shading() {
  int i;
  if (Vertices_texture_coordinates.size() == 0 and
      caras_texture_coordinates.size() == 0)
    textura_general();
  glPolygonMode(GL_FRONT, GL_FILL);
  glColor3fv((GLfloat *)&WHITE);
  glShadeModel(GL_FLAT);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_caras[i]);
    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._0]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._1]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._2]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
}
void _triangulos3D::draw_texture_illumination_smooth_shading() {
  int i;
  if (Vertices_texture_coordinates.size() == 0 and
      caras_texture_coordinates.size() == 0)
    textura_general();
  glPolygonMode(GL_FRONT, GL_FILL);
  glColor3fv((GLfloat *)&WHITE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++) {
    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._0]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._1]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

    if (normales_vertices.size() > 0)
      glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
    if (Vertices_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&Vertices_texture_coordinates[caras[i]._2]);
    else if (caras_texture_coordinates.size() > 0)
      glTexCoord2fv((GLfloat *)&caras_texture_coordinates[i][2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2,
                         float g2, float b2, float grosor) {
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
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
      break;
    case ILLUMINATION_FLAT_SHADING:
      draw_illumination_flat_shading();
      break;
    case ILLUMINATION_SMOOTH_SHADING:
      draw_illumination_smooth_shading();
      break;
    case TEXTURE:
      draw_texture();
      break;
    case TEXTURE_ILLUMINATION_FLAT_SHADING:
      draw_texture_illumination_flat_shading();
      break;
    case TEXTURE_ILLUMINATION_SMOOTH_SHADING:
      draw_texture_illumination_smooth_shading();
      break;

    case SELECTION:
      draw_selection();
      break;
  }
  // drawNormales();
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

void _triangulos3D::generarNormalesExamen() {
  int M_x = 0, M_y = 0, M_z = 0, m_x = 0, m_y = 0, m_z = 0;
  // Recorremos los vertices
  for (int i = 0; i < vertices.size(); i++) {
    _vertex3f punto(vertices[i]);
    if (punto.x > M_x) M_x = punto.x;
    if (punto.y > M_y) M_y = punto.y;
    if (punto.z > M_z) M_z = punto.z;
    if (punto.x < m_x) m_x = punto.x;
    if (punto.y < m_y) m_y = punto.y;
    if (punto.z < m_z) m_z = punto.z;
  }
  _vertex3f centro((M_x + m_x) / 2, (M_y + m_y) / 2, (M_z + m_z) / 2);

  for (int i = 0; i < vertices.size(); i++) {
    _vertex3f punto(vertices[i]);
    _vertex3f normal(punto.x - centro.x, punto.y - centro.y,
                     punto.z - centro.z);

    normal = normal.normalize();
    normales_vertices.push_back(normal);
  }
}

void _triangulos3D::generarNormales() {
  generarNormalesCaras();
  generarNormalesExamen();
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

_chess_board::_chess_board(float Size, unsigned int Divisions1) {
  vertices.push_back(_vertex3f(0, 0, -Size / 2));
  vertices.push_back(_vertex3f(0, 0, Size / 2));
  vertices.push_back(_vertex3f(0, Size, Size / 2));
  vertices.push_back(_vertex3f(0, Size, -Size / 2));

  caras.push_back(_vertex3i(0, 1, 3));
  caras.push_back(_vertex3i(1, 2, 3));

  generarNormales();

  Vertices_texture_coordinates.push_back(_vertex2f(0, 1));
  Vertices_texture_coordinates.push_back(_vertex2f(1, 1));
  Vertices_texture_coordinates.push_back(_vertex2f(1, 0));
  Vertices_texture_coordinates.push_back(_vertex2f(0, 0));
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
  caras.push_back(_vertex3i(0, 1, 3));
  caras.push_back(_vertex3i(3, 1, 2));

  caras.push_back(_vertex3i(1, 5, 2));
  caras.push_back(_vertex3i(2, 5, 6));

  caras.push_back(_vertex3i(5, 4, 6));
  caras.push_back(_vertex3i(6, 4, 7));

  caras.push_back(_vertex3i(4, 0, 7));
  caras.push_back(_vertex3i(7, 0, 3));

  caras.push_back(_vertex3i(3, 2, 7));
  caras.push_back(_vertex3i(7, 2, 6));

  caras.push_back(_vertex3i(0, 4, 1));
  caras.push_back(_vertex3i(1, 4, 5));

  // texels
  // cara 1
  caras_texture_coordinates.resize(12);
  caras_texture_coordinates[0].push_back(_vertex2f(0, 1));
  caras_texture_coordinates[0].push_back(_vertex2f(0.5, 1));
  caras_texture_coordinates[0].push_back(_vertex2f(0, 0.5));
  caras_texture_coordinates[1].push_back(_vertex2f(0, 0.5));
  caras_texture_coordinates[1].push_back(_vertex2f(0.5, 1));
  caras_texture_coordinates[1].push_back(_vertex2f(0.5, 0.5));

  caras_texture_coordinates[4].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[4].push_back(_vertex2f(0, 0.5));
  caras_texture_coordinates[4].push_back(_vertex2f(0.5, 1));
  caras_texture_coordinates[5].push_back(_vertex2f(0.5, 1));
  caras_texture_coordinates[5].push_back(_vertex2f(0, 0.5));
  caras_texture_coordinates[5].push_back(_vertex2f(0, 1));

  caras_texture_coordinates[2].push_back(_vertex2f(1, 0.5));
  caras_texture_coordinates[2].push_back(_vertex2f(1, 0));
  caras_texture_coordinates[2].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[3].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[3].push_back(_vertex2f(1, 0));
  caras_texture_coordinates[3].push_back(_vertex2f(0.5, 0));

  caras_texture_coordinates[6].push_back(_vertex2f(1, 0.5));
  caras_texture_coordinates[6].push_back(_vertex2f(1, 0));
  caras_texture_coordinates[6].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[7].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[7].push_back(_vertex2f(1, 0));
  caras_texture_coordinates[7].push_back(_vertex2f(0.5, 0));

  caras_texture_coordinates[8].push_back(_vertex2f(0, 0.5));
  caras_texture_coordinates[8].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[8].push_back(_vertex2f(0, 0));
  caras_texture_coordinates[9].push_back(_vertex2f(0, 0));
  caras_texture_coordinates[9].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[9].push_back(_vertex2f(0.5, 0));

  caras_texture_coordinates[10].push_back(_vertex2f(0.5, 1));
  caras_texture_coordinates[10].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[10].push_back(_vertex2f(1, 1));
  caras_texture_coordinates[11].push_back(_vertex2f(1, 1));
  caras_texture_coordinates[11].push_back(_vertex2f(0.5, 0.5));
  caras_texture_coordinates[11].push_back(_vertex2f(1, 0.5));

  generarNormales();
  generarVectorSeleccion();
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
  caras_texture_coordinates.resize(2);
  caras_texture_coordinates[0].push_back(_vertex2f(0, 0));
  caras_texture_coordinates[0].push_back(_vertex2f(1, 0));
  caras_texture_coordinates[0].push_back(_vertex2f(0.5, 1));
  caras_texture_coordinates[1].push_back(_vertex2f(0, 0));
  caras_texture_coordinates[1].push_back(_vertex2f(1, 0));
  caras_texture_coordinates[1].push_back(_vertex2f(0.5, 1));

  generarVectorSeleccion();
}

//*************************************************************************
// clase cilindro
//*************************************************************************

_cilindro::_cilindro() {
  // perfil para un cilindro
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = .5;
  aux.y = -0.7;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = .5;
  aux.y = 0.7;
  aux.z = 0.0;
  perfil.push_back(aux);
  nuevoPerfil(perfil);
  generarPerfil();
  generarNormales();
  generarVectorSeleccion();
}
//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply(const char *archivo) {
  // leer lista de coordenadas de vértices y lista de indices de vértices
  lector.lee_ply(vertices, caras, archivo);
  generarNormales();
  generarVectorSeleccion();
}

_esfera::_esfera() {
  div = 12;

  double angle = (M_PI / div);
  vector<_vertex3f> perfil;
  vertices.push_back(_vertex3f(0.0, -1.0, 0.0));
  nuevoPerfil(perfil);
  for (unsigned int k = 0; k < div; k++) {
    vertices.push_back(rotateZ(vertices[k], angle));
  }

  _vertex3f vertice_sup = vertices.back();
  _vertex3f vertice_inf = vertices.front();
  vertices.erase(vertices.begin());
  vertices.erase(vertices.end());

  nuevoPerfil(vertices);
  generarPerfil(0, 0);
  vertices.push_back(vertice_inf);
  generarCaraAbajo(false);
  vertices.push_back(vertice_sup);
  generarCaraArriba(false);
  generarNormales();
  generarVectorSeleccion();
}

_cono::_cono() {
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.x = 0.5;
  aux.y = 0.0;
  aux.z = 0.0;
  perfil.push_back(aux);
  nuevoPerfil(perfil);
  generarPerfil(0);
  vertices.push_back(_vertex3f(0.0, 1.0, 0.0));
  generarCaraArriba(false);
  generarNormales();
  generarVectorSeleccion();
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

void _revolucion::nuevoPerfil(const vector<_vertex3f> &perfil) {
  Perfil.erase(Perfil.begin(), Perfil.end());
  Perfil = perfil;
}

void _revolucion::generarPerfil(bool tapaArriba, bool tapaAbajo, char eje) {
  int i, j;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int p_size;
  eje = toupper(eje);

  p_size = Perfil.size();
  double angle = (2.0 * M_PI) / steps;

  for (int step = 0; step < steps; step++) {
    vector<_vertex3f> siguientePerfil(p_size);
    for (unsigned int k = 0; k < p_size; k++)
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
    for (i = 0; i < p_size - 1; i++) {
      cara_aux._0 = i + ((j + 1) % steps) * p_size;
      cara_aux._1 = i + 1 + ((j + 1) % steps) * p_size;
      cara_aux._2 = i + 1 + j * p_size;
      caras.push_back(cara_aux);

      cara_aux._0 = i + 1 + j * p_size;
      cara_aux._1 = i + j * p_size;
      cara_aux._2 = i + ((j + 1) % steps) * p_size;
      caras.push_back(cara_aux);
    }
  }

  if (tapaAbajo) generarCaraAbajo();
  if (tapaArriba) generarCaraArriba();

  calcular_coord_textura(steps, Perfil.size());
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
        _vertex3i(perfilactual, siguientePerfil, vertices.size() - 1));
  }
}

void _revolucion::calcular_coord_textura(int N, int M) {
  int n_vertices = vertices.size();

  double d[M];  // vector de distancias
  d[0] = 0;

  for (unsigned int k = 1; k < M; k++)
    d[k] = d[k - 1] + distancia(vertices[k - 1], vertices[k]);

  for (unsigned int i = 0; i <= N; i++) {
    float si = (float)i / (N - 1);
    for (unsigned int j = 0; j < M; j++) {
      float tj = d[j] / d[M - 1];

      Vertices_texture_coordinates.push_back(_vertex2f(tj, si));
    }
  }
}

double _revolucion::distancia(_vertex3f a, _vertex3f b) {
  double x = pow((b.x - a.x), 2);
  double y = pow((b.y - a.y), 2);
  double z = pow((b.z - a.z), 2);

  return sqrt((double)(x + y + z));
}