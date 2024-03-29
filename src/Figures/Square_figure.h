#ifndef __Square_figure_H__
#define __Square_figure_H__

#include "Figure.h"

class Square_figure : public Figure
{
public:
   Square_figure(float x, float y, float width, float height)
   {
      vx = new float[4];
      vy = new float[4];

      type = QUADRADO;
      proportion.set(1, 1);

      draw = new Vector2[4]();

      width_box = width;
      height_box = height;
      elems = 4;

      midle_x = (2 * x + width) / 2;
      up_y = 10 + y + height;

      vx[0] = update_x[0] = x;
      vy[0] = update_y[0] = y;
      vx[1] = update_x[1] = x + width;
      vy[1] = update_y[1] = y;
      vx[2] = update_x[2] = x + width;
      vy[2] = update_y[2] = y + width;
      vx[3] = update_x[3] = x;
      vy[3] = update_y[3] = y + height;

      for (int i = 0; i < elems; i++)
      {
         draw[i].set(vx[i] - vx[0], vy[i] - vy[0]);
         update_base[i].set(draw[i]);
      }
   }
};

#endif
