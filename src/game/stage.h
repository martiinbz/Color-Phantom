#pragma once
#include "framework/includes.h"

#include "framework/camera.h"
#include "framework/entities/entity_ui.h"

enum {
    STAGE_INTRO,
    STAGE_TUTORIAL,
    STAGE_MENU,
    STAGE_L1,
    STAGE_L2,
    STAGE_L3,
    STAGE_L4,
    STAGE_L5,
    STAGE_SETTINGS
};

class Stage {
protected:
    bool mouse_locked; //tells if the mouse is locked (not seen)
    bool opened_menu = false;
    bool L1_completed = false;
    bool L2_completed = false;
    bool L3_completed = false;
    bool L4_completed = false;
    bool L5_completed = false;

public:
    virtual void init() {};
    virtual void update(double dt) {};
    virtual void render(Camera* camera) {};
    virtual void onLeave(Stage* stage) {};
	virtual void onEnter(Stage* stage) {};
	
};

class IntroStage : public Stage {

public:
    void init() override;
    void render(Camera* camera) override;
    void update(double dt) override;
	void onEnter(Stage* stage) override;
	void onLeave(Stage* stage) override;
};

class  MenuStage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

class  TutorialStage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

class SettingsStage : public Stage {

public:
    void init() override;
    void render(Camera* camera) override;
    void update(double dt) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

// LEVEL STAGES

class  L1Stage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

class  L2Stage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

class  L3Stage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

class  L4Stage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};

class  L5Stage : public Stage {

public:
    void init() override;
    void update(double dt) override;
    void render(Camera* camera) override;
    void onEnter(Stage* stage) override;
    void onLeave(Stage* stage) override;
};