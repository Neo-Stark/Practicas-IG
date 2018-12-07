/*
* Prácticas de IG
* Copyright Domingo Martín Perandrés 2014-218
* dmartin@ugr.es
*
* GPL 3
*/

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "object3d.h"

class _chess_board:public _object3D
{
public:
  void create(float Size=1.0, unsigned int Divisions1=1);

protected:
  unsigned int Divisions;
};

#endif
