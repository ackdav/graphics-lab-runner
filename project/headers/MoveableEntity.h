//
//  MoveableEntity.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_MoveableEntity_h
#define bRenderer_ios_MoveableEntity_h

#include "Entity.h"
#include "Movement.h"

class MoveableEntity: public Entity{
    
private:
    
    Movement *movement;
    
public:
    
    MoveableEntity(){} 
    
    MoveableEntity(vmml::Vector3f _scale,vmml::Vector3f _translation, std::string _shaderName, std::string _objName, bool _isCollectible, bool _collision2D, Movement *_movement):Entity(_scale,_translation,_shaderName,_objName,_isCollectible, _collision2D){
        movement = _movement;
    }
    
    Movement* getMovement() {
        return movement;
    }
    
    bool isMoving() {
        return true;
    }
    
    
    
    
};

#endif
