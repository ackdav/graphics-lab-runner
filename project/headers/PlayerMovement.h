//
//  PlayerMovement.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_PlayerMovement_h
#define bRenderer_ios_PlayerMovement_h

#include "Movement.h"


class PlayerMovement: public Movement {
private:
   //Movement *_movement
public:
    
    PlayerMovement(){}

    vmml::Vector<4, bool > GetMove() {
        return vmml::Vector<4,bool>(false,false,false,false);
    }
};
#endif
