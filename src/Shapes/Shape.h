
#ifndef __SHAPE_H__
#define __SHAPE_H__

#define RADIUS_BALL 5
#define MIN_EDGE 5

#include "../gl_canvas2d.h"
#include "../Handles/Point.h"
class Shape
{
public:
   virtual void render(void)
   {
      CV::color(this->r, this->g, this->b);
      if (fill_flag)
         CV::polygonFill(vx, vy, elems);
      else
         CV::polygon(vx, vy, elems);
      CV::color(1, 1, 1);
   }
   virtual void resize_shape(Mouse mouse)
   {
      float realative_x;
      float realative_y;

      float proportion_x, proportion_y;

      float cos_angle = cos(angle);
      float sin_angle = sin(angle);
      std::cout << " angulo " << cos_angle << " " << sin_angle << std::endl;
      if (resize_pos & 1)
      {
         update_x[(resize_pos + 1 + 4) % 4] += mouse.moveX();
         update_y[(resize_pos + 1 + 4) % 4] += mouse.moveY();
         // update_x[(resize_pos - 1 + 4) % 4] += mouse.moveX();
         // update_y[(resize_pos - 1 + 4) % 4] += mouse.moveY();
      }
      else
      {
         // update_x[(resize_pos + 1 + 4) % 4] += mouse.moveX();
         // update_y[(resize_pos + 1 + 4) % 4] += mouse.moveY();
         update_x[(resize_pos - 1 + 4) % 4] += mouse.moveX();
         update_y[(resize_pos - 1 + 4) % 4] += mouse.moveY();
      }
      update_x[resize_pos] += mouse.moveX();
      update_y[resize_pos] += mouse.moveY();

      float base_x = update_x[0];
      float base_y = update_y[0];

      // proportion_x = (Point::distance(update_x[1], update_y[1], update_x[0], update_y[0])) / width_box * 1.0;
      // proportion_y = (Point::distance(update_x[1], update_y[1], update_x[2], update_y[2])) / height_box * 1.0;
      proportion_x = (update_x[1] - update_x[0]) / width_box * 1.0;
      proportion_y = (update_y[2] - update_y[1]) / height_box * 1.0;

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

   virtual void update_shape(Mouse mouse)
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

   virtual void move_shape(Mouse mouse)
   {
      for (int i = 0; i < elems; i++)
      {
         vx[i] += mouse.moveX();
         vy[i] += mouse.moveY();
      }
      for (int i = 0; i < 4; i++)
      {
         update_x[i] += mouse.moveX();
         update_y[i] += mouse.moveY();
      }
      up_y += mouse.moveY();
      midle_x += mouse.moveX();
   }

   virtual void update(Mouse mouse)
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

   bool checkUpdateShape(Mouse mouse)
   {
      for (int i = 0; i < 4; i++)
      {
         if (Point::distance(mouse.getX(), mouse.getY(), update_x[i], update_y[i]) < RADIUS_BALL)
         {
            resize_pos = i;
            updateShape = true;
            return true;
         }
      }
      if (Point::distance(mouse.getX(), mouse.getY(), midle_x, up_y) < RADIUS_BALL)
      {
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

   virtual void fill(void)
   {
      this->fill_flag = !fill_flag;
   }

   virtual bool isInside(float x, float y)
   {
      return Point::isInside(x, y, elems, vx, vy);
   }

   virtual void rotate_shape(Mouse mouse)
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
   void color(float r, float g, float b)
   {
      this->r = r;
      this->g = g;
      this->b = b;
   }
   virtual void high_light()
   {
      CV::color(0.5f + (1 + cos(this->gold)) / 4, 0.5f + (1 + cos(this->gold)) / 4, 0);
      this->gold += 0.05;
      CV::polygon(update_x, update_y, 4);
      CV::polygon(draw_x, draw_y, elems);
      CV::color(1, 1, 1);
      for (int i = 0; i < 4; i++)
      {
         CV::circleFill(update_x[i], update_y[i], RADIUS_BALL, 10);
      }
      CV::circleFill(midle_x, this->up_y, RADIUS_BALL, 10);
   }

protected:
   // Coords of shape
   float *vx;
   float *vy;
   int elems;

   // Coords of resize options
   float update_x[4];
   float update_y[4];
   float width_box;
   float height_box;

   // Coords rotate options
   float midle_x;
   float up_y;

   // Coords of shape relate to (0,0)
   float *draw_x;
   float *draw_y;

   float angle = 0.0;

   // Flags and select
   bool fill_flag = true;
   float r = 1, g = 1, b = 1;
   float gold = 0.2;

   bool updateShape = false;
   int resize_pos;
};

#endif
