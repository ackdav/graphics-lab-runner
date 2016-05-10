//
//  Movement.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_Movement_h
#define bRenderer_ios_Movement_h

#include "bRenderer.h"

class Movement {
    
private:
    
public:

    virtual vmml::Vector<4, bool > GetMove() = 0;
    
};


#endif
