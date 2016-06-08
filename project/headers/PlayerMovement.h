//
//  PlayerMovement.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_PlayerMovement_h
#define bRenderer_ios_PlayerMovement_h

#include "Movement.h"
#include <map>


class PlayerMovement: public Movement {
private:
    
    Renderer bRenderer;
    std::list<Entity> buttons;
    
    std::map<std::string, vmml::AABBf> boxes;
    
    std::list<Entity>::iterator buttonIterator;
    
    bool checkButton(std::string button,vmml::Vector2f click) {
        std::map<std::string, vmml::AABBf>::iterator boxIterator = boxes.find(button);
        if (boxIterator != boxes.end()) {
            return boxIterator->second.isIn2d(click);
        }
        return false;
    }
    
public:
    
    PlayerMovement(Renderer _bRenderer,std::list<Entity> _buttons):bRenderer(_bRenderer),buttons(_buttons){
        for (buttonIterator = buttons.begin(); buttonIterator != buttons.end(); ++buttonIterator){
            vmml::AABBf boundingBox = bRenderer.getObjects()->getModel(buttonIterator->getObjName())->getBoundingBoxObjectSpace();
            vmml::AABBf box(buttonIterator->getPos() * boundingBox.getMin(),buttonIterator->getPos() * boundingBox.getMax());
            boxes[buttonIterator->getObjName()] = box;
        }

    }

    vmml::Vector<4, bool > getMove() {
                TouchMap touchMap = bRenderer.getInput()->getTouches();
                vmml::Vector<4,bool> result(false,false,false,false);
                for (auto t = touchMap.begin(); t != touchMap.end(); ++t) {
                    Touch touch = t->second;
                    float x = (2.0f*((float)(touch.startPositionX-0)/(bRenderer.getView()->getScreenWidth()-0)))-1.0f;
                    float y =1.0f-(2.0f*((float)(touch.startPositionY-0)/(bRenderer.getView()->getScreenHeight()-0)));
                    vmml::Vector2f click = vmml::Vector2f(x,y);
                    result = vmml::Vector<4,bool>(checkButton("bLeft",click),checkButton("bRight",click),checkButton("bUp",click),checkButton("bTarget",click));
                }
        return result;
    }
};
#endif
