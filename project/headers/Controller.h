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
#include "Environment.h"
#include "Background.h"

class Controller {
private:
    std::list<Entity> entities;
    std::list<Entity> moveableEntities;
    GameController gameController;
    Renderer brenderer;
    Player player;
    bool jumpable;
    
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
        //         std::cout << "object " << objName << brenderer.getObjects()->getModel("block")->getBoundingBoxObjectSpace();
        
    }
    
    
public:
    
    Controller(){
        
    }
    
    ~Controller(){
        
    }
    
    vmml::Vector3f getPlayerPosition() {
        std::cout<<"POS"<<vmml::Vector3f(moveableEntities.begin()->getCurrentXPos(), 0.f,0.f)<<std::endl;
        return vmml::Vector3f(-moveableEntities.begin()->getCurrentXPos(), 0.f,0.f);
    }
    
    void initialize(Renderer _brenderer) {
        brenderer = _brenderer;
        // TODO: Matrix to initialize objects
        Ground ground;
        entities.push_back(ground);
        jumpable = false;
        
        for( int a = 1; a < 100; a = a + 1 )
        {
            Ground ground(vmml::Vector3f(a*2.0f,0.0f,0.0f));
            entities.push_back(ground);
            Ground ground2(vmml::Vector3f(a*2.0f*-1,0.0f,0.0f));
            entities.push_back(ground2);
            
        }
        
        Environment cloud(vmml::Vector3f(5,5,0));
        entities.push_back(cloud);
        
        Background background(vmml::Vector3f(0,0,-50));
        entities.push_back(background);
        
        moveableEntities.push_back(player);
        
    }
    
    void update(double elapsedTime, int direction) {
        std::list<Entity>::iterator iterator;
        std::list<Entity>::iterator moveableIterator;
        for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
            drawEntity(*iterator);
        }
        // Move default -0.15 to floor
        float gravity = -0.15;
        for (moveableIterator = moveableEntities.begin(); moveableIterator != moveableEntities.end(); ++moveableIterator) {
            //get userInput and add gravity - Todo: Check if user is on ground -> he can jump, else: not.
            //jumpable = bool
            vmml::Vector3f user = gameController.getInput(elapsedTime, direction, jumpable);
            moveableIterator->move(vmml::Vector3f(0.f,gravity,0.f) + user);
            //?
            vmml::Vector4f pos = moveableIterator->getPos().get_column(3);
            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
                
                vmml::AABBf boundingBox2 = brenderer.getObjects()->getModel(iterator->getObjName())->getBoundingBoxObjectSpace();
                vmml::AABBf box2(iterator->getPos() * boundingBox2.getMin(),iterator->getPos() * boundingBox2.getMax());
                
                // Upon collision, move 0.01 up, until no collision is detected (still need to check with all other objects for collision, therefore no break)
                while (box2.isIn(vmml::Vector3f(pos.x(),pos.y(),pos.z()))) {
                    moveableIterator->move(vmml::Vector3f(0,0.001f,0.f));
                    pos = moveableIterator->getPos().get_column(3);
                }

                
            }
            drawEntity(*moveableIterator);
        }
        
        
        
        
        
        
        //gameController.update(elapsedTime,direction);
        
    }
    
    
};

#endif