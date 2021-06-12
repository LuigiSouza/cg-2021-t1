#include "Botao.hpp"

Botao::Botao(float _x, float _y, float _larg, float _alt, int function, int *rgb)
{
   altura = _alt;
   largura = _larg;
   x = _x;
   y = _y;
   r = b = 0;
   g = 1;
   this->function = function;
   set_function(function, rgb);
}

void Botao::render()
{
   CV::color(r, g, b);
   CV::rectFill(x, y, x + largura, y + altura);
   CV::color(0, 0, 0);
   if (!isFigure)
      CV::text(x + 5, y + altura / 2, label); //escreve o label do botao mais ou menos ao centro.
   else
      CV::polygon(vx, vy, elems);
}

bool Botao::isInside(Mouse mouse_state)
{
   if (mouse_state.getX() >= x && mouse_state.getX() <= (x + largura) && mouse_state.getY() >= y && mouse_state.getY() <= (y + altura))
   {
      return true;
   }
   return false;
}

void Botao::set_function(int function, int *rgb)
{
   isFigure = false;
   float ang, radius, inc;

   std::string a;
   switch (function)
   {
   case ELEVAR:
      a = "up";
      strcpy(label, a.c_str());
      return;
   case ABAIXAR:
      a = "down";
      strcpy(label, a.c_str());
      return;
   case DELETAR:
      a = "del";
      strcpy(label, a.c_str());
      return;
   case PREENCHER:
      a = "fill";
      strcpy(label, a.c_str());
      return;
   case COR:
      set_color(++(*rgb));
      strcpy(label, "");
      return;
   case QUADRADO:
      vx = new float[4];
      vy = new float[4];

      std::cout << "quad " << std::endl;

      elems = 4;
      vx[0] = x + 10;
      vy[0] = y + 10;
      vx[1] = x + 20;
      vy[1] = y + 10;
      vx[2] = x + 20;
      vy[2] = y + 20;
      vx[3] = x + 10;
      vy[3] = y + 20;
      break;
   case TRIANGULO:
      vx = new float[3];
      vy = new float[3];

      elems = 3;
      vx[0] = x + 10;
      vy[0] = y + 10;
      vx[1] = x + 20;
      vy[1] = y + 10;
      vx[2] = x + 15;
      vy[2] = y + 20;
      break;
   case CIRCULO:
      vx = new float[10];
      vy = new float[10];

      radius = 5;
      elems = 10;

      ang = 0;
      inc = PI_2 / elems;
      for (int lado = 0; lado < elems; lado++)
      {
         vx[lado] = (cos(ang) * radius) + x + 15;
         vy[lado] = (sin(ang) * radius) + y + 15;
         ang += inc;
      }
      break;
   case POLIGONO:
      vx = new float[5];
      vy = new float[5];

      elems = 5;

      vx[0] = x + 12;
      vy[0] = y + 10;
      vx[1] = x + 18;
      vy[1] = y + 10;
      vx[2] = x + 21;
      vy[2] = y + 15;
      vx[3] = x + 15;
      vy[3] = y + 20;
      vx[4] = x + 9;
      vy[4] = y + 15;

      break;
   default:
      std::cout << "Código de Figura Inválida..." << std::endl;
      exit(1);
      break;
   }
   isFigure = true;
}
