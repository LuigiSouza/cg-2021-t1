
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
   bool updateShape = false;
   int resize_pos;

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

   void resize_shape(Mouse mouse)
   {
      if (resize_pos == -1)
      {
         std::cout << "girar" << std::endl;
      }
      else
      {
         float realative_x;
         float realative_y;

         float proportion_x, proportion_y;
         if (resize_pos & 1)
         {
            update_x[(resize_pos + 1 + 4) % 4] += mouse.moveX();
            update_y[(resize_pos - 1 + 4) % 4] += mouse.moveY();
         }
         else
         {
            update_x[(resize_pos - 1 + 4) % 4] += mouse.moveX();
            update_y[(resize_pos + 1 + 4) % 4] += mouse.moveY();
         }
         update_x[resize_pos] += mouse.moveX();
         update_y[resize_pos] += mouse.moveY();

         float base_x = update_x[0];
         float base_y = update_y[0];

         proportion_x = (update_x[1] - update_x[0]) / width_box * 1.0;
         proportion_y = (update_y[2] - update_y[1]) / height_box * 1.0;

         std::cout << std::endl
                   << "proporcao " << proportion_x << " " << proportion_y
                   << " " << mouse.moveX();

         for (int i = 0; i < 4; i++)
         {
            realative_x = draw_x[i] * proportion_x * 1.0;
            realative_y = draw_y[i] * proportion_y * 1.0;

            vx[i] = realative_x + base_x;
            vy[i] = realative_y + base_y;
         }

         midle_x += mouse.moveX() / 2 * 1.0;
         up_y = update_y[0] > update_y[2] ? update_y[0] + 10 : update_y[2] + 10;
      }
   }

   void move_shape(Mouse mouse)
   {
      for (int i = 0; i < elems; i++)
      {
         vx[i] += mouse.moveX();
         vy[i] += mouse.moveY();
         update_x[i] += mouse.moveX();
         update_y[i] += mouse.moveY();
      }
      up_y += mouse.moveY();
      midle_x += mouse.moveX();
   }

   void update(Mouse mouse)
   {
      if (updateShape)
      {
         resize_shape(mouse);
      }
      else
      {
         move_shape(mouse);
      }
   }

   void fill(void)
   {
      this->fill_flag = !fill_flag;
   }

   bool isInside(float x, float y)
   {
      return Point::isInside(x, y, elems, vx, vy);
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
      draw_x = new float[4];
      draw_y = new float[4];
      width_box = width;
      height_box = height;
      elems = 4;

      midle_x = (2 * x + width) / 2;
      up_y = 10 + y + height;

      draw_x[0] = vx[0] = update_x[0] = x;
      draw_y[0] = vy[0] = update_y[0] = y;
      draw_x[1] = vx[1] = update_x[1] = x + width;
      draw_y[1] = vy[1] = update_y[1] = y;
      draw_x[2] = vx[2] = update_x[2] = x + width;
      draw_y[2] = vy[2] = update_y[2] = y + width;
      draw_x[3] = vx[3] = update_x[3] = x;
      draw_y[3] = vy[3] = update_y[3] = y + height;

      for (int i = 0; i < elems; i++)
      {
         draw_x[i] -= vx[0];
         draw_y[i] -= vy[0];
      }
   }

   bool checkUpdateShape(Mouse mouse)
   {
      for (int i = 0; i < 4; i++)
      {
         if (Point::distance(mouse.getX(), mouse.getY(), update_x[i], update_y[i]) < RADIUS_BALL)
         {
            std::cout << " entrou no " << update_x[i] << " " << update_y[i] << std::endl;
            resize_pos = i;
            updateShape = true;
            return true;
         }
      }
      if (Point::distance(mouse.getX(), mouse.getY(), midle_x, up_y) < RADIUS_BALL)
      {
         std::cout << " entrou no topo " << std::endl;
         resize_pos = -1;
         updateShape = true;
         return true;
      }
      return false;
   }

   void releaseMouse(void)
   {
      updateShape = false;
   }
};

#endif
