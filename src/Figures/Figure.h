
#ifndef __FIGURE_H__
#define __FIGURE_H__

#define RADIUS_BALL 5

#include "../gl_canvas2d.h"
#include "../Handles/Point.hpp"
#include "../Handles/Vector2.h"
class Figure
{
private:
   // Rotate Figure with mouse
   virtual void rotate_figure(Mouse mouse)
   {
      float base_x = (update_x[0] + update_x[2]) / 2.0;
      float base_y = (update_y[0] + update_y[2]) / 2.0;

      float vector_x = midle_x - base_x;
      float vector_y = up_y - base_y;

      // Gets angle and rotates based on mouse
      // coordinate and middle button option
      float angle = Point::getAngle(midle_x, up_y, mouse.getX(), mouse.getY(), base_x, base_y);
      Vector2 rotate = Point::rotate(vector_x, vector_y, angle);

      midle_x = rotate.x + base_x;
      up_y = rotate.y + base_y;

      // Translate figure points
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

   virtual void resize_figure(Mouse mouse)
   {
      // Update settings square
      for (int i = -1; i <= 1; i += 2)
      {
         // Vector with adjacents points realte to (0, 0)
         Vector2 aux_base = Vector2(
             update_x[(resize_pos + i + 4) % 4] - update_base[(resize_pos + i + 4) % 4].x,
             update_y[(resize_pos + i + 4) % 4] - update_base[(resize_pos + i + 4) % 4].y);

         // Update adjacents points to keep proportion
         Vector2 ret = Point::perpendicular(
             update_base[(resize_pos + i + 4) % 4] + aux_base,
             update_base[(resize_pos + 2) % 4] + aux_base,
             update_x[resize_pos] + mouse.moveX(),
             update_y[resize_pos] + mouse.moveY());

         update_x[(resize_pos + i + 4) % 4] = ret.x;
         update_y[(resize_pos + i + 4) % 4] = ret.y;
      }
      // Update selected vertice
      update_x[resize_pos] += mouse.moveX();
      update_y[resize_pos] += mouse.moveY();

      // Base coords to convert to (0, 0)
      float base_x = update_x[0];
      float base_y = update_y[0];

      // Calculate resize ratio based on coordinates (0,0)
      Vector2 dist_side_to_center = Vector2(
          Point::distance(update_x[1] - base_x, update_y[1] - base_y, 0, 0),
          Point::distance(update_x[3] - base_x, update_y[3] - base_y, 0, 0));
      // Calculate proportion between update box and base box
      Vector2 proportion = Vector2(
          dist_side_to_center.x / width_box * 1.0,
          dist_side_to_center.y / height_box * 1.0);

      Vector2 point1 = Point::perpendicular(update_base[1], update_base[2], update_x[1] - base_x, update_y[1] - base_y);
      Vector2 point2 = Point::perpendicular(update_base[2], update_base[3], update_x[3] - base_x, update_y[3] - base_y);

      float dist_side_to_side_x = Point::distance(update_x[1] - base_x, update_y[1] - base_y, point1.x, point1.y);
      float dist_side_to_side_y = Point::distance(update_x[3] - base_x, update_y[3] - base_y, point2.x, point2.y);
      // Checks if distance between an update option point to base option box at other side is bigger
      // than this distance to center. If true, then the figure is mirrored in x ou y direction.
      proportion.set(
          dist_side_to_side_x > std::max(dist_side_to_center.x, width_box) ? proportion.x * -1 : proportion.x,
          dist_side_to_side_y > std::max(dist_side_to_center.y, height_box) ? proportion.y * -1 : proportion.y);

      // Update coordinates
      for (int i = 0; i < elems; i++)
      {
         Vector2 relative = draw[i] * proportion;

         Vector2 rotate = Point::rotate(relative.x, relative.y, angle);

         vx[i] = rotate.x + base_x;
         vy[i] = rotate.y + base_y;
      }
      // Update rotate option (the middle one)
      float up_or_down_x = proportion.y < 0 ? -10 : 10;
      float up_or_down_y = proportion.y < 0 ? -10 : 10;
      midle_x = ((update_x[2] + update_x[3]) / 2) + sin(angle) * up_or_down_x * -1.0;
      up_y = ((update_y[2] + update_y[3]) / 2) + cos(angle) * up_or_down_y * 1.0;

      this->proportion.set(proportion);
   }

   virtual void update_figure(Mouse mouse)
   {
      if (resize_pos == -1)
      {
         rotate_figure(mouse);
      }
      else
      {
         resize_figure(mouse);
      }
   }

   virtual void move_figure(Mouse mouse)
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
   // Rotate fig given angle, used when loading a figure
   void rotate_angle(float angle)
   {
      angle = angle * (PI / 180.0);

      float base_x = update_x[0];
      float base_y = update_y[0];

      float vector_x = midle_x - base_x;
      float vector_y = up_y - base_y;

      Vector2 rotate = Point::rotate(vector_x, vector_y, angle);

      midle_x = rotate.x + base_x;
      up_y = rotate.y + base_y;

      // Translate figure points
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

      this->angle = angle;
   }
   // Resive fig given proportion, used when loading a figure
   void resize_proportion(Vector2 proportion)
   {
      float base_x = update_x[0];
      float base_y = update_y[0];
      // Update figure coordinates
      for (int i = 0; i < elems; i++)
      {
         Vector2 relative = draw[i] * proportion;

         vx[i] = relative.x + base_x;
         vy[i] = relative.y + base_y;
      }
      // Update resize options coordinates
      for (int i = 0; i < 4; i++)
      {
         Vector2 relative = update_base[i] * proportion;

         update_x[i] = relative.x + base_x;
         update_y[i] = relative.y + base_y;
      }
      float up_or_down_x = proportion.y < 0 ? -10 : 10;
      float up_or_down_y = proportion.y < 0 ? -10 : 10;
      midle_x = ((update_x[2] + update_x[3]) / 2) + sin(this->angle) * up_or_down_x * -1.0;
      up_y = ((update_y[2] + update_y[3]) / 2) + cos(this->angle) * up_or_down_y * 1.0;

      this->proportion.set(proportion);
   }

   virtual void render(void)
   {
      CV::color(this->r, this->g, this->b);
      if (fill_flag)
         CV::polygonFill(vx, vy, elems);
      else
         CV::polygon(vx, vy, elems);
      CV::color(1, 1, 1);
   }

   // Checks if are resizing/rotating or just mobing
   virtual void update(Mouse mouse)
   {
      if (updateFigure)
      {
         update_figure(mouse);
      }
      else
      {
         move_figure(mouse);
      }
   }

   // Checks if mouse is inside of figure
   virtual bool isInside(Mouse mouse)
   {
      return Point::isInside(mouse.getX(), mouse.getY(), elems, vx, vy);
   }

   // Checks if mouse is inside of a
   // resize option and set option
   bool checkUpdateFigure(Mouse mouse)
   {
      for (int i = 0; i < 4; i++)
      {
         if (Point::distance(mouse.getX(), mouse.getY(), update_x[i], update_y[i]) < RADIUS_BALL)
         {
            resize_pos = i;
            updateFigure = true;
            return true;
         }
      }
      if (Point::distance(mouse.getX(), mouse.getY(), midle_x, up_y) < RADIUS_BALL)
      {
         resize_pos = -1;
         updateFigure = true;
         return true;
      }
      updateFigure = false;
      return false;
   }

   void releaseMouse(void)
   {
      updateFigure = false;
   }

   // Change Color
   void color(float r, float g, float b)
   {
      this->r = r;
      this->g = g;
      this->b = b;
   }

   // Change fill of figure
   virtual void fill(void)
   {
      this->fill_flag = !this->fill_flag;
   }

   // Highlight the selected figure
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

   // getters
   int getType() { return this->type; }
   int getElems() { return this->elems; }

   bool getFill() { return this->fill_flag; }

   float getAngle() { return this->angle; }

   float getWidth() { return this->width_box; }
   float getHeight() { return this->height_box; }

   float getX() { return this->update_x[0]; }
   float getY() { return this->update_y[0]; }

   Vector2 getIndexXY(int i)
   {
      return Vector2(draw[i].x, draw[i].y);
   }

   Vector2 getProportion() { return this->proportion; }

   float *getRGB()
   {
      float *rgb = new float[3];
      rgb[0] = r;
      rgb[1] = g;
      rgb[2] = b;
      return rgb;
   }

   virtual ~Figure()
   {
      delete vx;
      delete vy;
      delete draw;
   }

protected:
   // Coords of figure
   float *vx;
   float *vy;
   int elems;

   // Coords of resize options (the box around)
   float update_x[4];
   float update_y[4];
   float width_box;
   float height_box;

   // Coords rotate options
   float midle_x;
   float up_y;

   // Coords of figure relate to (0,0)
   Vector2 *draw;

   // Coords of resize options relate to (0,0)
   Vector2 update_base[4];

   float angle = 0.0;

   // Flags and select
   bool fill_flag = true;
   float r = 1, g = 1, b = 1;
   float gold = 0.2;

   // Stores type and proportion of object to save into file
   int type;
   Vector2 proportion;

   // Coords and flag of resize option selected
   bool updateFigure = false;
   int resize_pos;
};

#endif
