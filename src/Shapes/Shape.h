
#ifndef __SHAPE_H__
#define __SHAPE_H__

#define RADIUS_BALL 5
#define MIN_EDGE 5

#include "../gl_canvas2d.h"
#include "../Handles/Point.h"
#include "../Handles/Vector2.h"
class Shape
{
private:
   virtual void rotate_shape(Mouse mouse)
   {
      float base_x = vx[0];
      float base_y = vy[0];

      float vector_x = midle_x - base_x;
      float vector_y = up_y - base_y;

      float angle = Point::getAngle(midle_x, up_y, mouse.getX(), mouse.getY(), base_x, base_y);

      Vector2 rotate = Point::rotate(vector_x, vector_y, angle);

      midle_x = rotate.x + base_x;
      up_y = rotate.y + base_y;

      // Translate shpae points
      for (int i = 0; i < elems; i++)
      {
         rotate = Point::rotate(vx[i] - base_x,
                                vy[i] - base_y, angle);

         vx[i] = rotate.x + base_x;
         vy[i] = rotate.y + base_y;
      }
      for (int i = 0; i < 4; i++)
      {
         // Translate box points
         rotate = Point::rotate(update_x[i] - base_x,
                                update_y[i] - base_y, angle);

         update_x[i] = rotate.x + base_x;
         update_y[i] = rotate.y + base_y;

         // Translate box points relato to (0, 0)
         rotate = Point::rotate(update_base[i].x,
                                update_base[i].y, angle);

         update_base[i].set(rotate.x, rotate.y);
      }

      this->angle += angle;
   }

   virtual void resize_shape(Mouse mouse)
   {
      for (int i = -1; i <= 1; i += 2)
      {
         Vector2 aux_base = Vector2(
             update_x[(resize_pos + i + 4) % 4] - update_base[(resize_pos + i + 4) % 4].x,
             update_y[(resize_pos + i + 4) % 4] - update_base[(resize_pos + i + 4) % 4].y);

         Vector2 ret = Point::perpendicular(
             update_base[(resize_pos + i + 4) % 4] + aux_base,
             update_base[(resize_pos + 2) % 4] + aux_base,
             update_x[resize_pos] + mouse.moveX(),
             update_y[resize_pos] + mouse.moveY());

         update_x[(resize_pos + i + 4) % 4] = ret.x;
         update_y[(resize_pos + i + 4) % 4] = ret.y;
      }
      update_x[resize_pos] += mouse.moveX();
      update_y[resize_pos] += mouse.moveY();

      float base_x = update_x[0];
      float base_y = update_y[0];

      Vector2 dist_side_to_center = Vector2(
          Point::distance(update_x[1] - base_x, update_y[1] - base_y, 0, 0),
          Point::distance(update_x[3] - base_x, update_y[3] - base_y, 0, 0));

      Vector2 proportion = Vector2(
          dist_side_to_center.x / width_box * 1.0,
          dist_side_to_center.y / height_box * 1.0);

      Vector2 point1 = Point::perpendicular(update_base[1], update_base[2], update_x[1] - base_x, update_y[1] - base_y);
      Vector2 point2 = Point::perpendicular(update_base[2], update_base[3], update_x[3] - base_x, update_y[3] - base_y);

      float dist_side_to_side_x = Point::distance(update_x[1] - base_x, update_y[1] - base_y, point1.x, point1.y);
      float dist_side_to_side_y = Point::distance(update_x[3] - base_x, update_y[3] - base_y, point2.x, point2.y);

      std::cout << " dit: " << dist_side_to_center.x << " " << dist_side_to_center.y << std::endl;
      std::cout << " propo2: " << dist_side_to_side_x << " " << dist_side_to_side_y << std::endl;

      proportion.set(
          dist_side_to_side_x > std::max(dist_side_to_center.x, width_box) ? proportion.x * -1 : proportion.x,
          dist_side_to_side_y > std::max(dist_side_to_center.y, height_box) ? proportion.y * -1 : proportion.y);

      for (int i = 0; i < elems; i++)
      {
         Vector2 relative = draw[i] * proportion;

         Vector2 rotate = Point::rotate(relative.x, relative.y, angle);

         vx[i] = rotate.x + base_x;
         vy[i] = rotate.y + base_y;
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

   // Checks if mouse is inside of a resize option
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

   // Change fill of shape
   virtual void fill(void)
   {
      this->fill_flag = !fill_flag;
   }

   // Checks if mouse is inside of shape
   virtual bool isInside(Mouse mouse)
   {
      return Point::isInside(mouse.getX(), mouse.getY(), elems, vx, vy);
   }

   // Change Color
   void color(float r, float g, float b)
   {
      this->r = r;
      this->g = g;
      this->b = b;
   }

   // Highlight the selected shape
   virtual void high_light()
   {
      CV::color(0.5f + (1 + cos(this->gold)) / 4, 0.5f + (1 + cos(this->gold)) / 4, 0);
      this->gold += 0.05;
      CV::polygon(update_x, update_y, 4);
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
   Vector2 *draw;

   // Coords of resize sides relate to (0,0)
   Vector2 update_base[4];

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
