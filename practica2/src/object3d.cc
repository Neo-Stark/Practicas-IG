
#include "object3d.h"

using namespace _colors_ne;

/**
 *
 *@param
 *@returns
 */

void _object3D::draw_line() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  for (unsigned int i = 0; i < Triangles.size(); i++) {
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._0]);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._1]);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._2]);
  }
  glEnd();
}

/**
 *
 *@param
 *@returns
 */

void _object3D::draw_fill() {
  glPolygonMode(GL_FRONT, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (unsigned int i = 0; i < Triangles.size(); i++) {
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._0]);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._1]);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._2]);
  }
  glEnd();
}

/**
 *
 *@param
 *@returns
 */

void _object3D::draw_chess() {
  glPolygonMode(GL_FRONT, GL_FILL);
  glBegin(GL_TRIANGLES);
  static int cont = 0;
  for (unsigned int i = 0; i < Vertices.size(); i++) {
    if (cont < 20)
      Vertices[i].x += 0.1;
    else {
      Vertices[i].x -= 2;
    }
  }
  std::cout << cont << std::endl;
  cont == 20 ? cont = 0 : cont++;
  for (unsigned int i = 0; i < Triangles.size(); i++) {
    if (i % 2 == 0)
      glColor3fv((GLfloat *)&BLACK);
    else
      glColor3fv((GLfloat *)&GREEN);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._0]);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._1]);
    glVertex3fv((GLfloat *)&Vertices[Triangles[i]._2]);
  }
  glEnd();
}
