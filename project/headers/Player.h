//
//  Player.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 02/05/16.
//
//

#ifndef bRenderer_ios_Player_h
#define bRenderer_ios_Player_h

#include "Entity.h"

class Player: public Entity{
    
private:
    
public:
    
    Player(vmml::Matrix4f &Mat):Entity(Mat){}
    Player():Entity(vmml::Vector3f(0.3f),vmml::Vector3f(-100.0f,1.0f,0.0f),vmml::Vector3f(0,1,0),M_PI_F*3/2,"guy","minecraftcharacter"){}
    
};


#endif
