
#include "HandleMouse.hpp"

Mouse::Mouse(void)
{
   this->prev_pos_x = this->pos_x = 0;
   this->prev_pos_y = this->pos_y = 0;
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
float Mouse::getX(void) { return this->pos_x; }
float Mouse::getY(void) { return this->pos_y; }
float Mouse::moveX(void) { return this->pos_x - this->prev_pos_x; }
float Mouse::moveY(void) { return this->pos_y - this->prev_pos_y; }

void Mouse::setCtrl(bool ctrl) { this->ctrl = ctrl; }
bool Mouse::getCtrl(void) { return this->ctrl; }
