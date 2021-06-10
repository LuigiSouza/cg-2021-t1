/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp

//  Todos os arquivos do projeto devem ser .cpp
//
//  Versao 2.0
//
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>

#include "gl_canvas2d.h"

#include "Bmp.h"
#include "Imagem.h"

#include "Handles/HandleMouse.h"

#include "Panel/Panel.h"

#include "Shapes/Polygon_shape.h"
#include "Shapes/Square_shape.h"
#include "Shapes/Circle_shape.h"
#include "Shapes/Triangle_shape.h"

int screenWidth = 800, screenHeight = 680;
Mouse *mouse_state;

Shape *Drag;
Shape *Choose;

float r = 0.23;

int fps = 0;

std::list<Shape *> shapes;
std::list<Panel *> panels;

void update()
{
   if (Drag)
   {
      Drag->update(*mouse_state);
   }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
   CV::clear(0, 0, 0);
   CV::color(1, 1, 1);

   for (auto it = shapes.rbegin(); it != shapes.rend(); ++it)
   {
      (*it)->render();
   }

   for (auto it = panels.begin(); it != panels.end(); ++it)
   {
      (*it)->render();
   }

   if (Choose)
   {
      Choose->high_light();
   }
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nTecla: %d", key);

   switch (key)
   {
   case 27: //finaliza programa
      exit(0);
      break;
   case 214:
      mouse_state->setCtrl(true);
      break;
   }
}
//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou tecla: %d", key);

   switch (key)
   {
   case 43:
      if (Choose)
      {
         for (auto it = ++shapes.begin(); it != shapes.end(); ++it)
         {
            if ((*it) == Choose)
            {
               shapes.remove(Choose);
               shapes.emplace(--it, Choose);
               break;
            }
         }
      }
      break;
   case 45:
      if (Choose)
      {
         for (auto it = shapes.begin(); it != --shapes.end(); ++it)
         {
            if ((*it) == Choose)
            {
               auto aux = ++it;
               shapes.remove(Choose);
               shapes.emplace(++aux, Choose);
               break;
            }
         }
      }
      break;
   case 214:
      mouse_state->setCtrl(false);
      break;
   }
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);

   mouse_state->setX(x);
   mouse_state->setY(y);

   update();

   if (button == 0)
   {
      if (state == 1)
      {
         if (Choose)
            Choose->releaseMouse();
         Choose = Drag;
         Drag = nullptr;
      }
      else if (state == 0)
      {
         if (Choose && Choose->checkUpdateShape(*mouse_state))
         {
            Drag = Choose;
            return;
         }
         for (auto it = shapes.begin(); it != shapes.end(); ++it)
         {
            if ((*it)->isInside(*mouse_state))
            {
               Drag = (*it);
               Choose = Drag;
               break;
            }
         }
      }
   }
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Teste trabalho 1");

   Square_shape *sq1 = new Square_shape(200, 200, 100, 100);
   Circle_shape *sq2 = new Circle_shape(100, 100, 50, 20);
   sq1->fill();
   Triangle_shape *sq5 = new Triangle_shape(300, 300, 100, 100);
   sq1->color(1, 0, 0);
   sq2->color(0, 0, 1);
   sq5->color(1, 1, 0);
   shapes.push_front(sq1);
   shapes.push_front(sq2);
   shapes.push_front(sq5);

   float offset = 10;
   float panel_w = screenWidth / 2;
   float panel_h = 150;
   float panel_x = screenHeight / 3 - offset;
   float panel_y = screenHeight - panel_h - offset;

   Panel *panel = new Panel(panel_x, panel_y, panel_w, panel_h);
   float square_x[] = {10, 20, 20, 10};
   float square_y[] = {10, 10, 20, 20};
   panel->addButton(10, 10, 30, 30, square_x, square_y, 4);
   float triangle_x[] = {10, 20, 15};
   float triangle_y[] = {10, 10, 20};
   panel->addButton(10, 45, 30, 30, triangle_x, triangle_y, 3);
   panel->addButton(10, 80, 100, 25, "Circulo");
   panel->addButton(10, 115, 100, 25, "Poligono");

   panel->addButton(panel_w - 50, 115, 40, 25, "del");
   panel->addButton(panel_w - 50, 80, 40, 25, " +");
   panel->addButton(panel_w - 50, 45, 40, 25, " -");
   panels.push_front(panel);

   mouse_state = new Mouse();

   CV::run();
}
