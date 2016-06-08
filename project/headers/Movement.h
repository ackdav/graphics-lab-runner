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
    
    double durationFlying;
    
public:

    virtual vmml::Vector<4, bool > getMove() = 0;
    
    void setDurationFlying(double _durationFlying) {
        durationFlying = _durationFlying;
    }
    
    double getDurationFlying() {
        return durationFlying;
    }
    
};


#endif
