//
//  Movement.h
//  bRenderer_ios
//
//  Created by dev_lab on 10/05/16.
//
//

#ifndef bRenderer_ios_Movement_h
#define bRenderer_ios_Movement_h

#include "bRenderer.h"

class Movement {
    
private:
    
    double durationFlying;
    std::map <int, double> runTimes;
    std::map <int, double> stepSizes;
    std::map <int, double> decelerateTimes;
    
public:
    
    Movement() {
        runTimes[0] = 0.0f;
        runTimes[1] = 0.0f;
        stepSizes[0] = 0.0f;
        stepSizes[1] = 0.0f;
        decelerateTimes[0] = 0.0f;
        decelerateTimes[1] = 0.0f;
    }

    virtual vmml::Vector<4, bool > getMove() = 0;
    
    void setDurationFlying(double _durationFlying) {
        durationFlying = _durationFlying;
    }
    
    double getDurationFlying() {
        return durationFlying;
    }
    
    double getStepAccellerate(double elapsedTime,int direction) {
        double runTime = runTimes.find(direction)->second;
        runTime += elapsedTime;
        runTimes[direction] = runTime;
        std::cout<<"ELAPSED TIME RUNNING: "<<runTime<<std::endl;
        float step = .5/(1+exp(-0.5*(runTime-4)));
        step = std::max(step,0.1f);
        stepSizes[direction] = step;
        return step;
    }
    
    double getStepDeccellerate(double elapsedTime,int direction) {
        double lastStepSize = stepSizes.find(direction)->second;
        if (lastStepSize == 0.0f) {
            return lastStepSize;
        }
        double runTime = runTimes.find(direction)->second;
        if (runTime > 0.0f) {
            runTimes[direction] = 0.0f;
        }
        double decelerateTime = decelerateTimes.find(direction)->second;
        decelerateTime += elapsedTime;
        std::cout<<"ELAPSED TIME DECELERATING: "<<decelerateTime<<std::endl;
        float step = .5/(1+exp(-0.6*(-decelerateTime-4)));
        if (step < 0.01f){
            lastStepSize = 0.0f;
            decelerateTime = 0.0f;
        } else {
            lastStepSize = step;
        }
        decelerateTimes[direction] = decelerateTime;
        stepSizes[direction] = lastStepSize;
        return lastStepSize;
    }
    
};


#endif
