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
    
    vmml::Vector3f translation;
    vmml::Matrix4f _startPos;
    vmml::Matrix4f _currentPos;
    std::string shaderName;
    std::string objName;
    
public:
    
    
    void setPos(vmml::Matrix4f &Mat){
        _currentPos = Mat;
    }
    
    vmml::Matrix4f getPos(){
        return _currentPos;
    }
    
    vmml::Vector3f getTranslate() {
        return translation;
    }
    
    
    void move(vmml::Vector3f _translation) {
        translation = _translation;
        _currentPos = vmml::create_translation(translation) * _startPos;
    }
    
    std::string getShaderName() {
        return shaderName;
    }
    
    std::string getObjName() {
        return objName;
    }
    
    Entity(vmml::Vector3f scale,vmml::Vector3f _translation, std::string _shaderName, std::string _objName):translation(_translation),shaderName(_shaderName),objName(_objName){
        _startPos = vmml::create_scaling(scale);
        _currentPos= vmml::create_translation(_translation) * _startPos;
        
    }
    
    Entity(vmml::Vector3f scale,vmml::Vector3f _translation,vmml::Vector3f axis,float angle, std::string _shaderName, std::string _objName):translation(_translation),shaderName(_shaderName),objName(_objName){
        _startPos =  vmml::create_rotation(angle, axis) *vmml::create_scaling(scale);
        _currentPos= vmml::create_translation(_translation) * _startPos;
        
    }
    
    Entity(vmml::Matrix4f &Pos){
        _currentPos = Pos;
    }
    
    Entity(){}
    
    
    
    
    
};




#endif
