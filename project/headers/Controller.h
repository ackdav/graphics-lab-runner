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
    
    vmml::Vector<4,bool> checkCollision(vmml::AABBf box, vmml::AABBf oldPosition,vmml::AABBf newPosition) {
        vmml::Vector3f movedBy = newPosition.getCenter() - oldPosition.getCenter();
        vmml::Vector3f boxMax = box.getMax();
        vmml::Vector3f boxMin = box.getMin();
        vmml::Vector3f posMax = newPosition.getMax();
        vmml::Vector3f posMin = newPosition.getMin();
        bool left= false;
        bool right = false;
        bool top = false;
        bool bottom = false;
        bool yCollision = boxMax.at(1) > posMin.at(1) && boxMin.at(1) < posMax.at(1);
        bool xCollision = boxMin.at(0) < posMin.at(0) && boxMax.at(0) > posMax.at(0);
        std::cout<<"BOX MAX: "<<boxMax<<std::endl;
        std::cout<<"BOX MIN: "<<boxMin<<std::endl;
        std::cout<<"POS MAX: "<<posMax<<std::endl;
        std::cout<<"POS MIN: "<<posMin<<std::endl;
        if (yCollision) {
            if (oldPosition.getCenter().at(1) > box.getCenter().at(1)) {
                top = true;
            } else {
                bottom = true;
            }
        }
        if (xCollision) {
            if (oldPosition.getCenter().at(0) > box.getCenter().at(0)) {
                right = true;
            } else {
                left = true;
            }
        }
//        
//        
//        
//        if (std::abs(x) > std::abs(y)) {
//            //Movement in x direction
//            if (x < 0) {
//                return vmml::Vector<4,bool>(true,false,false,false);
//            } else {
//            }
//        } else {
//            //Movement in y direction
//            if (y < 0) {
//                std::cout<<"Y COLLISION "<<box.getMax().at(1)<<","<<box.getMin().at(1)<<std::endl;
//                std::cout<<"Y COLLISION1 "<<newPosition.getMax().at(1)<<","<<newPosition.getMin().at(1)<<std::endl;
//                std::cout<<"Y COLLISION2 "<<oldPosition.getMax().at(1)<<","<<oldPosition.getMin().at(1)<<std::endl;
//                //To bottom
//                return vmml::Vector3f(movedBy.at(0),box.getMax().at(1) - newPosition.getMin().at(1),0.0f);
//            } else {
//                //To Top
//                return vmml::Vector3f(movedBy.at(0),box.getMin().at(1) - newPosition.getMin().at(1),0.0f);
//            }
//        }
        return vmml::Vector<4,bool>(left,right,top,bottom);
//        vmml::Vector3f(
//        std::cout<<"MAX: "<<box.getMax()<<std::endl;
//        std::cout<<"MIN: "<<box.getMin()<<std::endl;
//        std::cout<<"OLD POS: "<<oldPosition<<std::endl;
//        std::cout<<"POS: "<<newPosition<<std::endl;
//        std::cout<<"MOVED: "<<movedBy<<std::endl;
//    }
    }
    
    
    
    bool hasCollision(vmml::Vector<4,bool> collision) {
        return collision.at(0) || collision.at(1) || collision.at(2) || collision.at(3);
    }
    
    vmml::Vector3f getCollision(vmml::AABBf oldbox,vmml::AABBf newbox) {
        vmml::Vector3f movedBy = newbox.getCenter() - oldbox.getCenter();
        std::list<Entity>::iterator iterator;
        float xtrans = 0;
        float ytrans = 0;
        for (iterator = entities.begin(); iterator != entities.end(); ++iterator) {
            
            vmml::AABBf boundingBox2 = brenderer.getObjects()->getModel(iterator->getObjName())->getBoundingBoxObjectSpace();
            vmml::AABBf box2(iterator->getPos() * boundingBox2.getMin(),iterator->getPos() * boundingBox2.getMax());
            vmml::Vector3f boxMax = box2.getMax();
            vmml::Vector3f boxMin = box2.getMin();
            vmml::Vector3f posMax = newbox.getMax();
            vmml::Vector3f posMin = newbox.getMin();
            // Upon collision, move 0.01 up, until no collision is detected (still need to check with all other objects for collision, therefore no break)
            if (box2.isIn2d(newbox.getMin()) || box2.isIn2d(newbox.getMax())){
                std::cout<<"BOX "<<boxMax.at(0)<<","<<boxMin.at(0)<<std::endl;
                std::cout<<"POS "<<posMax.at(0)<<","<<posMin.at(0)<<std::endl;
                vmml::Vector<4,bool> collision = checkCollision(box2,oldbox,newbox);
                std::cout<<"COLLISION "<<collision<<std::endl;
                if (collision.at(2)) {
                    ytrans = box2.getMax().at(1) - newbox.getMin().at(1)+0.001;
                }
                if (collision.at(1)) {
                    xtrans = box2.getMax().at(0)-newbox.getMax().at(0)+0.001;
                }
                if (collision.at(0)) {
                    xtrans = box2.getMin().at(0)-newbox.getMin().at(0)-0.001;
                }

            }
        }
        std::cout<<"MOVED BY"<< movedBy<<std::endl;
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
                std::cout<<"COL BE "<<col<<std::endl;
                moveableIterator->move(col);
                newbox.set(moveableIterator->getPos() * boundingBox.getMin(),moveableIterator->getPos() * boundingBox.getMax());
                col = getCollision(oldbox,newbox);
                std::cout<<"COL AF "<<col<<std::endl;
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