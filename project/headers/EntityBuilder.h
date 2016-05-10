//
//  EntityBuilder.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_EntityBuilder_h
#define bRenderer_ios_EntityBuilder_h

#include "Entity.h"

class EntityBuilder {
    
    
private:
    
    class EntityProperties {
        
    private:
        
        EntityProperties() : isMoving(false),
        shaderName(""),
        objectName(""),
        facing(-1){
        }
        
        vmml::Vector3f scale;
        vmml::Vector3f translation;
        std::string shaderName;
        std::string objectName;
        Movement *movement;
        bool isMoving;
        bool setValue;
        int facing;
        
        friend class EntityBuilder;
    };
    
    EntityProperties eProperties;
    
public:
    
    EntityBuilder(){}
    
    ~EntityBuilder(){
        
    }
    
    EntityBuilder& setScale(vmml::Vector3f _scale) {
        eProperties.scale = _scale;
        return *this;
    }
    
    EntityBuilder& setTranslation(vmml::Vector3f _translation) {
        eProperties.translation = _translation;
        return *this;
    }
    
    EntityBuilder& setObjectName(std::string _objectName) {
        eProperties.objectName = _objectName;
        return *this;
    }
    
    EntityBuilder& setMovement(Movement *_movement) {
       eProperties.movement = _movement;
        return *this;
    }
    
    EntityBuilder& setIsMoving(bool _isMoving) {
        eProperties.isMoving = _isMoving;
        return *this;
    }
    
    //0 front, 1 left, 2 back, 3 right, -1 no rotation
    EntityBuilder& setFacing(int _facing) {
        eProperties.facing = _facing;
        return *this;
    }
    
    std::string getObjectName() {
        return eProperties.objectName;
    }
    
    bool isMoving() {
        return eProperties.isMoving;
    }
    
    EntityBuilder& setShaderName(std::string _shaderName) {
        eProperties.shaderName = _shaderName;
        return *this;
    }
    
    Entity createEntity() {
        if (eProperties.facing < 0) {
            Entity result(eProperties.scale,eProperties.translation,eProperties.shaderName,eProperties.objectName);
            return result;
        }
        if (eProperties.isMoving) {
            MoveableEntity result(eProperties.scale,eProperties.translation,vmml::Vector3f(0,1,0),M_PI_F*eProperties.facing*1/2,eProperties.shaderName,eProperties.objectName,eProperties.movement);
            return result;
        }
        Entity result(eProperties.scale,eProperties.translation,vmml::Vector3f(0,1,0),M_PI_F*eProperties.facing*1/2,eProperties.shaderName,eProperties.objectName);
        return result;
    }
    
    MoveableEntity createMoveableEntity() {
        MoveableEntity result(eProperties.scale,eProperties.translation,vmml::Vector3f(0,1,0),M_PI_F*eProperties.facing*1/2,eProperties.shaderName,eProperties.objectName,eProperties.movement);
        return result;
    }
};


#endif
