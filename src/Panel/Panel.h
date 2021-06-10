#ifndef __PANEL_H__
#define __PANEL_H__

#include "../Handles/Vector2.h"
#include "Botao.h"

#include "../gl_canvas2d.h"

#include <list>
class Panel
{
private:
   /* data */
   float coord_x;
   float coord_y;
   float width;
   float height;

   std::list<Botao *> buttons;

   float r, g, b;

public:
   Panel(float x, float y, float width, float height);
   ~Panel();

   void color(float r, float g, float b)
   {
      this->r = r;
      this->g = g;
      this->b = b;
   }

   void addButton(float _x, float _y, float _larg, float _alt, std::string _label)
   {
      _x += coord_x;
      _y += coord_y;
      buttons.push_front(new Botao(_x, _y, _larg, _alt, _label));
   }

   void addButton(float _x, float _y, float _larg, float _alt, float *vx, float *vy, int elems)
   {
      _x += coord_x;
      _y += coord_y;
      buttons.push_front(new Botao(_x, _y, _larg, _alt, vx, vy, elems));
   }

   void render()
   {
      CV::color(r, g, b);
      CV::rectFill(coord_x, coord_y, coord_x + width, coord_y + height);
      CV::rect(coord_x, coord_y, coord_x + width, coord_y + height);

      for (auto it = buttons.begin(); it != buttons.end(); ++it)
      {
         (*it)->Render();
      }
   }
};

Panel::Panel(float x, float y, float width, float height)
{
   coord_x = x;
   coord_y = y;
   this->width = width;
   this->height = height;
   r = g = b = 1;
}

Panel::~Panel()
{
}

#endif
