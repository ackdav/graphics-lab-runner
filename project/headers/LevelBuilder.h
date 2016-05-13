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

class LevelBuilder {
private:
    Renderer bRenderer;
    Entity player;
    Entity skyplane;
    
    std::list<Entity> entities;
    std::list<Entity> moveableEntities;
    
    
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
        } else if (std::strcmp(index.c_str(),"2") ==0) {
            PlayerMovement movement;
            builder.setObjectName("minecraftcharacter").setShaderName("guy").setIsMoving(true).setFacing(2).setMovement(&movement);
        } else if (std::strcmp(index.c_str(),"3") ==0) {
            builder.setObjectName("minecraftcharacter").setShaderName("guy").setIsMoving(true).setFacing(1);
        }
        //There is an element
        if (std::strcmp(index.c_str(),"0") !=0) {
            std::cout<<builder.getObjectName()<<std::endl;
            vmml::AABBf boundingBox = bRenderer.getObjects()->getModel(builder.getObjectName())->getBoundingBoxObjectSpace();
            builder.setScale(vmml::Vector3f(1/boundingBox.getDimension().find_max())).setTranslation(vmml::Vector3f(column-colCenter,row+rowCenter,0));
            if (builder.isMoving()) {
                MoveableEntity entity = builder.createMoveableEntity();
                moveableEntities.push_back(entity);
                if (std::strcmp(index.c_str(),"2") ==0) {
                    player = entity;
                }
            } else {
                entities.push_back(builder.createEntity());
            }
        }
    }
    
public:
    
    LevelBuilder(Renderer _brenderer):bRenderer(_brenderer){
        // TODO: Load level from txt file
        level = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 2 0 0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 3 0 0 0 0\n1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
        std::vector<std::string> rows = split(level,'\n');
        int row = 0;
        for(std::vector<std::string>::iterator it = rows.begin(); it != rows.end(); ++it) {
            int column = 0;
            std::vector<std::string> elements = split(*it,' ');
            for(std::vector<std::string>::iterator it2 = elements.begin(); it2 != elements.end(); ++it2) {
                addElement(*it2,row,column);
                column++;
            }
            row--;
        }
        EntityBuilder builder;
        builder.setObjectName("backgroundPlane").setShaderName("background").setScale(vmml::Vector3f(3.0f)).setTranslation(vmml::Vector3f(1,1,10)).setFacing(1);
        skyplane = builder.createEntity();
        
    }
    
    ~LevelBuilder(){
        
    }
    
    std::list<Entity> getEntities() {
        return entities;
    }
    
    std::list<Entity> getMoveableEntities() {
        return moveableEntities;
    }
    
    Entity getPlayer() {
        return player;
    }
    
    Entity getSkyplane() {
        return skyplane;
    }
    
};

#endif
