
#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "Shape.h"
#include "../gl_canvas2d.h"
#include "../Handles/Point.h"

#include <iostream>
#include <cmath>

class Polygon_shape : public Shape
{
public:
   Polygon_shape(float x[], float y[], int elems)
   {
      vx = new float[elems];
      vy = new float[elems];

      draw = new Vector2[elems]();

      float minx = 100000;
      float miny = 100000;
      float maxx = -1;
      float maxy = -1;
      this->elems = elems;

      for (int i = 0; i < elems; i++)
      {
         minx = x[i] < minx ? x[i] : minx;
         miny = y[i] < miny ? y[i] : miny;
         maxx = x[i] > maxx ? x[i] : maxx;
         maxy = y[i] > maxy ? y[i] : maxy;
         vx[i] = x[i];
         vy[i] = y[i];
      }
      midle_x = (minx + maxx) / 2;
      up_y = 10 + maxy;
      width_box = (maxx - minx);
      height_box = (maxy - miny);

      update_x[0] = minx;
      update_y[0] = miny;
      update_x[1] = maxx;
      update_y[1] = miny;
      update_x[2] = maxx;
      update_y[2] = maxy;
      update_x[3] = minx;
      update_y[3] = maxy;

      for (int i = 0; i < elems; i++)
      {
         draw[i].set(vx[i] - vx[0], vy[i] - vy[0]);
      }
      for (int i = 0; i < 4; i++)
      {
         update_base[i].set(update_x[i] - vx[0], update_y[i] - vy[0]);
      }
   }
};

#endif
