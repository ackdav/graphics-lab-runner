
//
//  Background.h
//  bRenderer_ios
//
//  Created by Orell Basil Endres (oendre) on 03/05/16.
//
//

#ifndef bRenderer_ios_Background_h
#define bRenderer_ios_Background_h

class Background: public Entity{
    
private:
    
public:
    
    Background(vmml::Vector3f(translate)):Entity(vmml::Vector3f(1.0f),vmml::Vector3f(0.0f,-3.0f,0.0f) + translate,"background","backgroundPlane"){}
    
    Background():Entity(vmml::Vector3f(0.3f),vmml::Vector3f(-100.0f,1.0f,0.0f),vmml::Vector3f(0,1,0),M_PI_F*3/2,"background","backgroundPlane"){}
    
};



#endif
