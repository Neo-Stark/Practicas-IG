/*
* Prácticas de IG
* Copyright Domingo Martín Perandrés 2014-218
* dmartin@ugr.es
*
* GPL 3
*/

#include "glwidget.h"
#include "window.h"

using namespace std;
using namespace _gl_widget_ne;
using namespace _colors_ne;

/**
* Constructor
*
*@param
*@returns
*/

_gl_widget::_gl_widget(_window *Window1):Window(Window1)
{
  setMinimumSize(300, 300);
  setFocusPolicy(Qt::StrongFocus);
}

/**
* Evento tecla pulsada
*
*@param
*@returns
*/


void _gl_widget::keyPressEvent(QKeyEvent *Keyevent)
{
  switch(Keyevent->key()){
  case Qt::Key_1:Object=OBJECT_TETRAHEDRON;break;
  case Qt::Key_2:Object=OBJECT_CUBE;break;
  case Qt::Key_3:Object=OBJECT_CONE;break;
  case Qt::Key_4:Object=OBJECT_CYLINDER;break;
  case Qt::Key_5:Object=OBJECT_SPHERE;break;
  case Qt::Key_6:Object=OBJECT_PLY;break;
  case Qt::Key_7:Object=OBJECT_HIERARCHICAL;break;
  case Qt::Key_8:Object=OBJECT_CHESS_BOARD;break;

  case Qt::Key_P:Draw_point=!Draw_point;break;
  case Qt::Key_L:Draw_line=!Draw_line;break;
  case Qt::Key_F:Draw_fill=!Draw_fill;break;

  case Qt::Key_F1:Mode_rendering=MODE_RENDERING_SOLID;break;
  case Qt::Key_F2:Mode_rendering=MODE_RENDERING_SOLID_CHESS;break;
  case Qt::Key_F3:Mode_rendering=MODE_RENDERING_ILLUMINATION_FLAT_SHADING;break;
  case Qt::Key_F4:Mode_rendering=MODE_RENDERING_ILLUMINATION_SMOOTH_SHADING;break;
  case Qt::Key_F5:Mode_rendering=MODE_RENDERING_TEXTURE;break;

  case Qt::Key_A:
    Animation=!Animation;
    set_animation();
    break;

  case Qt::Key_J:Light0_active=!Light0_active;break;
  case Qt::Key_K:Light1_active=!Light1_active;break;

  case Qt::Key_M:Material_active=(Material_active+1)%3;break;

  case Qt::Key_Left:Observer_angle_y-=ANGLE_STEP;break;
  case Qt::Key_Right:Observer_angle_y+=ANGLE_STEP;break;
  case Qt::Key_Up:Observer_angle_x-=ANGLE_STEP;break;
  case Qt::Key_Down:Observer_angle_x+=ANGLE_STEP;break;
  case Qt::Key_PageUp:Observer_distance*=1.2;break;
  case Qt::Key_PageDown:Observer_distance/=1.2;break;

  case Qt::Key_Q:break;
  case Qt::Key_W:break;
  case Qt::Key_S:break;
  case Qt::Key_D:break;
  case Qt::Key_Z:break;
  case Qt::Key_X:break;

  case Qt::Key_E:break;
  case Qt::Key_R:break;
  case Qt::Key_T:break;
  case Qt::Key_Y:break;
  case Qt::Key_U:break;
  case Qt::Key_I:break;
  }

  update();
}

/**
* Limpiar ventana
*
*@param
*@returns
*/

void _gl_widget::clear_window()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


/**
* Funcion para definir la transformación de proyeccion
*
*@param
*@returns
*/


void _gl_widget::change_projection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
  //  Front_plane>0  Back_plane>PlanoDelantero)
  glFrustum(X_MIN,X_MAX,Y_MIN,Y_MAX,FRONT_PLANE_PERSPECTIVE,BACK_PLANE_PERSPECTIVE);
}


/**
* Funcion para definir la transformación de vista (posicionar la camara)
*
*@param
*@returns
*/

void _gl_widget::change_observer()
{
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-Observer_distance);
  glRotatef(Observer_angle_x,1,0,0);
  glRotatef(Observer_angle_y,0,1,0);
}


/**
* Funcion que dibuja los objetos
*
*@param
*@returns
*/

void _gl_widget::draw_objects()
{
  Axis.draw_line();

  if (Draw_point){
    glPointSize(5);
    glColor3fv((GLfloat *) &BLACK);
    switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_point();break;
    case OBJECT_CONE:Cone.draw_point();break;
    case OBJECT_CYLINDER:Cylinder.draw_point();break;
    case OBJECT_SPHERE:Sphere.draw_point();break;
    case OBJECT_PLY:Object_ply.draw_point();break;
    case OBJECT_HIERARCHICAL:Level3.draw_point();break;
    case OBJECT_CHESS_BOARD:Chess_board.draw_point();break;
    default:break;
    }
  }

  if (Draw_line){
    glLineWidth(1);
    glColor3fv((GLfloat *) &MAGENTA);
    switch (Object){
    case OBJECT_TETRAHEDRON:Tetrahedron.draw_line();break;
    case OBJECT_CONE:Cone.draw_line();break;
    case OBJECT_CYLINDER:Cylinder.draw_line();break;
    case OBJECT_SPHERE:Sphere.draw_line();break;
    case OBJECT_PLY:Object_ply.draw_line();break;
    case OBJECT_HIERARCHICAL:Level3.draw_line();break;
    case OBJECT_CHESS_BOARD:Chess_board.draw_line();break;
    default:break;
    }
  }

  if (Draw_fill){
    switch (Mode_rendering){
    case MODE_RENDERING_SOLID:
      glColor3fv((GLfloat *) &BLUE);
      switch (Object){
      case OBJECT_TETRAHEDRON:Tetrahedron.draw_fill();break;
      case OBJECT_CONE:Cone.draw_fill();break;
      case OBJECT_CYLINDER:Cylinder.draw_fill();break;
      case OBJECT_SPHERE:Sphere.draw_fill();break;
      case OBJECT_PLY:Object_ply.draw_fill();break;
      case OBJECT_HIERARCHICAL:Level3.draw_fill();break;
      case OBJECT_CHESS_BOARD:Chess_board.draw_fill();break;
      default:break;
      }
      break;
    case MODE_RENDERING_SOLID_CHESS:
      switch (Object){
      case OBJECT_TETRAHEDRON:Tetrahedron.draw_chess();break;
      case OBJECT_CONE:Cone.draw_chess();break;
      case OBJECT_CYLINDER:Cylinder.draw_chess();break;
      case OBJECT_SPHERE:Sphere.draw_chess();break;
      case OBJECT_PLY:Object_ply.draw_chess();break;
      case OBJECT_HIERARCHICAL:Level3.draw_chess();break;
      case OBJECT_CHESS_BOARD:Chess_board.draw_chess();break;
      default:break;
      }
      break;
    case MODE_RENDERING_ILLUMINATION_FLAT_SHADING:
      {
      _vertex4f Ambient(0.1,0.1,0.1,1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        switch (Object){
        case OBJECT_TETRAHEDRON:Tetrahedron.draw_illumination_flat_shading();break;
//        case OBJECT_CONE:Cone.draw_illumination_flat_shading();break;
//        case OBJECT_CYLINDER:Cylinder.draw_illumination_flat_shading();break;
        case OBJECT_SPHERE:Sphere.draw_illumination_flat_shading();break;
        case OBJECT_PLY:Object_ply.draw_illumination_flat_shading();break;
        case OBJECT_HIERARCHICAL:Level3.draw_illumination_flat_shading();break;
        case OBJECT_CHESS_BOARD:Chess_board.draw_illumination_flat_shading();break;
        default:break;
        }
        glDisable(GL_LIGHTING);
      }
      break;

    case MODE_RENDERING_ILLUMINATION_SMOOTH_SHADING:
      {
        _vertex4f Ambient(0.1,0.1,0.1,1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        switch (Object){
        case OBJECT_TETRAHEDRON:Tetrahedron.draw_illumination_smooth_shading();break;
//        case OBJECT_CONE:Cone.draw_illumination_smooth_shading();break;
//        case OBJECT_CYLINDER:Cylinder.draw_illumination_smooth_shading();break;
        case OBJECT_SPHERE:Sphere.draw_illumination_smooth_shading();break;
        case OBJECT_PLY:Object_ply.draw_illumination_smooth_shading();break;
        case OBJECT_HIERARCHICAL:Level3.draw_illumination_smooth_shading();break;
        case OBJECT_CHESS_BOARD:Chess_board.draw_illumination_smooth_shading();break;
        default:break;
        }
        glDisable(GL_LIGHTING);
      }
      break;
    case MODE_RENDERING_TEXTURE:
      switch (Object){
      case OBJECT_TETRAHEDRON:Tetrahedron.draw_texture();break;
//      case OBJECT_CONE:Cone.draw_texture();break;
//      case OBJECT_CYLINDER:Cylinder.draw_texture();break;
//      case OBJECT_SPHERE:Sphere.draw_texture();break;
//      case OBJECT_PLY:Object_ply.draw_texture();break;
//      case OBJECT_HIERARCHICAL:Level3.draw_texture();break;
      case OBJECT_CHESS_BOARD:Chess_board.draw_texture();break;
      default:break;
      }
      break;
    case MODE_RENDERING_TEXTURE_ILLUMINATION_FLAT_SHADING:
      {
        _vertex4f Ambient(0.1,0.1,0.1,1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        // parametros de aplicacion de la textura
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        switch (Object){
        case OBJECT_TETRAHEDRON:Tetrahedron.draw_texture_illumination_flat_shading();break;
//        case OBJECT_CONE:Cone.draw_texture();break;
//        case OBJECT_CYLINDER:Cylinder.draw_texture();break;
//        case OBJECT_SPHERE:Sphere.draw_texture();break;
//        case OBJECT_PLY:Object_ply.draw_texture();break;
//        case OBJECT_HIERARCHICAL:Level3.draw_texture();break;
        case OBJECT_CHESS_BOARD:Chess_board.draw_texture_illumination_flat_shading();break;
        default:break;
        }
        glDisable(GL_LIGHTING);
      }
      break;
    case MODE_RENDERING_TEXTURE_ILLUMINATION_SMOOTH_SHADING:
      {
        _vertex4f Ambient(0.1,0.1,0.1,1);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(GLfloat *)&Ambient);

        set_lights();
        set_materials();

        glEnable(GL_LIGHTING);
        // parametros de aplicacion de la textura
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        switch (Object){
        case OBJECT_TETRAHEDRON:Tetrahedron.draw_texture_illumination_smooth_shading();break;
//        case OBJECT_CONE:Cone.draw_texture();break;
//        case OBJECT_CYLINDER:Cylinder.draw_texture();break;
//        case OBJECT_SPHERE:Sphere.draw_texture();break;
//        case OBJECT_PLY:Object_ply.draw_texture();break;
//        case OBJECT_HIERARCHICAL:Level3.draw_texture();break;
        case OBJECT_CHESS_BOARD:Chess_board.draw_texture_illumination_smooth_shading();break;
        default:break;
        }
        glDisable(GL_LIGHTING);
      }
      break;
    }
  }
}


/**
* Evento de dibujado
*
*@param
*@returns
*/


void _gl_widget::paintGL()
{
  clear_window();
  change_projection();
  change_observer();
  draw_objects();
}


/**
* Evento de cambio de tamaño de la ventana
*@param
*@returns
*/

void _gl_widget::resizeGL(int Width1, int Height1)
{
  glViewport(0,0,Width1,Height1);
}


/**
* Inicialización de OpenGL
*@param
*@returns
*/

void _gl_widget::initializeGL()
{
  const GLubyte* strm;

  strm = glGetString(GL_VENDOR);
  std::cerr << "Vendor: " << strm << "\n";
  strm = glGetString(GL_RENDERER);
  std::cerr << "Renderer: " << strm << "\n";
  strm = glGetString(GL_VERSION);
  std::cerr << "OpenGL Version: " << strm << "\n";

  if (strm[0] == '1'){
    std::cerr << "Only OpenGL 1.X supported!\n";
    exit(-1);
  }

  strm = glGetString(GL_SHADING_LANGUAGE_VERSION);
  std::cerr << "GLSL Version: " << strm << "\n";

  int Max_texture_size=0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &Max_texture_size);
  std::cerr << "Max texture size: " << Max_texture_size << "\n";

  glClearColor(1.0,1.0,1.0,1.0);
  glEnable(GL_DEPTH_TEST);;

  Observer_angle_x=0;
  Observer_angle_y=0;
  Observer_distance=DEFAULT_DISTANCE;

  Draw_point=false;
  Draw_line=true;
  Draw_fill=false;
  Draw_chess=false;

  Animation=false;

  Cone.create(40);
  Cylinder.create(40);
  Sphere.create(40,40);
  Level3.create();
  Chess_board.create(1,10);


  // load and create ply object

  Level3.create();


  Timer = new QTimer(this);
  Timer->setInterval(0);
  connect(Timer, SIGNAL(timeout()), this, SLOT(tick()));

  // lectura de la textura
  QString File_name("images/bricks.jpg");
  QImage Image;
  QImageReader Reader(File_name);
  Reader.setAutoTransform(true);
  Image = Reader.read();
  if (Image.isNull()) {
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot load %1.").arg(QDir::toNativeSeparators(File_name)));
    exit(-1);
  }
  Image=Image.mirrored();
  Image=Image.convertToFormat(QImage::Format_RGB888);

  // creacion e inicialización de la textura
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // parametros de aplicacion de la textura
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  // asignacion de la textura
  glTexImage2D(GL_TEXTURE_2D,0,3,Image.width(),Image.height(),0,GL_RGB,GL_UNSIGNED_BYTE,Image.bits());


  Object=OBJECT_TETRAHEDRON;
  Mode_rendering=MODE_RENDERING_SOLID;
  Material_active=0;
  Light0_active=true;
  Light1_active=false;
  Light1_angle=0;

  _vertex4f Color(1,0,1,1);

  // set color por LIGHT1


  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
}

/**
*
*@param
*@returns
*/

void _gl_widget::set_animation()
{
  if (Animation) Timer->start();
  else Timer->stop();
}

/**
*
*@param
*@returns
*/

void _gl_widget::tick()
{
  cout << "Tic" << endl;

  update();
}

/**
*
*@param
*@returns
*/

void _gl_widget::set_lights()
{
  if (Light0_active){
    _vertex4f Position(0,0,1,0);

    glEnable(GL_LIGHT0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0,GL_POSITION,(GLfloat *)&Position);
    glPopMatrix();
  }
  else{
    glDisable(GL_LIGHT0);
  }
  // poner la segunda luz
}


/**
*
*@param
*@returns
*/

void _gl_widget::set_materials()
{
  switch (Material_active){
  case 0:{
    _vertex3f Material_diffuse(0.3,0.3,0.3);
    _vertex3f Material_specular(0.1,0.1,0.1);
    _vertex3f Material_ambient(0.05,0.05,0.05);
    float Material_shininess=1;

    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(GLfloat *)&Material_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *)&Material_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(GLfloat *)&Material_ambient);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material_shininess);
    }
    break;
  case 1: // segundo material
    break;
  case 2: // tercer material
    break;
  }
}

