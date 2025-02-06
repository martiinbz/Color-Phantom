#pragma once
#ifndef STAGE_H
#define STAGE_H
#include "framework/includes.h"

#include "framework/camera.h"



class Stage {
protected:
    bool mouse_locked; //tells if the mouse is locked (not seen)


public:

    virtual void update(double dt) {};
    virtual void render(Camera* camera) {};
    
	
};



class  PlayStage : public Stage {

   

public:
   

    PlayStage();
  
    

    void update(double dt) override;
    void render(Camera* camera) override;
  
};



#endif


