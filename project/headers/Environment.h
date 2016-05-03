//
//  Environment.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 03/05/16.
//
//

#ifndef bRenderer_ios_Environment_h
#define bRenderer_ios_Environment_h

#include "Entity.h"

class Environment: public Entity{
    
private:
    
public:
    
    Environment(vmml::Vector3f(translate)):Entity(vmml::Vector3f(1.0f),vmml::Vector3f(0.0f,-3.0f,0.0f) + translate,"guy","clouds"){}
    
    Environment():Entity(vmml::Vector3f(1.0f),vmml::Vector3f(0.0f,-3.0f,0.0f),"guy","clouds"){}
    
};


#endif
