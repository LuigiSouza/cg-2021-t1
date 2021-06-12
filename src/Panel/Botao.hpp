#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "../gl_canvas2d.h"
#include "../Handles/HandleMouse.hpp"
#include "../Handles/Vector2.h"

#include <string>
#include <iostream>

class Botao
{
private:
  float altura, largura, x, y;
  char label[100];

  float r, g, b;

  float *vx;
  float *vy;
  int elems;

  char function;

  bool isFigure;

  // Sets functions of button
  void set_function(int function, int *rgb);

public:
  Botao(float _x, float _y, float _larg, float _alt, int function, int *rgb);

  void render();

  bool isInside(Mouse mouse_state);

  float *get_rgb()
  {
    float *ret = new float[3];
    ret[0] = r;
    ret[1] = g;
    ret[2] = b;

    return ret;
  }

  char get_function()
  {
    return this->function;
  }

  void set_color(float r, float g, float b)
  {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  void set_color(int i)
  {
    float *rgb = CV::get_color(i);
    this->r = rgb[0];
    this->g = rgb[1];
    this->b = rgb[2];
  }
};

#endif
