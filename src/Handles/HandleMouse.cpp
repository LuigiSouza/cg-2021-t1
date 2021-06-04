
#include "HandleMouse.h"

Mouse::Mouse(void)
{
   for (int i = 0; i < 3; i++)
   {
      prev_button[i] = false;
      button[i] = false;
   }
}

void Mouse::update(int index)
{
   Mouse::prev_button[index] = Mouse::button[index];
   Mouse::button[index] = !Mouse::button[index];
}

bool Mouse::isDown(int index)
{
   return Mouse::button[index];
}

bool Mouse::clicked(int index)
{
   bool ret = !Mouse::button[index] && Mouse::prev_button[index];
   if (ret)
      Mouse::prev_button[index] = Mouse::button[index];
   return ret;
}

void Mouse::setX(float x)
{
   this->prev_pos_x = this->pos_x;
   this->pos_x = x;
}
void Mouse::setY(float y)
{
   this->prev_pos_y = this->pos_y;
   this->pos_y = y;
}
float Mouse::getX() { return this->pos_x; }
float Mouse::getY() { return this->pos_y; }
float Mouse::moveX() { return this->pos_x - this->prev_pos_x; }
float Mouse::moveY() { return this->pos_y - this->prev_pos_y; }
