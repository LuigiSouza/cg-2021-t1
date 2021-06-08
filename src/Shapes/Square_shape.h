
#ifndef __Square_shape_H__
#define __Square_shape_H__

#include "Shape.h"
#include "../gl_canvas2d.h"
#include "../Handles/Point.h"

#include <iostream>
#include <cmath>

class Square_shape : public Shape
{
private:
public:
   Square_shape(float x, float y, float width, float height)
   {
      vx = new float[4];
      vy = new float[4];
      draw_x = new float[4];
      draw_y = new float[4];
      width_box = width;
      height_box = height;
      elems = 4;

      midle_x = (2 * x + width) / 2;
      up_y = 10 + y + height;

      update_x_base[0] = draw_x[0] = vx[0] = update_x[0] = x;
      update_y_base[0] = draw_y[0] = vy[0] = update_y[0] = y;
      update_x_base[1] = draw_x[1] = vx[1] = update_x[1] = x + width;
      update_y_base[1] = draw_y[1] = vy[1] = update_y[1] = y;
      update_x_base[2] = draw_x[2] = vx[2] = update_x[2] = x + width;
      update_y_base[2] = draw_y[2] = vy[2] = update_y[2] = y + width;
      update_x_base[3] = draw_x[3] = vx[3] = update_x[3] = x;
      update_y_base[3] = draw_y[3] = vy[3] = update_y[3] = y + height;

      for (int i = 0; i < elems; i++)
      {
         draw_x[i] -= vx[0];
         draw_y[i] -= vy[0];
         update_x_base[i] = update_x_base[i] - vx[0];
         update_y_base[i] = update_y_base[i] - vy[0];
      }
   }
};

#endif
