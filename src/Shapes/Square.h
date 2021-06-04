
#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "Shape.h"
#include "../gl_canvas2d.h"
#include "../Handles/Point.h"

#include <iostream>

class Square : public Shape
{
private:
   float angle = 0.2;
public:
   void render(void)
   {
      CV::color(this->r, this->g, this->b);
      if (fill_flag)
         CV::polygonFill(vx, vy, elems);
      else
         CV::polygon(vx, vy, elems);
      CV::color(1, 1, 1);
   }
   void update(Mouse mouse)
   {
      float max_x = vx[0];
      float min_x = vx[0];
      float max_y = vy[0];

      for (int i = 0; i < elems; i++)
      {
         vx[i] = vx[i] + mouse.moveX();
         vy[i] = vy[i] + mouse.moveY();

         max_x = max_x < vx[i] ? vx[i] : max_x;
         min_x = min_x > vx[i] ? vx[i] : min_x;
         max_y = max_y < vy[i] ? vy[i] : max_y;
      }
      up_y = max_y;
      midle_x = (max_x + min_x) / 2;
   }
   void fill(void)
   {
      this->fill_flag = !fill_flag;
   }

   bool isInside(float x, float y)
   {
      Point p;
      return p.isInside(x, y, elems, vx, vy);
   }

   void rotate(void)
   {
      float realative_x[4];
      float realative_y[4];

      float base_x = vx[0];
      float base_y = vy[0];

      for (int i = 0; i < 4; i++)
      {
         realative_x[i] = vx[i] - base_x;
         realative_y[i] = vy[i] - base_y;
         float x = realative_x[i];

         realative_x[i] = x * cos(angle) - realative_y[i] * sin(angle);
         realative_y[i] = x * sin(angle) + realative_y[i] * cos(angle);

         vx[i] = realative_x[i] + base_x;
         vy[i] = realative_y[i] + base_y;
      }
   }

   Square(float x, float y, float width, float height)
   {
      vx = new float[4];
      vy = new float[4];
      elems = 4;

      midle_x = (2 * x + width) / 2;
      up_y = y + height;

      vx[0] = x;
      vy[0] = y;
      vx[1] = x + width;
      vy[1] = y;
      vx[2] = x + width;
      vy[2] = y + width;
      vx[3] = x;
      vy[3] = y + height;
   }
};

#endif
