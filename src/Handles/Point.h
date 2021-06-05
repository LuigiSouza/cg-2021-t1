#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
// Define Infinite (Using INT_MAX caused overflow problems)
#define INF 10000

class Point
{
private:
   struct t_Point
   {
      float x;
      float y;
   };

   // Code extracted From:
   // https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/

   // Given three colinear points p, q, r, the function checks if
   // point q lies on line segment 'pr'
   static bool onSegment(t_Point p, t_Point q, t_Point r)
   {
      if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
          q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
         return true;
      return false;
   }

   // To find orientation of ordered triplet (p, q, r).
   // The function returns following values
   // 0 --> p, q and r are colinear
   // 1 --> Clockwise
   // 2 --> Counterclockwise
   static int orientation(t_Point p, t_Point q, t_Point r)
   {
      int val = (q.y - p.y) * (r.x - q.x) -
                (q.x - p.x) * (r.y - q.y);

      if (val == 0)
         return 0;              // colinear
      return (val > 0) ? 1 : 2; // clock or counterclock wise
   }

   // The function that returns true if line segment 'p1q1'
   // and 'p2q2' intersect.
   static bool doIntersect(t_Point p1, t_Point q1, t_Point p2, t_Point q2)
   {
      // Find the four orientations needed for general and
      // special cases
      int o1 = orientation(p1, q1, p2);
      int o2 = orientation(p1, q1, q2);
      int o3 = orientation(p2, q2, p1);
      int o4 = orientation(p2, q2, q1);

      // General case
      if (o1 != o2 && o3 != o4)
         return true;

      // Special Cases
      // p1, q1 and p2 are colinear and p2 lies on segment p1q1
      if (o1 == 0 && onSegment(p1, p2, q1))
         return true;

      // p1, q1 and p2 are colinear and q2 lies on segment p1q1
      if (o2 == 0 && onSegment(p1, q2, q1))
         return true;

      // p2, q2 and p1 are colinear and p1 lies on segment p2q2
      if (o3 == 0 && onSegment(p2, p1, q2))
         return true;

      // p2, q2 and q1 are colinear and q1 lies on segment p2q2
      if (o4 == 0 && onSegment(p2, q1, q2))
         return true;

      return false; // Doesn't fall in any of the above cases
   }

public:
   static bool isInside(float x, float y, int n, float vx[], float vy[])
   {
      // Create a point for line segment from p to infinite
      t_Point extreme = {2000, y};
      t_Point p = {x, y};

      // Count intersections of the above line with sides of polygon
      int count = 0, i = 0;
      do
      {
         int next = (i + 1) % n;

         // Check if the line segment from 'p' to 'extreme' intersects
         // with the line segment from 'polygon[i]' to 'polygon[next]'
         t_Point polygon_i = {vx[i], vy[i]};
         t_Point polygon_next = {vx[next], vy[next]};
         if (doIntersect(polygon_i, polygon_next, p, extreme))
         {
            // If the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon_i, p, polygon_next) == 0)
               return onSegment(polygon_i, p, polygon_next);

            count++;
         }
         i = next;
      } while (i != 0);

      return count & 1; // Same as (count%2 == 1);
   }
   static float distance(float x1, float y1, float x2, float y2)
   {
      return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
   }
};

#endif
