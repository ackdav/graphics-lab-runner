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
    vmml::Matrix4f viewMatrix;
    std::string shaderName;
    std::string objName;
    std::string image;
    
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
    
    bool isMoving() {
        return false;
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
    
    vmml::Matrix4f getViewMatrix() {
        return viewMatrix;
    }
    
    Entity(vmml::Vector3f scale,vmml::Vector3f _translation, std::string _shaderName, std::string _objName,std::string _image,vmml::Matrix4f _viewMatrix):translation(_translation),shaderName(_shaderName),objName(_objName),image(_image),viewMatrix(_viewMatrix){
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