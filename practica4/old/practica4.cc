/*
 * Practicas de IG
 * Domingo Martin Perandres© 2014-2018
 * dmartin@ugr.es
 *
 * GPL 3
 */

#include <GL/gl.h>
#include <GL/glut.h>
#include <ctype.h>
#include "stdio.h"
#include "stdlib.h"

#include "axis.h"
#include "chess_board.h"
#include "colors.h"
#include "cone.h"
#include "cylinder.h"
#include "file_ply_stl.h"
#include "level2.h"
#include "level3.h"
#include "object_ply.h"
#include "sphere.h"
#include "tetrahedron.h"
#include "vertex.h"

using namespace _colors_ne;

const float X_MIN = -.1;
const float X_MAX = .1;
const float Y_MIN = -.1;
const float Y_MAX = .1;
const float FRONT_PLANE_PERSPECTIVE = (X_MAX - X_MIN) / 2;
const float BACK_PLANE_PERSPECTIVE = 1000;
const float DEFAULT_DISTANCE = 2;
const float ANGLE_STEP = 1;

typedef enum {
  MODE_DRAW_POINT,
  MODE_DRAW_LINE,
  MODE_DRAW_FILL,
  MODE_DRAW_CHESS
} _mode_draw;
typedef enum {
  OBJECT_TETRAHEDRON,
  OBJECT_CUBE,
  OBJECT_CONE,
  OBJECT_CYLINDER,
  OBJECT_SPHERE,
  OBJECT_PLY,
  OBJECT_HIERARCHICAL,
  OBJECT_CHESS_BOARD
} _object;
typedef enum {
  MODE_RENDERING_SOLID,
  MODE_RENDERING_SOLID_CHESS,
  MODE_RENDERING_ILLUMINATION_FLAT_SHADING,
  MODE_RENDERING_ILLUMINATION_SMOOTH_SHADING,
  MODE_RENDERING_TEXTURE,
  MODE_RENDERING_TEXTURE_ILLUMINATION_FLAT_SHADING,
  MODE_RENDERING_TEXTURE_ILLUMINATION_SMOOTH_SHADING
} _mode_rendering;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_angle_x = 0;
GLfloat Observer_angle_y = 0;
GLfloat Observer_distance = DEFAULT_DISTANCE;

// variables que controlan la ventana y la transformacion de perspectiva
// GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x = 50, UI_window_pos_y = 50, UI_window_width = 800,
    UI_window_height = 800;

_axis Axis;
_tetrahedron Tetrahedron;
_cone Cone;
_cylinder Cylinder;
_sphere Sphere;
_object_ply Object_ply;
_level3 Level3;
_chess_board Chess_board;

unsigned int Material_active = 0;
bool Light0_active = true;
bool Light1_active = false;
float Light1_angle = 0;

_mode_rendering Mode_rendering = MODE_RENDERING_SOLID;

bool Draw_point = false;
bool Draw_line = true;
bool Draw_fill = false;
bool Draw_chess = false;

_object Object = OBJECT_TETRAHEDRON;

const float INC_LIGHT1_ANGLE = 1;

/***********************************************************************************************/

/**
 *
 *@param
 *@returns
 */
void set_lights() {
  if (Light0_active) {
    _vertex4f Position(0, 0, 1, 0);

    glEnable(GL_LIGHT0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&Position);
    glPopMatrix();
  } else {
    glDisable(GL_LIGHT0);
  }
  // poner la segunda luz
}

/**
 *
 *@param
 *@returns
 */

void set_materials() {
  switch (Material_active) {
    case 0: {
      _vertex3f Material_diffuse(0.3, 0.3, 0.3);
      _vertex3f Material_specular(0.1, 0.1, 0.1);
      _vertex3f Material_ambient(0.05, 0.05, 0.05);
      float Material_shininess = 1;

      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&Material_diffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   (GLfloat *)&Material_specular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&Material_ambient);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Material_shininess);
    } break;
    case 1:  // segundo material
      break;
    case 2:  // tercer material
      break;
  }
}

/**
 * Limpiar ventana
 *
 *@param
 *@returns
 */

void clear_window() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

/**
 * Funcion para definir la transformación de proyeccion
 *
 *@param
 *@returns
 */

void change_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
  //  Front_plane>0  Back_plane>PlanoDelantero)
  glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, FRONT_PLANE_PERSPECTIVE,
            BACK_PLANE_PERSPECTIVE);
}

/**
 * Funcion para definir la transformación de vista (posicionar la camara)
 *
 *@param
 *@returns
 */

void change_observer() {
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -Observer_distance);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}

/**
 * Funcion que dibuja los objetos
 *
 *@param
 *@returns
 */

void draw_objects() {
  Axis.draw_line();

  if (Draw_point) {
    glPointSize(5);
    glColor3fv((GLfloat *)&BLACK);
    switch (Object) {
      case OBJECT_TETRAHEDRON:
        Tetrahedron.draw_point();
        break;
      case OBJECT_CONE:
        Cone.draw_point();
        break;
      case OBJECT_CYLINDER:
        Cylinder.draw_point();
        break;
      case OBJECT_SPHERE:
        Sphere.draw_point();
        break;
      case OBJECT_PLY:
        Object_ply.draw_point();
        break;
      case OBJECT_HIERARCHICAL:
        Level3.draw_point();
        break;
      case OBJECT_CHESS_BOARD:
        Chess_board.draw_point();
        break;
      default:
        break;
    }
  }

  if (Draw_line) {
    glLineWidth(3);
    glColor3fv((GLfloat *)&MAGENTA);
    switch (Object) {
      case OBJECT_TETRAHEDRON:
        Tetrahedron.draw_line();
        break;
      case OBJECT_CONE:
        Cone.draw_line();
        break;
      case OBJECT_CYLINDER:
        Cylinder.draw_line();
        break;
      case OBJECT_SPHERE:
        Sphere.draw_line();
        break;
      case OBJECT_PLY:
        Object_ply.draw_line();
        break;
      case OBJECT_HIERARCHICAL:
        Level3.draw_line();
        break;
      case OBJECT_CHESS_BOARD:
        Chess_board.draw_line();
        break;
      default:
        break;
    }
  }

  if (Draw_chess) {
    switch (Object) {
      case OBJECT_TETRAHEDRON:
        Tetrahedron.draw_chess();
        break;
      case OBJECT_CONE:
        Cone.draw_chess();
        break;
      case OBJECT_CYLINDER:
        Cylinder.draw_chess();
        break;
      case OBJECT_SPHERE:
        Sphere.draw_chess();
        break;
      case OBJECT_PLY:
        Object_ply.draw_chess();
        break;
      case OBJECT_HIERARCHICAL:
        Level3.draw_chess();
        break;
      case OBJECT_CHESS_BOARD:
        Chess_board.draw_chess();
        break;
      default:
        break;
    }
  }

  if (Draw_fill) {
    switch (Mode_rendering) {
      case MODE_RENDERING_SOLID: {
        _vertex4f Ambient(0.1, 0.1, 0.1, 1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        switch (Object) {
          case OBJECT_TETRAHEDRON:
            Tetrahedron.draw_illumination_flat_shading();
            break;
            //        case
            //        OBJECT_CONE:Cone.draw_illumination_flat_shading();break;
            //        case
            //        OBJECT_CYLINDER:Cylinder.draw_illumination_flat_shading();break;
          case OBJECT_SPHERE:
            Sphere.draw_illumination_flat_shading();
            break;
          case OBJECT_PLY:
            Object_ply.draw_illumination_flat_shading();
            break;
          case OBJECT_HIERARCHICAL:
            Level3.draw_illumination_flat_shading();
            break;
          case OBJECT_CHESS_BOARD:
            Chess_board.draw_illumination_flat_shading();
            break;
          default:
            break;
        }
        glDisable(GL_LIGHTING);
      } break;

      case MODE_RENDERING_ILLUMINATION_SMOOTH_SHADING: {
        _vertex4f Ambient(0.1, 0.1, 0.1, 1);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        switch (Object) {
          case OBJECT_TETRAHEDRON:
            Tetrahedron.draw_illumination_smooth_shading();
            break;
            //        case
            //        OBJECT_CONE:Cone.draw_illumination_smooth_shading();break;
            //        case
            //        OBJECT_CYLINDER:Cylinder.draw_illumination_smooth_shading();break;
          case OBJECT_SPHERE:
            Sphere.draw_illumination_smooth_shading();
            break;
          case OBJECT_PLY:
            Object_ply.draw_illumination_smooth_shading();
            break;
          case OBJECT_HIERARCHICAL:
            Level3.draw_illumination_smooth_shading();
            break;
          case OBJECT_CHESS_BOARD:
            Chess_board.draw_illumination_smooth_shading();
            break;
          default:
            break;
        }
        glDisable(GL_LIGHTING);
      } break;
      case MODE_RENDERING_TEXTURE:
        switch (Object) {
          case OBJECT_TETRAHEDRON:
            Tetrahedron.draw_texture();
            break;
            //      case OBJECT_CONE:Cone.draw_texture();break;
            //      case OBJECT_CYLINDER:Cylinder.draw_texture();break;
            //      case OBJECT_SPHERE:Sphere.draw_texture();break;
            //      case OBJECT_PLY:Object_ply.draw_texture();break;
            //      case OBJECT_HIERARCHICAL:Level3.draw_texture();break;
          case OBJECT_CHESS_BOARD:
            Chess_board.draw_texture();
            break;
          default:
            break;
        }
        break;
      case MODE_RENDERING_TEXTURE_ILLUMINATION_FLAT_SHADING: {
        _vertex4f Ambient(0.1, 0.1, 0.1, 1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        // parametros de aplicacion de la textura
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        switch (Object) {
          case OBJECT_TETRAHEDRON:
            Tetrahedron.draw_texture_illumination_flat_shading();
            break;
            //        case OBJECT_CONE:Cone.draw_texture();break;
            //        case OBJECT_CYLINDER:Cylinder.draw_texture();break;
            //        case OBJECT_SPHERE:Sphere.draw_texture();break;
            //        case OBJECT_PLY:Object_ply.draw_texture();break;
            //        case OBJECT_HIERARCHICAL:Level3.draw_texture();break;
          case OBJECT_CHESS_BOARD:
            Chess_board.draw_texture_illumination_flat_shading();
            break;
          default:
            break;
        }
        glDisable(GL_LIGHTING);
      } break;
      case MODE_RENDERING_TEXTURE_ILLUMINATION_SMOOTH_SHADING: {
        _vertex4f Ambient(0.1, 0.1, 0.1, 1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        // parametros de aplicacion de la textura
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        switch (Object) {
          case OBJECT_TETRAHEDRON:
            Tetrahedron.draw_texture_illumination_smooth_shading();
            break;
            //        case OBJECT_CONE:Cone.draw_texture();break;
            //        case OBJECT_CYLINDER:Cylinder.draw_texture();break;
            //        case OBJECT_SPHERE:Sphere.draw_texture();break;
            //        case OBJECT_PLY:Object_ply.draw_texture();break;
            //        case OBJECT_HIERARCHICAL:Level3.draw_texture();break;
          case OBJECT_CHESS_BOARD:
            Chess_board.draw_texture_illumination_smooth_shading();
            break;
          default:
            break;
        }
        glDisable(GL_LIGHTING);
      } break;
    }
  }
}

/**
 * Evento de dibujado
 *
 *@param
 *@returns
 */

void draw_scene(void) {
  clear_window();
  change_observer();
  draw_objects();
  glutSwapBuffers();
}

/**
 * Evento de cambio de tamaño de la ventana
 *@param
 *@returns
 */

void resize(int Ancho1, int Alto1) {
  // 	change_projection();
  glViewport(0, 0, Ancho1, Alto1);
  glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1, int x, int y) {
  switch (toupper(Tecla1)) {
    case '1':
      Object = OBJECT_TETRAHEDRON;
      break;
    case '2':
      Object = OBJECT_CUBE;
      break;
    case '3':
      Object = OBJECT_CONE;
      break;
    case '4':
      Object = OBJECT_CYLINDER;
      break;
    case '5':
      Object = OBJECT_SPHERE;
      break;
    case '6':
      Object = OBJECT_PLY;
      break;
    case '7':
      Object = OBJECT_HIERARCHICAL;
      break;
    case '8':
      Object = OBJECT_CHESS_BOARD;
      break;

    case 'P':
      Draw_point = !Draw_point;
      break;
    case 'L':
      Draw_line = !Draw_line;
      break;
    case 'F':
      Draw_fill = !Draw_fill;
      break;
      //      case 'C':Draw_chess=!Draw_chess;break;

    case 'J':
      Light0_active = !Light0_active;
      break;
    case 'K':
      Light1_active = !Light1_active;
      break;
    case 'M':
      Material_active = (Material_active + 1) % 3;
      break;

    case 'Q':
      exit(0);
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_keys(int Tecla1, int x, int y) {
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
      Mode_rendering = MODE_RENDERING_SOLID;
      break;
    case GLUT_KEY_F2:
      Mode_rendering = MODE_RENDERING_SOLID_CHESS;
      break;
    case GLUT_KEY_F3:
      Mode_rendering = MODE_RENDERING_ILLUMINATION_FLAT_SHADING;
      break;
    case GLUT_KEY_F4:
      Mode_rendering = MODE_RENDERING_ILLUMINATION_SMOOTH_SHADING;
      break;
    case GLUT_KEY_F5:
      Mode_rendering = MODE_RENDERING_TEXTURE;
      break;
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void) {
  Cone.create(40);
  Cylinder.create(40);
  Sphere.create(40, 40);
  Level3.create();
  Chess_board.create(1, 10);
  // load and create ply object
  Level3.create();

  // se indica cual sera el color para limpiar la ventana	(r,v,a,al)
  glClearColor(1, 1, 1, 1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  //
  change_projection();
  //
  glViewport(0, 0, UI_window_width, UI_window_height);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv) {
  // se llama a la inicialización de glut
  glutInit(&argc, argv);

  // se indica las caracteristicas que se desean para la visualización con
  // OpenGL Las posibilidades son: GLUT_SIMPLE -> memoria de imagen simple
  // GLUT_DOUBLE -> memoria de imagen doble
  // GLUT_INDEX -> memoria de imagen con color indizado
  // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada
  // pixel GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y
  // alfa para cada pixel GLUT_DEPTH -> memoria de profundidad o z-bufer
  // GLUT_STENCIL -> memoria de estarcido
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  // posicion de la esquina inferior izquierdad de la ventana
  glutInitWindowPosition(UI_window_pos_x, UI_window_pos_y);

  // tamaño de la ventana (ancho y alto)
  glutInitWindowSize(UI_window_width, UI_window_height);

  // llamada para crear la ventana, indicando el titulo (no se visualiza hasta
  // que se llama al bucle de eventos)
  glutCreateWindow("Practica 1");

  // asignación de la funcion llamada "dibujar" al evento de dibujo
  glutDisplayFunc(draw_scene);
  // asignación de la funcion llamada "cambiar_tamanio_ventana" al evento
  // correspondiente
  glutReshapeFunc(resize);
  // asignación de la funcion llamada "tecla_normal" al evento correspondiente
  glutKeyboardFunc(normal_keys);
  // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
  glutSpecialFunc(special_keys);

  // funcion de inicialización
  initialize();

  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
