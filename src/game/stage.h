#pragma once
#ifndef STAGE_H
#define STAGE_H
#include "framework/includes.h"

#include "framework/camera.h"



class Stage {
protected:
    


public:

    virtual void update(double dt) {};
    virtual void render(Camera* camera) {};
    
	
};



class  PlayStage : public Stage {

   

public:
   

    PlayStage();
  
    

    void update(double dt) override;
    void render(Camera* camera) override;
    bool isValid(Vector2 target) override;
	bool isFloor(Vector2 target) override;
	
};



#endif


