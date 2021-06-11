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

bool click = false;

float r = 0.23;

int fps = 0;

std::list<Shape *> shapes;
std::list<Panel *> panels;

void create_panel()
{
   float offset = 10;
   float panel_w = screenWidth / 2;
   float panel_h = 150;
   float panel_x = screenHeight / 3 - offset;
   float panel_y = screenHeight - panel_h - offset;

   Panel *panel = new Panel(panel_x, panel_y, panel_w, panel_h);

   panel->addButton(offset, 115, 30, 30, QUADRADO);
   panel->addButton(offset, 80, 30, 30, TRIANGULO);
   panel->addButton(offset, 45, 30, 30, CIRCULO);
   panel->addButton(offset, 10, 30, 30, POLIGONO);

   panel->addButton(panel_w - 60, 115, 50, 30, ELEVAR);
   panel->addButton(panel_w - 60, 80, 50, 30, ABAIXAR);
   panel->addButton(panel_w - 60, 45, 50, 30, DELETAR);
   panel->addButton(panel_w - 60, 10, 50, 30, PREENCHER);

   for (int i = 0; i < 16; i++)
   {
      panel->addButton(((int)i / 4) * 40 + (panel_w / 4), ((i % 4) * 35) + 10, 40, 30, COR);
   }

   panels.push_front(panel);
}

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

   if (Choose)
   {
      Choose->high_light();
   }

   for (auto it = panels.begin(); it != panels.end(); ++it)
   {
      (*it)->render();
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
   case 214:
      mouse_state->setCtrl(false);
      break;
   }
}

void check_panel()
{
   for (auto it = panels.begin(); it != panels.end(); ++it)
   {
      Botao *ret = (*it)->isInside(*mouse_state);

      if (ret == nullptr)
         continue;

      switch (ret->get_function())
      {
      case ELEVAR:
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
      case ABAIXAR:
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
      case PREENCHER:
         if (Choose)
         {
            Choose->fill();
         }
         break;
      case DELETAR:
         if (Choose)
         {
            Drag = nullptr;
            shapes.remove(Choose);
            delete Choose;
            Choose = nullptr;
         }
      case COR:
         if (Choose)
         {
            float *rgb = ret->get_rgb();
            Choose->color(rgb[0], rgb[1], rgb[2]);
            delete rgb;
         }
         break;
      
      default:
         std::cout << "Funcao Desconhecida..." << std::endl;
         exit(1);
         break;
      }
      click = true;
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
         if (!click)
         {
            Choose = Drag;
            Drag = nullptr;
         }
         click = false;
      }
      else if (state == 0)
      {
         check_panel();

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
   Circle_shape *sq2 = new Circle_shape(100, 100, 50, 30);
   Triangle_shape *sq5 = new Triangle_shape(300, 300, 100, 100);
   sq1->color(1, 0, 0);
   sq2->color(0, 0, 1);
   sq5->color(1, 1, 0);
   shapes.push_front(sq1);
   shapes.push_front(sq2);
   shapes.push_front(sq5);

   create_panel();

   mouse_state = new Mouse();

   CV::run();
}
