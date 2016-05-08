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


class GameController {
    
    
private:
    std::list<Entity> entities;
    Player player;
    Renderer brenderer;
    vmml::Vector3f trans;
    vmml::Matrix4f CurrentIteratorPos;
    
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
            
            shader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
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
    
    
    void update(double elapsedTime, int direction) {
        std::list<Entity>::iterator iterator;

        
        if(direction==1){
            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
                iterator->move(vmml::Vector3f(elapsedTime,-elapsedTime,0));
//                trans = iterator->getPos();
//                trans = vmml::Vector3f(-elapsedTime,0,0);
                trans = vmml::Vector3f( -iterator->getTranslate().x(), 0.f,0.f);

                //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
                drawEntity(*iterator);
            }
        }
        
        if(direction==2){
            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
                
                iterator->move(vmml::Vector3f(-elapsedTime,-elapsedTime,0));
                
                trans = vmml::Vector3f(-iterator->getTranslate().x(), 0.f,0.f);

                //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
                drawEntity(*iterator);
            }
        }
        
        if(direction==5){
            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
                
                //iterator->move(vmml::Vector3f(0,0,0));
                //trans = vmml::Vector3f(-0,0,0);
                //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
                CurrentIteratorPos = iterator->getPos();
                CurrentIteratorPos = vmml::create_translation(vmml::Vector3f(0,-0.5,0))*CurrentIteratorPos;
                iterator->setPos(CurrentIteratorPos);
                drawEntity(*iterator);
            }
        }

        std::cout << "CONTROLLER GET PLAYER POSITION: " << trans;

        
        
    }
    

    
   /*             void update(double elapsedTime) {
                    std::list<Entity>::iterator iterator;
                    for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
                        iterator->move(vmml::Vector3f(elapsedTime,0,0));
                        trans = vmml::Vector3f(-elapsedTime,0,0);
                        //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
                        drawEntity(*iterator);
                    }
                    
     */
       

    };



#endif
