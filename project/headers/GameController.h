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
#include "GameObject.h"

class GameController {
    
    
private:
    std::list<Entity> entities;
    Entity player;
    Renderer brenderer;
    vmml::Vector3f trans;
    vmml::Matrix4f CurrentIteratorPos;
    float ycoordinate = 0.f;
    
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
            xVec += 0.05;
        }
        
        if(direction==2){
            xVec -= 0.05;
            if (jumpable == true){
                yVec += 0.9;}
        }
        
        return vmml::Vector3f(xVec,yVec,zVec);
    };;
    };

#endif
