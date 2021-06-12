#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include "Vector2.h"

class Point
{
public:
   static Vector2 perpendicular(Vector2 vec1, Vector2 vec2, float px, float py)
   {
      float k = ((vec2.y - vec1.y) * (px - vec1.x) - (vec2.x - vec1.x) * (py - vec1.y)) / ((vec2.y - vec1.y) * (vec2.y - vec1.y) + (vec2.x - vec1.x) * (vec2.x - vec1.x) * 1.0);

      return Vector2(px - k * (vec2.y - vec1.y) * 1.0,
                     py + k * (vec2.x - vec1.x) * 1.0);
   }

   static Vector2 rotate(float x, float y, float angle)
   {
      return Vector2(x * cos(angle) - y * sin(angle),
                     x * sin(angle) + y * cos(angle));
   }

   static float getAngle(float x1, float y1, float x2, float y2, float base_x, float base_y)
   {
      float vectorA_x = x1 - base_x;
      float vectorA_y = y1 - base_y;
      float vectorB_x = x2 - base_x;
      float vectorB_y = y2 - base_y;

      float dot_product = vectorA_x * vectorB_x + vectorA_y * vectorB_y * 1.0;
      float croos_product = vectorA_x * vectorB_y - vectorA_y * vectorB_x * 1.0;
      float angle = atan2(fabs(croos_product), dot_product);

      return croos_product < 0 ? angle * -1.0 : angle;
   }

   /*
    * PNPOLY - Point Inclusion in Polygon Test
    * W. Randolph Franklin (WRF)
    * From: https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
   */
   static bool isInside(float x, float y, int n, float vx[], float vy[])
   {
      bool c = false;
      for (int i = 0, j = n-1; i < n; j = i++) {
        if ((((vy[i] <= y) && (y < vy[j])) ||
             ((vy[j] <= y) && (y < vy[i]))) &&
            (x < (vx[j] - vx[i]) * (y - vy[i]) / (vy[j] - vy[i]) + vx[i]))
          c = !c;
      }
      return c;
   }
   
   static float distance(float x1, float y1, float x2, float y2)
   {
      return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
   }
};

#endif
