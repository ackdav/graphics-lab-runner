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
    Player(vmml::Vector3f &scale):Entity(scale){}

    
    
    
    
};


#endif
