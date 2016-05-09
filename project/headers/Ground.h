//
//  Ground.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 02/05/16.
//
//

#ifndef bRenderer_ios_Ground_h
#define bRenderer_ios_Ground_h

#include "Entity.h"

class Ground: public Entity{
    
private:
    
public:
    
    Ground(vmml::Vector3f(translate)):Entity(vmml::Vector3f(1.0f),vmml::Vector3f(0.0f,-3.0f,0.0f) + translate,"guy","block"){}
    
    Ground():Entity(vmml::Vector3f(1.0f),vmml::Vector3f(0.0f,-3.0f,0.0f),"guy","block"){}
    
    
};


#endif
