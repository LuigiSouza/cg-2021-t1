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
#include <fstream>

#include "gl_canvas2d.h"

#include "Handles/HandleMouse.hpp"

#include "Panel/Panel.hpp"

#include "Figures/Polygon_figure.h"
#include "Figures/Square_figure.h"
#include "Figures/Circle_figure.h"
#include "Figures/Triangle_figure.h"
#include "Figures/Figure.h"

int screenWidth = 800, screenHeight = 680;
Mouse *mouse_state;

// Variable to keep chosen figure when clicking a panel option
bool click = false;

// Variables to track chosen figures
Figure *Drag;
Figure *Choose;

// Variable to track new figure
int New_Figure;
std::list<Vector2> newPolygon;

Panel *panel;
std::list<Figure *> figures;

/***********************************************************
*
* Render Functions
*
************************************************************/

void render_cursor_polygon()
{
   if (New_Figure != -1)
   {
      CV::color(0, 1, 0);
      CV::circleFill(mouse_state->getX(), mouse_state->getY(), RADIUS_BALL, 10);
      CV::color(0, 0, 0);
      CV::circle(mouse_state->getX(), mouse_state->getY(), RADIUS_BALL, 10);
   }

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
}

void render_figures()
{
   for (auto it = figures.rbegin(); it != figures.rend(); ++it)
      (*it)->render();
}

void high_light()
{
   if (Choose)
      Choose->high_light();
}

/***********************************************************
*
* Load/Create Functions
*
************************************************************/

void load_file()
{
   std::ifstream infile("out.gr");

   int n;
   infile >> n;

   Figure *shp;

   for (int i = 0; i < n; i++)
   {
      int type;
      infile >> type;

      // Loads each type of figure
      if (type == QUADRADO)
      {
         float x, y, width, height;
         infile >> x >> y >> width >> height;
         shp = new Square_figure(x, y, width, height);
      }
      else if (type == TRIANGULO)
      {
         float x, y, width, height;
         infile >> x >> y >> width >> height;
         shp = new Triangle_figure(x, y, width, height);
      }
      else if (type == CIRCULO)
      {
         float x, y, radius;
         infile >> x >> y >> radius;
         shp = new Circle_figure(x, y, radius, 30);
      }
      else if (type == POLIGONO)
      {
         int elems;
         float base_x, base_y;
         infile >> elems >> base_x >> base_y;

         float vx[elems], vy[elems];

         for (int i = 0; i < elems; i++)
         {
            infile >> vx[i] >> vy[i];
            vx[i] += base_x;
            vy[i] += base_y;
         }

         shp = new Polygon_figure(vx, vy, elems);
      }
      else
      {
         std::cout << "Tipo inválido..." << std::endl;
         exit(1);
      }
      float x, y, angle;
      infile >> angle >> x >> y;
      Vector2 propo(x, y);

      // Resizes and rotates
      shp->resize_proportion(propo);
      shp->rotate_angle(angle);

      float r, g, b;
      infile >> r >> g >> b;
      shp->color(r, g, b);

      figures.push_back(shp);
   }

   infile.close();
}

void save_file()
{
   FILE *fp;
   if ((fp = fopen("out.gr", "w")) == NULL)
   {
      printf("Erro na abertura do arquivo.");
      return;
   }
   else
      printf("Arquivo aberto com sucesso.");

   int n = figures.size();
   fprintf(fp, "%d\n", n);

   for (auto it = figures.begin(); it != figures.end(); ++it)
   {
      int n = (*it)->getType();
      float ang = (*it)->getAngle() * (180.0 / PI); // convert radian to angle

      Vector2 polygon;
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
                 (*it)->getX() + (*it)->getWidth() / 2.0,
                 (*it)->getY() + (*it)->getWidth() / 2.0,
                 (*it)->getWidth() / 2.0);
         break;
      case POLIGONO:
         elems = (*it)->getElems();
         fprintf(fp, "%d ", elems);
         fprintf(fp, "%.f %.f ", (*it)->getX(), (*it)->getY());
         for (int i = 0; i < elems; i++)
         {
            polygon.set((*it)->getIndexXY(i));
            fprintf(fp, "%.f %.f ", polygon.x, polygon.y);
         }
         break;
      default:
         std::cout << "Tipo inválido..." << std::endl;
         exit(1);
         break;
      }
      Vector2 proportion = (*it)->getProportion();
      float *rgb = (*it)->getRGB();

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

/***********************************************************
*
* Update Functions
*
************************************************************/

void update()
{
   if (Drag)
   {
      Drag->update(*mouse_state);
   }
}

bool check_panel()
{
   if (panel->insidePanel(*mouse_state))
   {
      New_Figure = -1;
      newPolygon.clear();
   }

   Botao *button = panel->buttonClicked(*mouse_state);

   if (button == nullptr)
   {
      return false;
   }

   switch (button->get_function())
   {
   case QUADRADO:
      New_Figure = QUADRADO;
      break;
   case TRIANGULO:
      New_Figure = TRIANGULO;
      break;
   case CIRCULO:
      New_Figure = CIRCULO;
      break;
   case POLIGONO:
      New_Figure = POLIGONO;
      break;
   case ELEVAR:
      if (Choose)
      {
         for (auto it = ++figures.begin(); it != figures.end(); ++it)
         {
            if ((*it) == Choose)
            {
               if (mouse_state->getCtrl())
               {
                  figures.remove(Choose);
                  figures.push_front(Choose);
                  break;
               }

               figures.remove(Choose);
               figures.emplace(--it, Choose);
               break;
            }
         }
      }
      break;
   case ABAIXAR:
      if (Choose)
      {
         for (auto it = figures.begin(); it != --figures.end(); ++it)
         {
            if ((*it) == Choose)
            {
               if (mouse_state->getCtrl())
               {
                  figures.remove(Choose);
                  figures.push_back(Choose);
                  break;
               }

               auto aux = ++it;
               figures.remove(Choose);
               figures.emplace(++aux, Choose);
               break;
            }
         }
      }
      break;
   case DELETAR:
      if (Choose)
      {
         Drag = nullptr;
         figures.remove(Choose);
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
         float *rgb = button->get_rgb();
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

bool add_figure(int *figure)
{
   Figure *shp;
   switch (*figure)
   {
   case -1:
      return false;
   case QUADRADO:
      shp = new Square_figure(mouse_state->getX() - 50, mouse_state->getY() - 50, 100, 100);
      break;
   case TRIANGULO:
      shp = new Triangle_figure(mouse_state->getX() - 50, mouse_state->getY() - 50, 100, 86.0254037844386); // 86.0254037844386 : height of an equilateral triangle with side: 100
      break;
   case CIRCULO:
      shp = new Circle_figure(mouse_state->getX(), mouse_state->getY(), 50, 30);
      break;
   case POLIGONO:
      // If clicks on first button, finishes polygon
      if (newPolygon.size() > 0 &&
          Point::distance(newPolygon.front().x, newPolygon.front().y, mouse_state->getX(), mouse_state->getY()) <= RADIUS_BALL)
      {
         int n = newPolygon.size();

         if (n < 3)
            return false;

         float vx[n], vy[n];
         int i = 0;
         for (auto it = newPolygon.begin(); it != newPolygon.end(); ++it, i++)
         {
            vx[i] = (*it).x;
            vy[i] = (*it).y;
         }
         newPolygon.clear();
         shp = new Polygon_figure(vx, vy, n);
         break;
      }
      // Else, adds a new point
      newPolygon.push_back(Vector2(mouse_state->getX(), mouse_state->getY()));
      return true;
   default:
      std::cout << "Figura Inválida..." << std::endl;
      exit(1);
      break;
   }
   shp->color(0, 1, 0);
   figures.push_front(shp);
   Choose = shp;

   *figure = -1;
   click = true;
   return true;
}

/***********************************************************
*
* Canvas Functions
*
************************************************************/

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
   CV::clear(0, 0, 0);

   render_figures();

   panel->render();

   high_light();

   render_cursor_polygon();
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   // printf("\nTecla: %d", key);
   switch (key)
   {
   case 27: //finaliza programa
      save_file();
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

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);

   mouse_state->setX(x);
   mouse_state->setY(y);

   update();

   // Left click
   if (button == 0)
   {
      // Release
      if (state == 1)
      {
         // Releases Choosen figure
         if (!click)
         {
            Choose = Drag;
            Drag = nullptr;
         }
         click = false;
      }
      // Push
      else if (state == 0)
      {
         // Check clicks on Panel
         if (check_panel())
            return;

         // Checks click to add new figure
         if (add_figure(&New_Figure))
            return;

         // Checks click to drag figure
         if (Choose && Choose->checkUpdateFigure(*mouse_state))
         {
            Drag = Choose;
            return;
         }
         // Checks click to select figure
         for (auto it = figures.begin(); it != figures.end(); ++it)
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

   New_Figure = -1;

   mouse_state = new Mouse();

   create_panel();

   load_file();

   CV::run();
}
