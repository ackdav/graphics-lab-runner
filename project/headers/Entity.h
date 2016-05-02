//
//  Entity.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 02/05/16.
//
//



#ifndef bRenderer_ios_Entity_h
#define bRenderer_ios_Entity_h

#include "bRenderer.h"


class Entity{

    
private:
    
    vmml::Matrix4f _currentPos;
    
public:
    

    void setPos(vmml::Matrix4f &Mat){
        
        _currentPos = Mat;
        
        
    }
    
    vmml::Matrix4f getPos(){
        
        return _currentPos;
        
    }
    
    Entity(vmml::Vector3f &scale){
        
        _currentPos=vmml::create_scaling(scale);
        
        
    }
    
    Entity(vmml::Matrix4f &Pos){
        
        _currentPos = Pos;
        
    }
    
    Entity(){}
    
    
    
    
    
};




#endif
