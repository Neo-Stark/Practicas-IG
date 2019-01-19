//**************************************************************************
// Práctica 4
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos.h"
#include "textura.h"

using namespace std;

// tipos
typedef enum {
  CUBO,
  PIRAMIDE,
  OBJETO_PLY,
  ESFERA,
  ARTICULADO,
  COCHE,
  CILINDRO,
  CONO,
  TABLERO
} _tipo_objeto;
_tipo_objeto t_objeto = ESFERA;
_modo modo = SOLID;

unsigned int Material_active = 0;
bool animar_luz = false;
bool animar = false;
bool luz_01 = true;
bool luz_00 = true;
bool interpolar_material = false;
_puntos3D punto_luz01;

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
_esfera esfera;
_tanque tanque;
_coche coche;
_cilindro cilindro;
_textura *textura;
_cono cono;
_chess_board tablero;

_triangulos3D *ply1;

void clean_window() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

//**************************************************************************
//Practica 4: Luces y materiales
//***************************************************************************
#pragma region luces
vector<_vertex4f> Material_ambient;
vector<_vertex4f> Material_diffuse;
vector<_vertex4f> Material_specular;
vector<float> Material_shininess;

float interpolacion(float n, float a, float b) { return a * n + b * (1 - n); }

void interpolacion_material(float n, const _vertex4f &a, const _vertex4f &b,
                            _vertex4f &m) {
  m.r = interpolacion(n, a.r, b.r);
  m.g = interpolacion(n, a.g, b.g);
  m.b = interpolacion(n, a.b, b.b);
  m.a = interpolacion(n, a.a, b.a);
  cout << "r " << m.r << endl;
  cout << "g " << m.g << endl;
  cout << "b " << m.b << endl;
  cout << "N " << n << endl;
}
void girar_luz(_vertex3f &punto) {
  _revolucion giro;
  float angle = 2 * M_PI / 360;
  punto = giro.rotateY(punto, angle);
}

void EnableLight01(void) {
  if (luz_01) {
    glEnable(GL_LIGHT1);  // enable light 1

    GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[] = {0.0, 1.0, 0.0, 1.0};
    GLfloat light_specular[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_position[] = {10.0, -10.0, 10.0, 1.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  } else
    glDisable(GL_LIGHT1);
}


void EnableLight0() {
  _vertex4f Position(-1, -1, 1, 0);
  if (luz_00) {
    glEnable(GL_LIGHT0);

    GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[] = {0.9, 0.75, 0.3, 1.0};
    GLfloat light_specular[] = {0.5, 0.6, 0.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&Position);
    glPopMatrix();
  } else
    glDisable(GL_LIGHT0);
}

void set_materials() {
  // Material 1
  Material_ambient.push_back(_vertex4f(0.24725f, 0.1995f, 0.0745f, 1.0));
  Material_diffuse.push_back(_vertex4f(0.75164f, 0.60648f, 0.22648f, 1.0));
  Material_specular.push_back(_vertex4f(0.628281f, 0.555802f, 0.366065f, 1.0));
  Material_shininess.push_back(51.2);
  // Material 2
  Material_ambient.push_back(_vertex4f(0.0215f, 0.1745f, 0.0215f, 0.5));
  Material_diffuse.push_back(_vertex4f(0.07568f, 0.61424f, 0.07568f, 0.5));
  Material_specular.push_back(_vertex4f(0.633f, 0.727811f, 0.633f, 0.5));
  Material_shininess.push_back(76.8);
  // // Material 3
  // Material_ambient.push_back(_vertex3f(0.24725f, 0.1995f, 0.0745f));
  // Material_diffuse.push_back(_vertex3f(0.75164f, 0.60648f, 0.22648f));
  // Material_specular.push_back(_vertex3f(0.628281f, 0.555802f, 0.366065f));
  // Material_shininess.push_back(0.6);
  // // Material interpolacion (empieza en Material 2 y va hasta Material 3)
  // Material_ambient.push_back(_vertex3f(0.24725f, 0.1995f, 0.0745f));
  // Material_diffuse.push_back(_vertex3f(0.75164f, 0.60648f, 0.22648f));
  // Material_specular.push_back(_vertex3f(0.628281f, 0.555802f, 0.366065f));
  // Material_shininess.push_back(0.6);
}
void change_materials() {
  switch (Material_active) {
    case 0: {
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                   (GLfloat *)&Material_diffuse[0]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   (GLfloat *)&Material_specular[0]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                   (GLfloat *)&Material_ambient[0]);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_shininess[0]);
    } break;
    case 1:
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                   (GLfloat *)&Material_diffuse[1]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   (GLfloat *)&Material_specular[1]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                   (GLfloat *)&Material_ambient[1]);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_shininess[1]);
      break;
    case 2:
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                   (GLfloat *)&Material_diffuse[2]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   (GLfloat *)&Material_specular[2]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                   (GLfloat *)&Material_ambient[2]);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_shininess[2]);
      break;
    case 3:
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                   (GLfloat *)&Material_diffuse[3]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   (GLfloat *)&Material_specular[3]);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                   (GLfloat *)&Material_ambient[3]);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_shininess[3]);
      break;
  }
}

#pragma endregion 
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
// Funcion para definir la transformación de vista (posicionar la camara)
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
  // _vertex4f Ambient(0.1, 0.1, 0.1, 1);
  // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)&Ambient);
  switch (t_objeto) {
    case CUBO:
      cubo.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case CILINDRO:
      cilindro.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case CONO:
      cono.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case PIRAMIDE:
      piramide.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case OBJETO_PLY:
      ply1->draw(modo, 1.0, 0.6, 0.0, 0.0, 1.0, 0.3, 2);
      break;
    case ESFERA:
      esfera.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
    case ARTICULADO:
      tanque.draw(modo, 0.5, 0.7, 0.2, 0.3, 0.6, 0.3, 2);
      break;
    case COCHE:
      coche.draw(modo);
      break;
    case TABLERO:
      tablero.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
      break;
  }
  punto_luz01.draw_puntos(1, 0, 1, 10);
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

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

/***********************************************************************************************/
void normal_key(unsigned char Tecla1, int x, int y) {
  switch (toupper(Tecla1)) {
    case 'Q':
      exit(0);
    case '1':
      t_objeto = PIRAMIDE;
      break;
    case '2':
      t_objeto = CUBO;
      break;
    case '3':
      t_objeto = CONO;
      break;
    case '4':
      t_objeto = CILINDRO;
      break;
    case '5':
      t_objeto = ESFERA;
      break;
    case '6':
      t_objeto = OBJETO_PLY;
      break;
    case '7':
      t_objeto = COCHE;
      break;
    case '8':
      t_objeto = TABLERO;
      break;

    case 'H':
      animar_luz = !animar_luz;
      break;
    case 'S':
      animar = !animar;
      break;
    case 'N':
      interpolar_material = !interpolar_material;
      break;
    case 'M':
      Material_active = (Material_active + 1) % 2;
      break;
    case 'J':
      luz_00 = !luz_00;
      break;
    case 'K':
      luz_01 = !luz_01;
      break;

    case 'E':
      coche.giro_antena += 5;
      break;
    case 'W':
      coche.giro_antena -= 5;
      break;
    case 'F':
      coche.desplazamiento += 1;
      if (coche.desplazamiento > coche.tope_desplazamiento)
        coche.desplazamiento = coche.tope_desplazamiento;
      break;
    case 'D':
      coche.desplazamiento -= 1;
      if (coche.desplazamiento < coche.tope_desplazamiento_min)
        coche.desplazamiento = coche.tope_desplazamiento_min;
      break;
    case 'Z':
      coche.suspension += 0.2;
      if (coche.suspension > coche.tope_suspension)
        coche.suspension = coche.tope_suspension;
      break;
    case 'X':
      coche.suspension -= 0.2;
      if (coche.suspension < coche.tope_suspension_min)
        coche.suspension = coche.tope_suspension_min;
      break;
    case 'R':
      set_materials();
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
      modo = SOLID;
      break;
    case GLUT_KEY_F2:
      modo = SOLID_CHESS;
      break;
    case GLUT_KEY_F3:
      modo = ILLUMINATION_FLAT_SHADING;
      break;
    case GLUT_KEY_F4:
      modo = ILLUMINATION_SMOOTH_SHADING;
      break;
    // case GLUT_KEY_F5:
    //   modo = TEXTURE;
    //   break;
    // case GLUT_KEY_F6:
    //   modo = TEXTURE_ILLUMINATION_FLAT_SHADING;
    //   break;
    // case GLUT_KEY_F7:
    //   modo = TEXTURE_ILLUMINATION_SMOOTH_SHADING;
    //   break;
  }
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
  Observer_distance = 10 * Front_plane;
  Observer_angle_x = 25;
  Observer_angle_y = 25;

  // se indica cual sera el color para limpiar la ventana	(r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1, 1, 1, 1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);

  // punto_luz01.vertices.push_back(_vertex3f(2, 2, 2));
  set_materials();

  change_projection();
  glViewport(0, 0, Window_width, Window_high);

  textura = new _textura("images/caja.jpg");
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
  static float angulo = 0;
  static float n = 0;
  static int cont = 0;
  static int cont2 = 0;
  static bool arriba = true;
  if (animar_luz) {
    angulo = (angulo + 0.05);
    cont2++;
    if (cont2 == 1500) {
      cont2 = 0;
      angulo = 0;
    }
    // girar_luz(punto_luz01.vertices[0]);
  }
  glPushMatrix();
  glTranslatef(0, angulo, 0);
  EnableLight01();
  glPopMatrix();

  if (interpolar_material) {
    if (arriba) {
      cont++;
      n = (n + 0.001);
    } else {
      cont--;
      n = (n - 0.001);
    }
    if (cont == 1000 or cont == 0) arriba = !arriba;
    // cout << "contador:  " << n << endl;

    interpolacion_material(n, Material_ambient[2], Material_ambient[1],
                           Material_ambient[3]);
    interpolacion_material(n, Material_diffuse[2], Material_diffuse[1],
                           Material_diffuse[3]);
    interpolacion_material(n, Material_specular[2], Material_specular[1],
                           Material_specular[3]);
    Material_shininess[3] =
        interpolacion(n, Material_shininess[1], Material_shininess[2]);
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

//**************************************************************************
//
//***************************************************************************

void draw(void) {
  clean_window();
  change_observer();
  draw_axis();
  draw_objects();
  change_materials();

  EnableLight0();
  EnableLight01();
  glutSwapBuffers();
}
//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv) {
  // creación del objeto ply
  if (argc < 2) {
    ply1 = new _objeto_ply("ply/beethoven.ply");
  } else
    ply1 = new _objeto_ply(argv[1]);

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
