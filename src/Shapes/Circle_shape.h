#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "../gl_canvas2d.h"

class Circle_shape : public Shape
{
private:
   /* data */
public:
   Circle_shape(float x, float y, float radius, int div)
   {
      vx = new float[div];
      vy = new float[div];

      type = CIRCULO;
      proportion.set(1, 1);

      draw = new Vector2[div]();

      width_box = 2.0 * radius;
      height_box = 2.0 * radius;
      elems = div;

      midle_x = x;
      up_y = 10 + y + radius;

      float ang = 0;
      float inc = PI_2 / div;
      for (int lado = 0; lado < div; lado++)
      {
         vx[lado] = (cos(ang) * radius) + x;
         vy[lado] = (sin(ang) * radius) + y;
         ang += inc;
      }
      update_x[0] = x - radius;
      update_y[0] = y - radius;
      update_x[1] = x + radius;
      update_y[1] = y - radius;
      update_x[2] = x + radius;
      update_y[2] = y + radius;
      update_x[3] = x - radius;
      update_y[3] = y + radius;

      for (int i = 0; i < elems; i++)
      {
         draw[i].set(vx[i] - update_x[0], vy[i] - update_y[0]);
      }
      for (int i = 0; i < 4; i++)
      {
         update_base[i].set(update_x[i] - update_x[0], update_y[i] - update_y[0]);
      }
   }

   ~Circle_shape()
   {
      delete vx;
      delete vy;
      delete draw;
   }
};

#endif
