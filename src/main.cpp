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

#include "Shapes/Square.h"

//variaveis globais

int screenWidth = 800, screenHeight = 680;
Bmp *bmp;
Mouse *mouse_state;

Shape *Drag;
Shape *Choose;

float angle = 0.05;
float vx[] = {-50, 50, 50, -50};
float vy[] = {-50, -50, 50, 50};

float r = 0.23;

std::list<Shape *> shapes;

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

   //angle -= 0.05;

   for (int i = 0; i < 4; i++)
   {
      float x = vx[i];
      vx[i] = x * cos(angle) - vy[i] * sin(angle);
      vy[i] = x * sin(angle) + vy[i] * cos(angle);
   }

   CV::translate(200, 200);
   CV::polygon(vx, vy, 4);
   CV::translate(0, 0);

   for (auto it = shapes.rbegin(); it != shapes.rend(); ++it)
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
            if ((*it)->isInside(mouse_state->getX(), mouse_state->getY()))
            {
               Drag = (*it);
               shapes.remove(*it);
               shapes.push_front(Drag);
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

   bmp = new Bmp("azag.bmp");
   bmp->convertBGRtoRGB();

   Square *sq1 = new Square(200, 200, 100, 100);
   Square *sq2 = new Square(250, 250, 100, 100);
   Square *sq3 = new Square(300, 300, 100, 100);
   sq1->color(1, 0, 0);
   sq2->color(0, 1, 0);
   sq3->color(0, 0, 1);
   shapes.push_front(sq1);
   shapes.push_front(sq2);
   shapes.push_front(sq3);

   mouse_state = new Mouse();

   CV::run();
}
