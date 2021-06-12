
#ifndef ___MOUSE__H___
#define ___MOUSE__H___

class Mouse
{
private:
    float pos_x;
    float pos_y;

    float prev_pos_x;
    float prev_pos_y;
    
    bool ctrl = false;

public:
    Mouse(void);
    // setters
    void setX(float x);
    void setY(float y);
    // getters
    float getX(void);
    float getY(void);
    // get move from ouse
    float moveX(void);
    float moveY(void);

    void setCtrl(bool ctrl);
    bool getCtrl(void);
};

#endif
