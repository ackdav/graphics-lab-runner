//
//  GameController.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 02/05/16.
//
//

#ifndef bRenderer_ios_GameController_h
#define bRenderer_ios_GameController_h

#include <list>

#include "bRenderer.h"
#include "Entity.h"
#include "Player.h"
#include "Environment.h"
#include "Background.h"
#include "GameObject.h"

class GameController {
    
    
private:
    std::list<Entity> entities;
    Entity player;
    Renderer brenderer;
    vmml::Vector3f trans;
    vmml::Matrix4f CurrentIteratorPos;
    float ycoordinate = 0.f;

    
    void drawEntity(Entity entity) {
        vmml::Matrix4f modelMatrix = entity.getPos();
        std::string shaderName = entity.getShaderName();
        std::string objName = entity.getObjName();
        
        vmml::Matrix4f viewMatrix = brenderer.getObjects()->getCamera("camera")->getViewMatrix();
        
        ShaderPtr shader = brenderer.getObjects()->getShader(shaderName);
        
        if (shader.get())
        {
            shader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
            shader->setUniform("ViewMatrix", viewMatrix);
            shader->setUniform("ModelMatrix", modelMatrix);
            
            vmml::Matrix3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
            
            vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
            shader->setUniform("EyePos", eyePos);
            
            shader->setUniform("LightPos", vmml::Vector4f(1.0, 1.0, 10.0, 1.0));
            shader->setUniform("Ia", vmml::Vector3f(1.f));
            shader->setUniform("Id", vmml::Vector3f(1.f));
            shader->setUniform("Is", vmml::Vector3f(1.f));
        }
        else
        {
            bRenderer::log("No shader available.");
        }
        
        brenderer.getModelRenderer()->drawModel(objName, "camera", modelMatrix, std::vector<std::string>({ }));
        
    }
    
    
public:
    
    GameController(){}
    
    ~GameController(){
        
    }
    
    Entity getPlayer() {
        return player;
    }
    
    vmml::Vector3f getPlayerPos() {
        return trans;
    }
    
    //Initialize logic elements
    void initialize(Renderer _brenderer) {
        brenderer = _brenderer;
        // TODO: Matrix to initialize objects
        entities.push_back(player);
    }
    
    vmml::Vector3f getInput(double elapsedTime, int direction, bool jumpable){
        std::list<Entity>::iterator iterator;
//        ycoordinate = (float)entities.begin()->getPos().y();
        float xVec = 0.f;
        float yVec = 0.f;
        float zVec = 0.f;
        
        if(direction==1){
            //            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
//            iterator = entities.begin();
//            if (ycoordinate - 0.03 < -1.f ){
//                ycoordinate = 0.f;
//            }
//            else {
//                ycoordinate =  - 0.03f;
//            }
//            
//            iterator->move(vmml::Vector3f(elapsedTime,ycoordinate,0.f));
            xVec += 0.05;
            
            //                trans = iterator->getPos();
            //                trans = vmml::Vector3f(-elapsedTime,0,0);
//            trans = vmml::Vector3f( -iterator->getTranslate().x(), 0.f,0.f);
//            
            //                if(iterator.collidesWith(plane2)){
            //                    modelMatrix = vmml::create_translation(vmml::Vector3f(-1.0f, 0.0f, 0.0f)) * modelMatrix;
            //                    std::cerr << "COLLISION DETECTED" <<  std::endl;
            //                }
            
//            drawEntity(*iterator);
            //            }
        }
        
        if(direction==2){
            xVec -= 0.05;
            if (jumpable == true){
                yVec += 0.9;}
            //            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
//            iterator = entities.begin();
//            
//            
//            if (ycoordinate - 0.03 < -1.f ){
//                ycoordinate = 0.f;
//            }
//            else {
//                ycoordinate =  - 0.03f;
//            }
//            
//            iterator->move(vmml::Vector3f(-elapsedTime,ycoordinate,0));
//            
//            trans = vmml::Vector3f(-iterator->getTranslate().x(), 0.f,0.f);
//            
//            //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
//            drawEntity(*iterator);
            //            }
        }
        
//        if(direction==5){
//            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
//                
//                if (ycoordinate - 0.03 < -1.f ){
//                    std::cout << "\n ddd" << ycoordinate;
//                    ycoordinate = 0.f;
//                }
//                else {
//                    ycoordinate =  - 0.03f;
//                }
//                
//                iterator->move(vmml::Vector3f(0.0f,ycoordinate,0.f));
//                drawEntity(*iterator);
//            }
        return vmml::Vector3f(xVec,yVec,zVec);
    };;
//    void update(double elapsedTime, int direction) {
//           }
    };


/*             void update(double elapsedTime) {
 std::list<Entity>::iterator iterator;
 for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
 iterator->move(vmml::Vector3f(elapsedTime,0,0));
 trans = vmml::Vector3f(-elapsedTime,0,0);
 //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
 drawEntity(*iterator);
 }
 
 */


#endif
