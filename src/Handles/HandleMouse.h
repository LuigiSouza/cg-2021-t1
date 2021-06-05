
#ifndef ___MOUSE__H___
#define ___MOUSE__H___

class Mouse
{
private:
    bool prev_button[3];
    bool button[3];
    float pos_x;
    float pos_y;
    float prev_pos_x;
    float prev_pos_y;
    bool ctrl = false;

public:
    Mouse(void);
    bool isDown(int index);
    bool clicked(int index);
    void update(int index);
    void setX(float x);
    void setY(float y);
    float getX(void);
    float getY(void);
    float moveX(void);
    float moveY(void);
    void setCtrl(bool ctrl);
    bool getCtrl(void);
};

#endif
