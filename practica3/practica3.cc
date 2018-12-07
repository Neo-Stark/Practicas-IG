//**************************************************************************
// Práctica 3
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos.h"

using namespace std;

// tipos
typedef enum {
  CUBO,
  PIRAMIDE,
  OBJETO_PLY,
  ROTACION,
  ARTICULADO,
  COCHE
} _tipo_objeto;
_tipo_objeto t_objeto = COCHE;
_modo modo = SOLID_CHESS;

bool animar = false;
// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 450, Window_high = 450;

// objetos
_cubo cubo;
_piramide piramide(0.85, 1.3);
_objeto_ply ply;
_revolucion rotacion;
_tanque tanque;
_coche coche;

// _objeto_ply *ply1;

//**************************************************************************
//
//***************************************************************************

void clean_window() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero,
  // plano_traser)
  //  plano_delantero>0  plano_trasero>PlanoDelantero)
  glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer() {
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -Observer_distance);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis() {
  glDisable(GL_LIGHTING);
  glLineWidth(2);
  glBegin(GL_LINES);
  // eje X, color rojo
  glColor3f(1, 0, 0);
  glVertex3f(-AXIS_SIZE, 0, 0);
  glVertex3f(AXIS_SIZE, 0, 0);
  // eje Y, color verde
  glColor3f(0, 1, 0);
  glVertex3f(0, -AXIS_SIZE, 0);
  glVertex3f(0, AXIS_SIZE, 0);
  // eje Z, color azul
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, -AXIS_SIZE);
  glVertex3f(0, 0, AXIS_SIZE);
  glEnd();
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects() {
  switch (t_objeto) {
    case CUBO:
      cubo.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case PIRAMIDE:
      piramide.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case OBJETO_PLY:
      ply.draw(modo, 1.0, 0.6, 0.0, 0.0, 1.0, 0.3, 2);
      break;
    case ROTACION:
      rotacion.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case ARTICULADO:
      tanque.draw(modo, 0.5, 0.7, 0.2, 0.3, 0.6, 0.3, 2);
      break;
    case COCHE:
      coche.draw(modo);
  }
}

//**************************************************************************
//
//***************************************************************************

void draw(void) {
  clean_window();
  change_observer();
  draw_axis();
  draw_objects();
  glutSwapBuffers();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1) {
  float Aspect_ratio;

  Aspect_ratio = (float)Alto1 / (float)Ancho1;
  Size_y = Size_x * Aspect_ratio;
  change_projection();
  glViewport(0, 0, Ancho1, Alto1);
  glutPostRedisplay();
}

//**********-o*****************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1, int x, int y) {
  switch (toupper(Tecla1)) {
    case 'Q':
      exit(0);
    case '1':
      t_objeto = CUBO;
      break;
    case '2':
      modo = EDGES;
      break;
    case '3':
      modo = SOLID;
      break;
    case '4':
      modo = SOLID_CHESS;
      break;
    case 'P':
      t_objeto = PIRAMIDE;
      break;
    case 'C':
      t_objeto = CUBO;
      break;
    case 'O':
      t_objeto = OBJETO_PLY;
      break;
    case 'R':
      t_objeto = ROTACION;
      break;
    case 'A':
      t_objeto = ARTICULADO;
      break;
    case 'H':
      t_objeto = COCHE;
      break;
    case 'S':
      animar = !animar;
      break;
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1, int x, int y) {
  switch (Tecla1) {
    case GLUT_KEY_LEFT:
      Observer_angle_y--;
      break;
    case GLUT_KEY_RIGHT:
      Observer_angle_y++;
      break;
    case GLUT_KEY_UP:
      Observer_angle_x--;
      break;
    case GLUT_KEY_DOWN:
      Observer_angle_x++;
      break;
    case GLUT_KEY_PAGE_UP:
      Observer_distance *= 1.2;
      break;
    case GLUT_KEY_PAGE_DOWN:
      Observer_distance /= 1.2;
      break;
    case GLUT_KEY_F1:
      tanque.giro_tubo += 1;
      if (tanque.giro_tubo > tanque.giro_tubo_max)
        tanque.giro_tubo = tanque.giro_tubo_max;
      break;
    case GLUT_KEY_F2:
      tanque.giro_tubo -= 1;
      if (tanque.giro_tubo < tanque.giro_tubo_min)
        tanque.giro_tubo = tanque.giro_tubo_min;
      break;
      break;
    case GLUT_KEY_F3:
      tanque.giro_torreta += 5;
      break;
    case GLUT_KEY_F4:
      tanque.giro_torreta -= 5;
      break;
    case GLUT_KEY_F5:
      coche.giro_antena += 5;
      break;
    case GLUT_KEY_F6:
      coche.giro_antena -= 5;
      break;
    case GLUT_KEY_F7:
      coche.desplazamiento += 1;
      if (coche.desplazamiento > coche.tope_desplazamiento)
        coche.desplazamiento = coche.tope_desplazamiento;
      break;
    case GLUT_KEY_F8:
      coche.desplazamiento -= 1;
      if (coche.desplazamiento < coche.tope_desplazamiento_min)
        coche.desplazamiento = coche.tope_desplazamiento_min;
      break;
    case GLUT_KEY_F9:
      coche.suspension += 0.2;
      if (coche.suspension > coche.tope_suspension)
        coche.suspension = coche.tope_suspension;
      break;
    case GLUT_KEY_F10:
      coche.suspension -= 0.2;
      if (coche.suspension < coche.tope_suspension_min)
        coche.suspension = coche.tope_suspension_min;
      break;
  }
  // std::cout << coche.desplazamiento << ' ' << coche.tope_desplazamiento << '
  // '
  //           << coche.tope_desplazamiento_min << ' ' << coche.giro_antena << '
  //           '
  //           << coche.tope_suspension << ' ' << coche.suspension << std::endl;
  glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void) {
  // se inicalizan la ventana y los planos de corte
  Size_x = 0.5;
  Size_y = 0.5;
  Front_plane = 1;
  Back_plane = 1000;

  // se incia la posicion del observador, en el eje z
  Observer_distance = 32 * Front_plane;
  Observer_angle_x = 25;
  Observer_angle_y = 25;

  // se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1, 1, 1, 1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  change_projection();
  glViewport(0, 0, Window_width, Window_high);
}
//***************************************************************************
// Funcion de animacion
//***************************************************************************
void animacion() {
  static int susp = true;
  if (animar && t_objeto == COCHE) {
    coche.desplazamiento -= 1;
    if (coche.desplazamiento < coche.tope_desplazamiento_min)
      coche.desplazamiento = 0;
    coche.giro_antena += 30;
    if (coche.giro_antena > 360) coche.giro_antena = 0;
    if (susp) {
      coche.suspension += 0.5;
      susp = false;
    } else {
      coche.suspension -= 0.5;
      susp = true;
    }
  }
  glutPostRedisplay();
}

void menu() {
  cout << endl << endl;
  cout << "C --> ver Cubo" << endl;
  cout << "T --> ver Tetraedro" << endl;
  cout << "P --> ver Piramide" << endl;
  cout << "H --> ver objeto Jerarquico Homer Coche" << endl;
  cout << "A --> ver objeto Jerarquico Tanque" << endl;
  cout << "R --> ver objeto de Revolucion peón" << endl;
  cout << "1 --> modo puntos" << endl;
  cout << "2 --> modo lineas" << endl;
  cout << "3 --> modo solido" << endl;
  cout << "4 --> modo ajedrez" << endl;
  cout << "S --> comenzar/detener animación" << endl;
  cout << "F1/F2 --> girar tubo tanque" << endl;
  cout << "F3/F4 --> girar torreta tanque" << endl;
  cout << "F5/F6 --> girar antena homer car" << endl;
  cout << "F7/F8 --> mover homer car" << endl;
  cout << "F9/F10 --> aumentar/disminuir suspension homer car" << endl;
  cout << "Q --> salir" << endl;
}
//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Uso: " << argv[0] << " arcvhivo.ply" << endl;
    exit(1);
  }
  // creación del objeto ply
  ply.parametros(argv[1]);

  // creacion del objeto por revolucion
  _file_ply lector;
  // lector.lee_ply(rotacion.vertices, rotacion.caras, "peon.ply");
  int n = 12;

  double angle = (M_PI / n);
  vector<_vertex3f> perfil;
  rotacion.vertices.push_back(_vertex3f(0.0, 1.0, 0.0));
  rotacion.nuevoPerfil(perfil);
  for (unsigned int k = 0; k < n; k++) {
    if (rotacion.vertices[k].y <= 0.0)
      rotacion.vertices.push_back(
          rotacion.rotateZ(rotacion.vertices[k], angle));
    else {
      _vertex3f aux = rotacion.rotateZ(rotacion.vertices[k], angle);
      aux.x -= 1;
      rotacion.vertices.push_back(aux);
    }
  }
  // rotacion.limpiar();
  // float x, y;
  // int Size = 1;
  // for (int i = -5; i <= 0; i++) {
  //   x = Size * cos(M_PI * i / 10);
  //   y = Size * sin(M_PI * i / 10);
  //   rotacion.vertices.push_back(_vertex3f(x, y, 0));
  // }

  // // for (int i = 5; i >= 0; i--) {
  // for (int i = -5; i <= 0; i++) {
  //   x = -Size * cos(M_PI * i / 10);
  //   y = Size * sin(M_PI * i / 10);
  //   rotacion.vertices.push_back(_vertex3f(x + Size, y + Size, 0));
  // }

  rotacion.nuevoPerfil(rotacion.vertices);
  rotacion.generarPerfil(0, 0);

  // se llama a la inicialización de glut
  glutInit(&argc, argv);

  // se indica las caracteristicas que se desean para la visualización con
  // OpenGL Las posibilidades son: GLUT_SIMPLE -> memoria de imagen simple
  // GLUT_DOUBLE -> memoria de imagen doble
  // GLUT_INDEX -> memoria de imagen con color indizado
  // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada
  // pixel GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y
  // alfa para cada pixel GLUT_DEPTH -> memoria de profundidad o z-bufer
  // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  // posicion de la esquina inferior izquierdad de la ventana
  glutInitWindowPosition(Window_x, Window_y);

  // tamaño de la ventana (ancho y alto)
  glutInitWindowSize(Window_width, Window_high);

  // llamada para crear la ventana, indicando el titulo (no se visualiza hasta
  // que se llama al bucle de eventos)
  glutCreateWindow("PRACTICAS IG");

  // asignación de la funcion llamada "dibujar" al evento de dibujo
  glutDisplayFunc(draw);
  // asignación de la funcion llamada "change_window_size" al evento
  // correspondiente
  glutReshapeFunc(change_window_size);
  // asignación de la funcion llamada "normal_key" al evento correspondiente
  glutKeyboardFunc(normal_key);
  // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
  glutSpecialFunc(special_key);

  // Animacion
  glutIdleFunc(animacion);

  // funcion de inicialización
  initialize();

  // mostramos el menu de opciones
  menu();
  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
