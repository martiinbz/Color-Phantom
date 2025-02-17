#pragma once
#ifndef STAGE_H
#define STAGE_H
#include "framework/includes.h"

#include "framework/camera.h"

enum {
    STAGE_PLAY,
    STAGE_MENU
};

class Stage {
protected:
    bool mouse_locked; //tells if the mouse is locked (not seen)

public:
    virtual void init() {};
    virtual void update(double dt) {};
    virtual void render(Camera* camera) {};
    virtual void onLeave(Stage* stage) {};
	virtual void onEnter(Stage* stage) {};
	
};

class MenuStage : public Stage {

public:
    MenuStage();
    void render(Camera* camera) override;
    void update(double dt) override;
	void onEnter(Stage* stage) override;
	void onLeave(Stage* stage) override;
};

class  PlayStage : public Stage {
  
public:
    PlayStage();
    void update(double dt) override;
    void render(Camera* camera) override;
	void onEnter(Stage* stage) override;
	void onLeave(Stage* stage) override;
};
#endif


