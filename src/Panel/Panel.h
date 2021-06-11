#ifndef __PANEL_H__
#define __PANEL_H__

#include "../Handles/Vector2.h"
#include "../Handles/HandleMouse.h"
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

   int color_control;

   float r, g, b;

public:
   Panel(float x, float y, float width, float height);
   ~Panel();

   Botao *isInside(Mouse mouse)
   {
      Botao *ret;
      for (auto it = buttons.begin(); it != buttons.end(); ++it)
      {
         if ((*it)->Colidiu(mouse))
         {
            ret = (*it);
            return ret;
         }
      }
      return nullptr;
   }

   void color(float r, float g, float b)
   {
      this->r = r;
      this->g = g;
      this->b = b;
   }

   void addButton(float _x, float _y, float _larg, float _alt, int function)
   {
      _x += coord_x;
      _y += coord_y;
      buttons.push_front(new Botao(_x, _y, _larg, _alt, function, &color_control));
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
   color_control = 0;
   r = g = b = 1;
}

Panel::~Panel()
{
}

#endif
