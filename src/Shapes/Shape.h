
#ifndef __SHAPE_H__
#define __SHAPE_H__

#define RADIUS_BALL 5
#define MIN_EDGE 5

class Shape
{
public:
   virtual void render(void){      
      CV::color(this->r, this->g, this->b);
      if (fill_flag)
         CV::polygonFill(vx, vy, elems);
      else
         CV::polygon(vx, vy, elems);
      CV::color(1, 1, 1);
   }

   virtual void fill(void) = 0;
   virtual void rotate_shape(Mouse mouse) = 0;
   virtual void update(Mouse) = 0;
   virtual void releaseMouse(void) = 0;
   virtual bool checkUpdateShape(Mouse) = 0;
   virtual bool isInside(float x, float y) = 0;
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
      CV::polygon(vx, vy, 4);
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
};

#endif
