#pragma once
#ifndef STAGE_H
#define STAGE_H
#include "framework/includes.h"

#include "framework/camera.h"
#include "framework/entities/entity_ui.h"

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

	
    EntityUI* background = nullptr;
    EntityUI* play_button = nullptr;
    EntityUI* exit_button = nullptr;


public:
    void init() override;
    void render(Camera* camera) override;
    void update(double dt) override;
	void onEnter(Stage* stage) override;
	void onLeave(Stage* stage) override;
};

class  PlayStage : public Stage {
  
public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
	void onEnter(Stage* stage) override;
	void onLeave(Stage* stage) override;
};
#endif


