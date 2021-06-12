#ifndef __POINT_H__
#define __POINT_H__

#include <cmath>
#include <iostream>

#include "Vector2.h"

// Auxiliar class to make calculus between points
class Point
{
public:
   static Vector2 perpendicular(Vector2 vec1, Vector2 vec2, float px, float py);

   static Vector2 rotate(float x, float y, float angle);

   static float getAngle(float x1, float y1, float x2, float y2, float base_x, float base_y);

   static float distance(float x1, float y1, float x2, float y2);

   /*
    * PNPOLY - Point Inclusion in Polygon Test
    * W. Randolph Franklin (WRF)
    * From: https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
   */
   static bool isInside(float x, float y, int n, float vx[], float vy[]);
};

#endif
