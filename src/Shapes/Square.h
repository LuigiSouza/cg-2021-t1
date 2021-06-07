
#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "Shape.h"
#include "../gl_canvas2d.h"
#include "../Handles/Point.h"

#include <iostream>
#include <cmath>

class Square : public Shape
{
private:
   bool updateShape = false;
   int resize_pos;

public:
   void resize_shape(Mouse mouse)
   {
      float realative_x;
      float realative_y;

      float proportion_x, proportion_y;

      float cos_angle = fabs(cos(angle));
      float sin_angle = fabs(sin(angle));
      if (resize_pos & 1)
      {
         update_x[(resize_pos + 1 + 4) % 4] += mouse.moveX() * cos_angle;
         update_y[(resize_pos + 1 + 4) % 4] += mouse.moveY() * sin_angle;
         update_x[(resize_pos - 1 + 4) % 4] += mouse.moveX() * sin_angle;
         update_y[(resize_pos - 1 + 4) % 4] += mouse.moveY() * cos_angle;
      }
      else
      {
         update_x[(resize_pos + 1 + 4) % 4] += mouse.moveX() * sin_angle;
         update_y[(resize_pos + 1 + 4) % 4] += mouse.moveY() * cos_angle;
         update_x[(resize_pos - 1 + 4) % 4] += mouse.moveX() * cos_angle;
         update_y[(resize_pos - 1 + 4) % 4] += mouse.moveY() * sin_angle;
      }
      update_x[resize_pos] += mouse.moveX();
      update_y[resize_pos] += mouse.moveY();

      float base_x = update_x[0];
      float base_y = update_y[0];

      proportion_x = (Point::distance(update_x[1], update_y[1], update_x[0], update_y[0])) / width_box * 1.0;
      proportion_y = (Point::distance(update_x[1], update_y[1], update_x[2], update_y[2])) / height_box * 1.0;
      // proportion_x = (update_x[1] - update_x[0]) / width_box * 1.0;
      // proportion_y = (update_y[2] - update_y[1]) / height_box * 1.0;

      std::cout << " propo: " << proportion_x << " " << proportion_y << std::endl;

      for (int i = 0; i < elems; i++)
      {
         realative_x = draw_x[i] * proportion_x * 1.0;
         realative_y = draw_y[i] * proportion_y * 1.0;

         vx[i] = realative_x + base_x;
         vy[i] = realative_y + base_y;
      }

      midle_x += mouse.moveX() / 2 * 1.0;
      up_y = update_y[0] > update_y[2] ? update_y[2] - 10 : update_y[2] + 10;
   }

   void update_shape(Mouse mouse)
   {
      if (resize_pos == -1)
      {
         rotate_shape(mouse);
      }
      else
      {
         resize_shape(mouse);
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
         update_shape(mouse);
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

   void rotate_shape(Mouse mouse)
   {
      float realative_x;
      float realative_y;

      float base_x = vx[0];
      float base_y = vy[0];

      float vecA_x = midle_x - base_x;
      float vecA_y = up_y - base_y;
      float mouseVec_x = mouse.getX() - base_x;
      float mouseVec_y = mouse.getY() - base_y;

      float dot_product = vecA_x * mouseVec_x + vecA_y * mouseVec_y * 1.0;
      float croos_product = vecA_x * mouseVec_y - vecA_y * mouseVec_x * 1.0;

      // float angle = acos(dot_product / div);
      float angle = atan2(fabs(croos_product), dot_product);
      angle = croos_product < 0 ? angle *= -1.0 : angle;
      float x = vecA_x;

      vecA_x = x * cos(angle) - vecA_y * sin(angle);
      vecA_y = x * sin(angle) + vecA_y * cos(angle);

      midle_x = vecA_x + base_x;
      up_y = vecA_y + base_y;

      // Translate shpae points
      for (int i = 0; i < elems; i++)
      {
         realative_x = vx[i] - base_x;
         realative_y = vy[i] - base_y;
         x = realative_x;

         realative_x = x * cos(angle) - realative_y * sin(angle);
         realative_y = x * sin(angle) + realative_y * cos(angle);

         vx[i] = realative_x + base_x;
         vy[i] = realative_y + base_y;

         realative_x = draw_x[i];
         realative_y = draw_y[i];

         draw_x[i] = realative_x * cos(angle) - realative_y * sin(angle);
         draw_y[i] = realative_x * sin(angle) + realative_y * cos(angle);
      }
      // Translate box points
      for (int i = 0; i < 4; i++)
      {
         realative_x = update_x[i] - base_x;
         realative_y = update_y[i] - base_y;
         x = realative_x;

         realative_x = x * cos(angle) - realative_y * sin(angle);
         realative_y = x * sin(angle) + realative_y * cos(angle);

         update_x[i] = realative_x + base_x;
         update_y[i] = realative_y + base_y;
      }
      this->angle += angle;
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
