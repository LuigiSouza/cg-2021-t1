#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "../gl_canvas2d.h"

class Botao
{
  float altura, largura, x, y;
  char label[100];
  float *vx;
  float *vy;

  int elems;

  bool fig;

public:
  Botao(float _x, float _y, float _larg, float _alt, std::string _label)
  {
    altura = _alt;
    largura = _larg;
    x = _x;
    y = _y;
    fig = false;
    strcpy(label, _label.c_str());
  }

  Botao(float _x, float _y, float _larg, float _alt, float *vx, float *vy, int elems)
  {
    altura = _alt;
    largura = _larg;
    x = _x;
    y = _y;
    fig = true;
    this->vx = vx;
    this->vy = vy;
    this->elems = elems;
  }

  void Render()
  {
    CV::color(0, 1, 0);
    CV::rectFill(x, y, x + largura, y + altura);
    CV::color(0, 0, 0);
    if (!fig)
      CV::text(x + 5, y + altura / 2, label); //escreve o label do botao mais ou menos ao centro.
    else
    {
      CV::translate(x, y);
      CV::polygon(vx, vy, elems);
      CV::translate(0, 0);
    }
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool Colidiu(int mx, int my)
  {
    if (mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura))
    {
      return true;
    }
    return false;
  }
};

#endif
