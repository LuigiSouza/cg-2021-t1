
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
      draw_x = new float[elems];
      draw_y = new float[elems];

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
         draw_x[i] = vx[i] = x[i];
         draw_y[i] = vy[i] = y[i];
      }
      midle_x = (minx + maxx) / 2;
      up_y = (miny + maxy) / 2;
      width_box = (maxx - minx);
      height_box = (maxy - miny);

      update_x_base[0] = update_x[0] = minx;
      update_y_base[0] = update_y[0] = miny;
      update_x_base[1] = update_x[1] = maxx;
      update_y_base[1] = update_y[1] = miny;
      update_x_base[2] = update_x[2] = maxx;
      update_y_base[2] = update_y[2] = maxy;
      update_x_base[3] = update_x[3] = minx;
      update_y_base[3] = update_y[3] = maxy;

      for (int i = 0; i < elems; i++)
      {
         draw_x[i] -= vx[0];
         draw_y[i] -= vy[0];
      }
      for (int i = 0; i < 4; i++)
      {
         update_x_base[i] = update_x_base[i] - vx[0];
         update_y_base[i] = update_y_base[i] - vy[0];
      }
   }
};

#endif
