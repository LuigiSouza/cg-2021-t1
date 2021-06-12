
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Shape.h"

class Triangle_shape : public Shape
{
public:
   Triangle_shape(float x1, float y1, float width, float height)
   {
      vx = new float[3];
      vy = new float[3];

      draw = new Vector2[3]();

      type = TRIANGULO;
      proportion.set(1, 1);

      this->elems = 3;

      vx[0] = x1;
      vy[0] = y1;
      vx[1] = x1 + width;
      vy[1] = y1;
      vx[2] = x1 + (width / 2.0);
      vy[2] = y1 + height;

      midle_x = vx[2];
      up_y = 10 + vy[2];
      width_box = width;
      height_box = height;

      update_x[0] = vx[0];
      update_y[0] = vy[0];
      update_x[1] = vx[1];
      update_y[1] = vy[0];
      update_x[2] = vx[1];
      update_y[2] = vy[2];
      update_x[3] = vx[0];
      update_y[3] = vy[2];

      for (int i = 0; i < 3; i++)
      {
         draw[i].set(vx[i] - vx[0], vy[i] - vy[0]);
      }
      for (int i = 0; i < 4; i++)
      {
         update_base[i].set(update_x[i] - vx[0], update_y[i] - vy[0]);
      }
   }

   ~Triangle_shape()
   {
      delete vx;
      delete vy;
      delete draw;
   }
};

#endif
