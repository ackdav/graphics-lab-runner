//
//  Controller.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 02/05/16.
//
//

#ifndef bRenderer_ios_Controller_h
#define bRenderer_ios_Controller_h

#include <iostream>
#include <fstream>
#include <string>

#include "GameController.h"
#include "bRenderer.h"
#include "Entity.h"
#include "Player.h"
#include "LevelBuilder.h"


class Controller {
private:
    std::list<Entity> entities;
    std::list<MoveableEntity> moveableEntities;
    std::list<Entity> buttons;
    
    GameController gameController;
    Renderer brenderer;
    Entity player;
    Entity skyplane;
    bool jumpable;
    double timeSinceLast;
    
    void drawEntity(Entity entity) {
        vmml::Matrix4f modelMatrix = entity.getPos();
        std::string shaderName = entity.getShaderName();
        std::string objName = entity.getObjName();
        std::cout<<"SHADER: "<<shaderName<<std::endl;
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
            
            vmml::Vector4f eyePos = vmml::Vector4f(-brenderer.getObjects()->getCamera("camera")->getPosition().x()-16.5f,brenderer.getObjects()->getCamera("camera")->getPosition().y()+22.f,brenderer.getObjects()->getCamera("camera")->getPosition().z(), 1.0f);
//            vmml::Vector4f eyePos = vmml::Vector4f(brenderer.getObjects()->getCamera("camera")->getPosition().x(),brenderer.getObjects()->getCamera("camera")->getPosition().y(),brenderer.getObjects()->getCamera("camera")->getPosition().z(), 1.0f);
            
            shader->setUniform("EyePos", eyePos);
            
            shader->setUniform("LightPos", -eyePos);
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
    
    vmml::Vector<4,bool> checkCollision(vmml::Vector3f oldCenter, vmml::Vector3f newCenter, vmml::Vector3f boxMin,vmml::Vector3f boxMax, vmml::Vector3f posMin, vmml::Vector3f posMax) {
        bool left= false;
        bool right = false;
        bool top = false;
        bool bottom = false;
        bool yCollision = boxMax.at(1) > posMin.at(1) && boxMin.at(1) < posMax.at(1);
        bool xCollision = boxMax.at(0) > posMin.at(0);
        if (yCollision) {
            if (oldCenter.at(1) > newCenter.at(1)) {
                top = true;
            } else {
                bottom = true;
            }
        }
        if (xCollision) {
            if (oldCenter.at(0) > newCenter.at(0)) {
                right = true;
            } else {
                left = true;
            }
        }
        return vmml::Vector<4,bool>(left,right,top,bottom);
    }
    
    bool isIn(vmml::Vector3f oldCenter, vmml::Vector3f newCenter, vmml::Vector3f boxMin,vmml::Vector3f boxMax, vmml::Vector3f posMin, vmml::Vector3f posMax) {
        float newwidth = posMax.at(0) - posMin.at(0);
        float oldwidth = boxMax.at(0) - boxMin.at(0);
        float newheight = posMax.at(1) - posMin.at(1);
        float oldheight = boxMax.at(1) - boxMin.at(1);
        return oldCenter.at(0) < newCenter.at(0) + newwidth && oldCenter.at(0) + oldwidth > newCenter.at(0) && oldCenter.at(1) < newCenter.at(1) + newheight && oldheight + oldCenter.at(1) > newCenter.at(1);
    }
    
    bool hasCollision(vmml::Vector<4,bool> collision) {
        return collision.at(0) || collision.at(1) || collision.at(2) || collision.at(3);
    }
    
    vmml::Vector3f getMax(vmml::Vector3f a,vmml::Vector3f b) {
        return vmml::Vector3f(std::max(a.at(0),b.at(0)),std::max(a.at(1),b.at(1)),std::max(a.at(2),b.at(2)));
    }
    
    vmml::Vector3f getMin(vmml::Vector3f a,vmml::Vector3f b) {
        return vmml::Vector3f(std::min(a.at(0),b.at(0)),std::min(a.at(1),b.at(1)),std::min(a.at(2),b.at(2)));
    }
    
    
    
    vmml::Vector3f getCollision(vmml::AABBf oldbox,vmml::AABBf newbox) {
        vmml::Vector3f movedBy = newbox.getCenter() - oldbox.getCenter();
        std::list<Entity>::iterator iterator;
        float xtrans = 0;
        float ytrans = 0;
        for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
            
            vmml::AABBf boundingBox2 = brenderer.getObjects()->getModel(iterator->getObjName())->getBoundingBoxObjectSpace();
            vmml::AABBf box2(iterator->getPos() * boundingBox2.getMin(),iterator->getPos() * boundingBox2.getMax());
            vmml::Vector3f boxMax = getMax(box2.getMax(),box2.getMin());
            vmml::Vector3f boxMin = getMin(box2.getMax(),box2.getMin());
            vmml::Vector3f posMax = getMax(newbox.getMax(),newbox.getMin());
            vmml::Vector3f posMin = getMin(newbox.getMax(),newbox.getMin());
            // Upon collision, move 0.01 up, until no collision is detected (still need to check with all other objects for collision, therefore no break)
            if (isIn(box2.getCenter(),newbox.getCenter(),boxMin,boxMax,posMin,posMax)){
                vmml::Vector<4,bool> collision = checkCollision(oldbox.getCenter(),newbox.getCenter(),boxMin,boxMax,posMin,posMax );
                if (collision.at(2)) {
                    ytrans = box2.getMax().at(1) - newbox.getMin().at(1)+0.001;
                }
                if (collision.at(1)) {
                    xtrans = box2.getMax().at(0)-newbox.getMax().at(0)+ 0.001;
                }
                if (collision.at(0)) {
                    xtrans = box2.getMin().at(0)-newbox.getMin().at(0)-0.001;
                }

            }
        }
        if (std::abs(movedBy.at(0)) > std::abs(movedBy.at(1))) {
            return vmml::Vector3f(xtrans,0.0f,0.0f);
        }
        return vmml::Vector3f(0.0f,ytrans,0.0f);
    }
    
    
    
    
    
    
public:
    
    Controller():timeSinceLast(0){
        
    }
    
    ~Controller(){
        
    }
    
    vmml::Vector3f getPlayerPosition() {
//        std::cout<<"POS"<<vmml::Vector3f(moveableEntities.begin()->getCurrentXPos(), 0.f,0.f)<<std::endl;
        return vmml::Vector3f(-moveableEntities.begin()->getCurrentXPos(), 0.f,0.f);
    }
    
    void initialize(Renderer _brenderer) {
        brenderer = _brenderer;
        // TODO: Matrix to initialize objects
        LevelBuilder levelBuilder(_brenderer);
        
        entities = levelBuilder.getEntities();
        moveableEntities = levelBuilder.getMoveableEntities();
        player = levelBuilder.getPlayer();
        skyplane = levelBuilder.getSkyplane();
        buttons = levelBuilder.getButtons();
    }
    
    void update(double elapsedTime, int direction) {
        timeSinceLast = elapsedTime - timeSinceLast;
        std::list<Entity>::iterator iterator;
        std::list<MoveableEntity>::iterator moveableIterator;
        std::list<Entity>::iterator buttonIterator;
        
        drawEntity(skyplane);
        
        for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
            std::cout<<iterator->getTranslate()<<std::endl;
            drawEntity(*iterator);
        }
        // Move default -0.9 to floor
        float gravity = -0.15f;
        for (moveableIterator = moveableEntities.begin(); moveableIterator != moveableEntities.end(); ++moveableIterator) {
            Movement *movement = moveableIterator->getMovement();
            
            vmml::Vector<4, bool > move = movement->getMove();
            float x = 0.0f;
            float y = 0.0f;
            if (move.at(0)) {
                x -= 0.1f;
            }
            if (move.at(1)) {
                x += 0.1f;
            }
            if (move.at(2)) {
                y += 0.1f;
            } else {
                y += gravity;
            }
            vmml::AABBf boundingBox = brenderer.getObjects()->getModel(moveableIterator->getObjName())->getBoundingBoxObjectSpace();
            vmml::AABBf oldbox(moveableIterator->getPos() * boundingBox.getMin(),moveableIterator->getPos() * boundingBox.getMax());
            vmml::Vector3f oldPosition = moveableIterator->getTranslate();
            moveableIterator->move(vmml::Vector3f(x,y,0.f));
            vmml::Vector3f newPosition = moveableIterator->getTranslate();
            vmml::AABBf newbox(moveableIterator->getPos() * boundingBox.getMin(),moveableIterator->getPos() * boundingBox.getMax());
            vmml::Vector3f col = getCollision(oldbox,newbox);
            if (col.at(0) != 0 || col.at(1) != 0) {
                movement->setDurationFlying(0);
            } else {
                movement->setDurationFlying(movement->getDurationFlying() + timeSinceLast);
            }
            while(col.at(0) != 0 || col.at(1) != 0) {
                moveableIterator->move(col);
                newbox.set(moveableIterator->getPos() * boundingBox.getMin(),moveableIterator->getPos() * boundingBox.getMax());
                col = getCollision(oldbox,newbox);
            }
            
            
            //?
//            vmml::Vector4f pos = moveableIterator->getPos().get_column(3);
//            for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
//                
//                vmml::AABBf boundingBox2 = brenderer.getObjects()->getModel(iterator->getObjName())->getBoundingBoxObjectSpace();
//                vmml::AABBf box2(iterator->getPos() * boundingBox2.getMin(),iterator->getPos() * boundingBox2.getMax());
//                
//                vmml::Vector<4,bool> collision = checkCollision(box2,oldbox,newbox);
//                // Upon collision, move 0.01 up, until no collision is detected (still need to check with all other objects for collision, therefore no break)
//                if (box2.isIn2d(newbox.getMin()) || box2.isIn2d(newbox.getMax())){
//                    vmml::Vector<4,bool> collision = checkCollision(box2,oldbox,newbox);
//                    std::cout<<"COLLISION BEFORE "<<collision<<std::endl;
//                    float xtrans = 0;
//                    float ytrans = 0;
//                    if (collision.at(2)) {
//                        ytrans = box2.getMax().at(1) - newbox.getMin().at(1);
//                    }
//                    if (collision.at(0)) {
//                        xtrans = newbox.getMax().at(0) - box2.getMin().at(0);
//                    }
//                    if (xtrans != 0 || ytrans != 0) {
//                        moveableIterator->move(vmml::Vector3f(xtrans,ytrans,0.0f));
//                    }
//                    newbox.set(moveableIterator->getPos() * boundingBox.getMin(),moveableIterator->getPos() * boundingBox.getMax());
//                    collision = checkCollision(box2,oldbox,newbox);
//                    std::cout<<"COLLISION AFTER "<<collision<<std::endl;
//                }
//
//                
//            }
            drawEntity(*moveableIterator);
        }
        
        
        for (buttonIterator = buttons.begin(); buttonIterator != buttons.end(); ++buttonIterator) {
            brenderer.getModelRenderer()->drawModel(brenderer.getObjects()->getModel(buttonIterator->getObjName()), buttonIterator->getPos(), buttonIterator->getViewMatrix(), vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false);
        }
        

        
        
        //gameController.update(elapsedTime,direction);
        
    }
    
    
};

#endif