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

// Variables to track chosen shapes
Shape *Drag;
Shape *Choose;

// Variable to track new shape
int New_Shape;

// Variable to keep chosen shape when clicking a panel option
bool click = false;

// Variable to keep track of new polygons
std::list<Vector2> newPolygon;

// List of shapes and
Panel *panel;
std::list<Shape *> shapes;

void save_program()
{
   FILE *fp;
   if ((fp = fopen("out.gr", "w")) == NULL)
   {
      printf("Erro na abertura do arquivo.");
      return;
   }
   else
      printf("Arquivo aberto com sucesso.");

   int n = shapes.size();
   fprintf(fp, "%d\n", n);

   for (auto it = shapes.begin(); it != shapes.end(); ++it)
   {
      int n = (*it)->getType();
      float ang = (*it)->getAngle() * (180.0 / PI); // convert to angle

      Vector2 proportion = (*it)->getProportion();
      Vector2 polygon;
      float *rgb = (*it)->getRGB();

      int elems;

      fprintf(fp, "%d ", n);
      switch (n)
      {
      case QUADRADO:
         fprintf(fp, "%.f %.f %.f %.f ",
                 (*it)->getX(),
                 (*it)->getY(),
                 (*it)->getWidth(),
                 (*it)->getHeight());
         break;
      case TRIANGULO:
         fprintf(fp, "%.f %.f %.f %.f ",
                 (*it)->getX(),
                 (*it)->getY(),
                 (*it)->getWidth(),
                 (*it)->getHeight());
         break;
      case CIRCULO:
         fprintf(fp, "%.f %.f %.f ",
                 ((*it)->getX() + (*it)->getWidth()) / 2.0,
                 ((*it)->getY() + (*it)->getWidth()) / 2.0,
                 (*it)->getWidth() / 2.0);
         break;
      case POLIGONO:
         elems = (*it)->getElems();
         fprintf(fp, "%d ", elems);
         for (int i = 0; i < elems; i++)
         {
            polygon.set((*it)->getBaseXY(i));
            fprintf(fp, "%.f %.f ", polygon.x, polygon.y);
         }
         break;
      default:
         break;
      }
      fprintf(fp, "%f %f %f %.2f %.2f %.2f\n",
              ang, proportion.x, proportion.y,
              rgb[0], rgb[1], rgb[2]);
      delete rgb;
   }

   fclose(fp);
}

void create_panel()
{
   float offset = 10;
   float panel_w = screenWidth / 2;
   float panel_h = 150;
   float panel_x = screenHeight / 3 - offset;
   float panel_y = screenHeight - panel_h - offset;

   panel = new Panel(panel_x, panel_y, panel_w, panel_h);

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

   panel->render();

   if (newPolygon.size() > 0)
   {
      float vx[newPolygon.size()];
      float vy[newPolygon.size()];
      int i = 0;
      for (auto it = newPolygon.begin(); it != newPolygon.end(); ++it, i++)
      {
         vx[i] = (*it).x;
         vy[i] = (*it).y;
      }
      CV::color(0, 1, 0);
      CV::polygonFill(vx, vy, newPolygon.size());
      CV::color(1, 1, 1);
      for (auto it = newPolygon.begin(); it != newPolygon.end(); ++it)
         CV::circleFill((*it).x, (*it).y, 5, 10);
   }

   if (New_Shape != -1)
   {
      CV::color(0, 1, 0);
      CV::circleFill(mouse_state->getX(), mouse_state->getY(), RADIUS_BALL, 10);
      CV::color(0, 0, 0);
      CV::circle(mouse_state->getX(), mouse_state->getY(), RADIUS_BALL, 10);
   }
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   // printf("\nTecla: %d", key);

   switch (key)
   {
   case 27: //finaliza programa
      save_program();
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
   // printf("\nLiberou tecla: %d", key);

   switch (key)
   {
   case 214:
      mouse_state->setCtrl(false);
      break;
   }
}

bool add_shape(int *shape)
{
   Shape *shp;
   double height;
   switch (*shape)
   {
   case -1:
      return false;
   case QUADRADO:
      shp = new Square_shape(mouse_state->getX() - 50, mouse_state->getY() - 50, 100, 100);
      break;
   case TRIANGULO:
      height = 86.0254037844386; // height of an equilateral triangle
      shp = new Triangle_shape(mouse_state->getX() - 50, mouse_state->getY() - 50, 100, height);
      break;
   case CIRCULO:
      shp = new Circle_shape(mouse_state->getX(), mouse_state->getY(), 50, 30);
      break;
   case POLIGONO:
      if (newPolygon.size() > 0 &&
          Point::distance(
              newPolygon.front().x,
              newPolygon.front().y,
              mouse_state->getX(),
              mouse_state->getY()) <= RADIUS_BALL)
      {
         int n = newPolygon.size();
         float vx[n];
         float vy[n];
         int i = 0;
         for (auto it = newPolygon.begin(); it != newPolygon.end(); ++it, i++)
         {
            vx[i] = (*it).x;
            vy[i] = (*it).y;
         }
         newPolygon.clear();
         shp = new Polygon_shape(vx, vy, n);
         break;
      }
      newPolygon.push_back(Vector2(mouse_state->getX(), mouse_state->getY()));
      return true;
   default:
      std::cout << "Figura Inválida..." << std::endl;
      exit(1);
      break;
   }
   shp->color(0, 1, 0);
   shapes.push_front(shp);
   Choose = shp;

   *shape = -1;
   click = true;
   return true;
}

bool check_panel()
{
   if (panel->insidePanel(*mouse_state))
   {
      New_Shape = -1;
   }

   Botao *ret = panel->isInside(*mouse_state);

   if (ret == nullptr)
   {
      return false;
   }

   switch (ret->get_function())
   {
   case QUADRADO:
      New_Shape = QUADRADO;
      break;
   case TRIANGULO:
      New_Shape = TRIANGULO;
      break;
   case CIRCULO:
      New_Shape = CIRCULO;
      break;
   case POLIGONO:
      New_Shape = POLIGONO;
      break;
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
   case DELETAR:
      if (Choose)
      {
         Drag = nullptr;
         shapes.remove(Choose);
         delete Choose;
         Choose = nullptr;
      }
      break;
   case PREENCHER:
      if (Choose)
      {
         Choose->fill();
      }
      break;
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
   return true;
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
         if (check_panel())
         {
            return;
         }

         if (add_shape(&New_Shape))
         {
            return;
         }

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

   New_Shape = -1;

   create_panel();

   mouse_state = new Mouse();

   CV::run();
}
