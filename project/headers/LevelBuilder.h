//
//  LevelBuilder.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_LevelBuilder_h
#define bRenderer_ios_LevelBuilder_h


#include "bRenderer.h"
#include "EntityBuilder.h"
#include "PlayerMovement.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdlib.h>

class LevelBuilder {
private:
    Renderer bRenderer;
    MoveableEntity player;
    Entity skyplane;
    Entity test;
    
    std::vector<Entity> buttons;
    
    std::list<Entity> entities;
    std::list<MoveableEntity> moveableEntities;
    
    GLfloat imageScale;
    
    std::string level;
    // Center of first screen
    int rowCenter = 3;
    int colCenter = 9;
    
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
    
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
    
    void addElement(std::string index, int row, int column) {
        //1 = Grassblock, 2 = Mario
        EntityBuilder builder;
        if (std::strcmp(index.c_str(),"1") ==0) {
            builder.setObjectName("block").setShaderName("guy");
        } else if (std::strcmp(index.c_str(),"E") ==0) {
            //PlayerMovement movement;
            std::cout<<"INITIALIZE SMURF "<<row<<","<<column<<std::endl;
//            builder.setObjectName("smurf").setShaderName("sprite_shader").setIsMoving(true).setMovement(new PlayerMovement(bRenderer,buttons));
        }  else if (std::strcmp(index.c_str(),"9") ==0) {
            //PlayerMovement movement;
            std::cout<<"INITIALIZE PLAYER "<<row<<","<<column<<std::endl;
            builder.setObjectName("smurf").setShaderName("sprite_shader").setIsMoving(true).setMovement(new PlayerMovement(bRenderer,buttons));
        } else if (std::strcmp(index.c_str(),"2") ==0) {
            //PlayerMovement movement;
            builder.setObjectName("minecraftcharacter").setShaderName("player").setIsMoving(true).setFacing(3).setMovement(new PlayerMovement(bRenderer,buttons));
        } else if (std::strcmp(index.c_str(),"3") ==0) {
            vmml::AABBf boundingBox = bRenderer.getObjects()->getModel("coin50")->getBoundingBoxObjectSpace();
            //set rotation !! Doesnt work yet
            builder.setObjectName("coin50").setShaderName("guy").setIsMoving(false).setFacing(2).setIsCollectible(true).setScale(vmml::Vector3f(1/boundingBox.getDimension().find_max())).setTranslation(vmml::Vector3f(column-colCenter,row+rowCenter,-0.5));
;
        }
        else if (std::strcmp(index.c_str(),"4") ==0) {
            vmml::AABBf boundingBox = bRenderer.getObjects()->getModel("coin20")->getBoundingBoxObjectSpace();
            //set rotation !!
            builder.setObjectName("coin20").setShaderName("guy").setIsMoving(false).setFacing(2).setIsCollectible(true).setScale(vmml::Vector3f(1/boundingBox.getDimension().find_max())).setTranslation(vmml::Vector3f(column-colCenter,row+rowCenter,-0.5));
;
        }
        else if (std::strcmp(index.c_str(),"A") ==0) {
            //set rotation !!
            builder.setObjectName("floating_tree1").setShaderName("guy").setIsMoving(false).setFacing(2);
        }
        else if (std::strcmp(index.c_str(),"C") ==0) {
            //set rotation !!
            builder.setObjectName("clouds").setShaderName("cloud").setIsMoving(false).setFacing(2);
        }
        else if (std::strcmp(index.c_str(),"F") ==0) {
            //set rotation !!
//            builder.setObjectName("frog").setShaderName("guy").setIsMoving(true).setFacing(4);
        }
        //There is an element
        if (std::strcmp(index.c_str(),"0") !=0) {
            vmml::AABBf boundingBox = bRenderer.getObjects()->getModel(builder.getObjectName())->getBoundingBoxObjectSpace();
            //Set Translation differently for background Stuff
            if (std::strcmp(index.c_str(),"A")==0){
                builder.setScale(vmml::Vector3f(1.f + 1./(arc4random_uniform(1.) + 1.))).setTranslation(vmml::Vector3f(column-colCenter,row+rowCenter,13. + arc4random_uniform(4.) ));
                }
                else if(std::strcmp(index.c_str(),"C")==0){
                     builder.setScale(vmml::Vector3f(1.f + 1/(arc4random_uniform(1.) + 1.) )).setTranslation(vmml::Vector3f(-3. + column-colCenter,5.+ row+rowCenter,8. + arc4random_uniform(4.) ));
                }
                else if(std::strcmp(index.c_str(),"9")==0){
                    builder.setScale(vmml::Vector3f(1/boundingBox.getDimension().find_max())).setTranslation(vmml::Vector3f(0,0,0));
                }
                else if(std::strcmp(index.c_str(),"3")!=0&&std::strcmp(index.c_str(),"4")!=0&&std::strcmp(index.c_str(),"2")!=0){
                builder.setScale(vmml::Vector3f(1/boundingBox.getDimension().find_max())).setTranslation(vmml::Vector3f(column-colCenter,row+rowCenter,0));
                }
            
            if (std::strcmp(index.c_str(),"2") ==0) {
                std::cout<<"SPRITE PLAYER :" <<boundingBox.getDimension().find_max()<<std::endl;
            } else if (std::strcmp(index.c_str(),"9") ==0) {
                std::cout<<"BOX PLAYER :" <<boundingBox.getDimension().find_max()<<std::endl;
            }
            
            if (builder.isMoving() && std::strcmp(index.c_str(),"E")!=0) {
                MoveableEntity entity = builder.createMoveableEntity();
                moveableEntities.push_back(entity);
                if (std::strcmp(index.c_str(),"9") ==0) {
                    player = entity;
                }                
            } else if (std::strcmp(index.c_str(),"E")==0) {
                test = builder.createMoveableEntity();
            } else {
                entities.push_back(builder.createEntity());
            }
        }
    }
    
    void addButton(std::string name, std::string image, vmml::Vector3f translation) {
        bRenderer.getObjects()->createSprite(name, image,bRenderer.getObjects()->getShader("sprite_shader"));
        EntityBuilder builder;
        vmml::Matrix4f _viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
        builder.setScale(vmml::Vector3f(imageScale / bRenderer.getView()->getAspectRatio(), imageScale, imageScale)).setTranslation(translation).setViewMatrix(_viewMatrixHUD).setObjectName(name).setImage(image).setShaderName("sprite_shader");
        buttons.push_back(builder.createEntity());
    }
    
    void addSpriteImage(std::string name, std::string image, vmml::Vector3f translation) {
        //bRenderer.getObjects()->createSprite(name, image,bRenderer.getObjects()->getShader("sprite_shader"));
        EntityBuilder builder;
        //bRenderer.getObjects()->createSprite("plala2", "sprite_pl.png");
        vmml::Matrix4f _viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
        vmml::AABBf boundingBox = bRenderer.getObjects()->getModel(name)->getBoundingBoxObjectSpace();
        builder.setScale(vmml::Vector3f(1/boundingBox.getDimension().find_max())).setTranslation(translation).setViewMatrix(_viewMatrixHUD).setObjectName(name).setImage(image).setShaderName("sprite_shader");
        //builder.setScale(vmml::Vector3f(imageScale / bRenderer.getView()->getAspectRatio()*7, imageScale*7, imageScale*7)).setTranslation(translation).setViewMatrix(_viewMatrixHUD).setObjectName(name).setImage(image).setShaderName("sprite_shader");
        buttons.push_back(builder.createEntity());
    }
    
public:
    
    LevelBuilder(Renderer _brenderer):bRenderer(_brenderer){
        
        imageScale = 0.1f;
        
        //addSpriteImage("plala2","sprite_pl.png",vmml::Vector3f(0.55f, -0.9f, -0.00f));
        addButton("bRight","arrowR.png",vmml::Vector3f(0.85f, -0.9f, -0.00f));
        addButton("bLeft","arrowL",vmml::Vector3f(0.65f, -0.9f, -0.00f));
        addButton("bUp","arrowU.png",vmml::Vector3f(-0.55f, -0.9f, -0.00f));
        addButton("bTarget","target.png",vmml::Vector3f(-0.85f, -0.9f, -0.00f));
        
        //vmml::Vector3f trans = vmml::Vector3f(7-9,7+3,0);
        vmml::Vector3f trans = vmml::Vector3f(7-7,7+-10,0);
        //addSpriteImage("bPlayerSprite","smurf_sprite.png",vmml::Vector3f(-0.35f, -0.4f, -0.00f));
        addSpriteImage("smurf","smurf_sprite.png",trans);
        
        std::string line;
        std::ifstream myfile (bRenderer::getFilePath("level.txt"));
        if (myfile.is_open())
        {
            int row = 0;
            while ( std::getline (myfile,line) )
            {
                int column = 0;
                std::vector<std::string> elements = split(line,' ');
                for(std::vector<std::string>::iterator it2 = elements.begin(); it2 != elements.end(); ++it2) {
                    addElement(*it2,row,column);
                    column++;
                }
                row--;
            }
            myfile.close();
        }

        EntityBuilder builder;
        builder.setObjectName("backgroundPlane").setShaderName("background").setScale(vmml::Vector3f(4.8f)).setFacing(1);
        skyplane = builder.createEntity();
    }
    
    ~LevelBuilder(){
        
    }
    
    std::list<Entity> getEntities() {
        return entities;
    }
    
    std::list<MoveableEntity> getMoveableEntities() {
        return moveableEntities;
    }
    
    std::vector<Entity> getButtons() {
        return buttons;
    }
    
    MoveableEntity getPlayer() {
        return player;
    }
    
    Entity getSkyplane() {
        return skyplane;
    }
        
    Entity getTest() {
        return test;
    }
    
    
    
};

#endif
