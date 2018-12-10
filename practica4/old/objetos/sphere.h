/*
* Prácticas de IG
* Copyright Domingo Martín Perandrés 2014-218
* dmartin@ugr.es
*
* GPL 3
*/

#ifndef SPHERE_H
#define SPHERE_H

#include "object3d_sweep_basic.h"

class _sphere:public _object3d_sweep_basic
{
public:
  void create(unsigned int Num_horizontal_divisions1=3,unsigned int Num_vertical_divisions1=3, float Radius1=0.5);
  void compute_triangles_normals();
  void compute_vertices_normals();
};

#endif
