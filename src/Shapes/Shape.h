
#ifndef __SHAPE_H__
#define __SHAPE_H__

#define RADIUS_BALL 5
#define MIN_EDGE 5

#include "../gl_canvas2d.h"
#include "../Handles/Point.h"
class Shape
{
private:
   virtual void rotate_shape(Mouse mouse)
   {
      float relative_x;
      float relative_y;

      float base_x = vx[0];
      float base_y = vy[0];

      float vector_x = midle_x - base_x;
      float vector_y = up_y - base_y;

      float angle = Point::getAngle(midle_x, up_y, mouse.getX(), mouse.getY(), base_x, base_y);

      Point::rotate(vector_x, vector_y, angle, &vector_x, &vector_y);

      midle_x = vector_x + base_x;
      up_y = vector_y + base_y;

      // Translate shpae points
      for (int i = 0; i < elems; i++)
      {
         relative_x = vx[i] - base_x;
         relative_y = vy[i] - base_y;

         Point::rotate(relative_x, relative_y, angle, &relative_x, &relative_y);

         vx[i] = relative_x + base_x;
         vy[i] = relative_y + base_y;
      }
      // Translate box points
      for (int i = 0; i < 4; i++)
      {
         relative_x = update_x[i] - base_x;
         relative_y = update_y[i] - base_y;

         Point::rotate(relative_x, relative_y, angle, &relative_x, &relative_y);

         update_x[i] = relative_x + base_x;
         update_y[i] = relative_y + base_y;

         // Translate box points relato to (0, 0)
         relative_x = update_x_base[i];
         relative_y = update_y_base[i];

         Point::rotate(relative_x, relative_y, angle, &relative_x, &relative_y);

         update_x_base[i] = relative_x;
         update_y_base[i] = relative_y;
      }

      this->angle += angle;
   }

   virtual void resize_shape(Mouse mouse)
   {
      float relative_x;
      float relative_y;

      float x, y;
      for (int i = -1; i <= 1; i += 2)
      {
         float aux_base_x = update_x[(resize_pos + i + 4) % 4] - update_x_base[(resize_pos + i + 4) % 4];
         float aux_base_y = update_y[(resize_pos + i + 4) % 4] - update_y_base[(resize_pos + i + 4) % 4];
         Point::perpendicular(
             update_x_base[(resize_pos + i + 4) % 4] + aux_base_x,
             update_y_base[(resize_pos + i + 4) % 4] + aux_base_y,
             update_x_base[(resize_pos + 2) % 4] + aux_base_x,
             update_y_base[(resize_pos + 2) % 4] + aux_base_y,
             update_x[resize_pos] + mouse.moveX(),
             update_y[resize_pos] + mouse.moveY(),
             &x, &y);

         update_x[(resize_pos + i + 4) % 4] = x;
         update_y[(resize_pos + i + 4) % 4] = y;
      }
      update_x[resize_pos] += mouse.moveX();
      update_y[resize_pos] += mouse.moveY();

      float base_x = update_x[0];
      float base_y = update_y[0];

      float dist_x = Point::distance(update_x[1] - base_x, update_y[1] - base_y, 0, 0);
      float dist_y = Point::distance(update_x[3] - base_x, update_y[3] - base_y, 0, 0);
      float proportion_x = dist_x / width_box * 1.0;
      float proportion_y = dist_y / height_box * 1.0;

      float px;
      float py;
      Point::perpendicular(update_x_base[1], update_y_base[1], update_x_base[2], update_y_base[2], update_x[1] - base_x, update_y[1] - base_y, &px, &py);
      float dist_points_x = Point::distance(update_x[1] - base_x, update_y[1] - base_y, px, py);

      std::cout << " dit: " << dist_x << " " << dist_points_x << std::endl;

      Point::perpendicular(update_x_base[2], update_y_base[2], update_x_base[3], update_y_base[3], update_x[3] - base_x, update_y[3] - base_y, &px, &py);
      float dist_points_y = Point::distance(update_x[3] - base_x, update_y[3] - base_y, px, py);

      proportion_x = dist_points_x > std::max(dist_x, width_box) ? proportion_x * -1 : proportion_x;
      proportion_y = dist_points_y > std::max(dist_y, height_box) ? proportion_y * -1 : proportion_y;

      std::cout << " propo2: " << proportion_x << " " << proportion_y << std::endl;

      for (int i = 0; i < elems; i++)
      {
         relative_x = draw_x[i] * proportion_x * 1.0;
         relative_y = draw_y[i] * proportion_y * 1.0;

         Point::rotate(relative_x, relative_y, angle, &relative_x, &relative_y);

         vx[i] = relative_x + base_x;
         vy[i] = relative_y + base_y;
      }
      midle_x = ((update_x[2] + update_x[3]) / 2) + sin(angle) * 10 * -1.0;
      up_y = ((update_y[2] + update_y[3]) / 2) + cos(angle) * 10 * 1.0;
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
      CV::polygon(update_x_base, update_y_base, 4);
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

   // Coords of resize sides relate to (0,0)
   float update_x_base[4];
   float update_y_base[4];

   float angle = 0.0;

   // Flags and select
   bool fill_flag = true;
   float r = 1, g = 1, b = 1;
   float gold = 0.2;

   // Coords and flag of option selected
   bool updateShape = false;
   int resize_pos;
};

#endif
