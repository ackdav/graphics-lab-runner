//
//  Controller.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 02/05/16.
//
//

#ifndef bRenderer_ios_Controller_h
#define bRenderer_ios_Controller_h

#include "GameController.h"
#include "bRenderer.h"
#include "Entity.h"
#include "Player.h"
#include "Ground.h"

class Controller {
private:
    std::list<Entity> entities;
    GameController gameController;
    Renderer brenderer;
    
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
    
    Controller(){
        
    }
    
    ~Controller(){
        
    }
    
    vmml::Vector3f getPlayerPosition() {
        return gameController.getPlayerPos();
    }
    
    void initialize(Renderer _brenderer) {
        brenderer = _brenderer;
        // TODO: Matrix to initialize objects
        Ground ground;
        entities.push_back(ground);
        for( int a = 1; a < 100; a = a + 1 )
        {
            Ground ground(vmml::Vector3f(a*2.0f,0.0f,0.0f));
            entities.push_back(ground);
            Ground ground2(vmml::Vector3f(a*2.0f*-1,0.0f,0.0f));
            entities.push_back(ground2);
        }
        gameController.initialize(_brenderer);
    }
    
    void update(double elapsedTime) {
        std::list<Entity>::iterator iterator;
        for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
            //position = position * vmml::create_translation(vmml::Vector3f(elapsedTime,0,0));
            drawEntity(*iterator);
        }
        gameController.update(elapsedTime);
    }
    
    
};

#endif
