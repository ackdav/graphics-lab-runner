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
    
    float getCurrentXPos() {
        return _currentPos.get_column(3).x();
    }
    
    vmml::Vector3f getTranslate() {
        return translation;
    }
    void setTranslate(vmml::Vector3f translate){
        translation = translate;
    }
    
    
    void move(vmml::Vector3f _translation) {
        //        std::cout << "current translate = " << getTranslate()+_translation;
        translation =  getTranslate() +_translation  ;
        //        _currentPos = vmml::create_translation(translation) * _startPos;
        //        std::cout << "\n start pos: \n" << _startPos << "\n current Pos :  \n" << _currentPos;
        _currentPos = vmml::create_translation(translation) *  _startPos;
        //        setTranslate(translation);
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
        translation = vmml::Vector3f(0.f,0.f,0.f);
    }
    
    Entity(vmml::Vector3f scale,vmml::Vector3f _translation,vmml::Vector3f axis,float angle, std::string _shaderName, std::string _objName):translation(_translation),shaderName(_shaderName),objName(_objName){
        _startPos =  vmml::create_rotation(angle, axis) *vmml::create_scaling(scale);
        _currentPos= vmml::create_translation(_translation) * _startPos;
        translation = vmml::Vector3f(0.f,0.f,0.f);
        
    }
    
    Entity(vmml::Matrix4f &Pos){
        _currentPos = Pos;
        translation = vmml::Vector3f(0.f,0.f,0.f);
    }
    
    Entity(){}
    
};




#endif