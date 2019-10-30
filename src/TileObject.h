#ifndef __Player_h__
#define __Player_h__

#include <iostream>
#include <vector>
#include "Object2D.h"
#include "GameObjectFactory.h"

class TileObject : public Object2D
{
public:
    
    TileObject();
    ~TileObject() {}
    
    void load();
    
    void draw();
    void update();
    void clean();
    
    void collision();
    
    std::string type() { return "TileObject"; }
    
private:
    
    // handle any input from the keyboard, mouse, or joystick
    //void handleInput();
    
    // handle any animation for the player
    //void handleAnimation();

};

// for the factory
class TileObjectCreator : public BaseCreator 
{
    Object2D* createGameObject() const{
        return new TileObject();
    }

    
};

#endif 
