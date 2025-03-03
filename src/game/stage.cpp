#include "stage.h"
#include "game.h"
#include "framework/utils.h"
#include <iostream>
#include "framework/input.h"
#include "game/scene_parser.h"
#include "framework/entities/entity.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "framework/player.h"
#include "framework/world.h"
#include "framework/entities/entity_ui.h"

// VARIABLES PARA GESTIONAR LOS NIVELES

bool opened_menu = false;
bool L1_completed = false;
bool L2_completed = false;
bool L3_completed = false;

bool showL1 = true;
bool showL2 = false;
bool showL3 = false;
bool showCS = false;
bool showTutorial = false;


// INTRO STAGE

void IntroStage::init() {
}

void IntroStage::render(Camera* camera) {
    // fondo de pantalla
    UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/PORTADA.png");

    // boton play
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.625), Vector2(192, 64), "data/button/PLAY.png")) {
        opened_menu = true;
        Game::instance->goToStage(STAGE_MENU);
    }

    // settings play
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.75), Vector2(192, 64), "data/button/SETTINGS.png")) {
        Game::instance->goToStage(STAGE_SETTINGS);
    }

    // boton exit
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.875), Vector2(192, 64), "data/button/EXIT.png")) {
        exit(0);
    }
}

void IntroStage::update(double dt) {

}

void IntroStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false); // por si en otra escena estaba bloqueado
}

void IntroStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
}


// MENU STAGE

void MenuStage::init() {

}

void MenuStage::render(Camera* camera) {
    if (showL1) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/LEVEL1.png");
    if (showL2) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/LEVEL2.png");
    if (showL3) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/PORTADA.png");
    if (showTutorial) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/PORTADA.png");

    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.93), Vector2(192, 64), "data/button/PLAY.png")) {
        if (showL1) Game::instance->goToStage(STAGE_L1);
        if (showL2 && L1_completed) Game::instance->goToStage(STAGE_L2);
        if (showL3 && L2_completed) Game::instance->goToStage(STAGE_L3);
        if (showTutorial) Game::instance->goToStage(STAGE_TUTORIAL);
    }

    if (!showTutorial) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.05, Game::instance->window_height * 0.5), Vector2(32, 32), "data/button/FLECHAIZQ.png")) {
            if (showL1) {
                showL1 = false;
                showTutorial = true;
            }
            if (showL2) {
                showL2 = false;
                showL1 = true;
            }
            if (showL3) {
                showL3 = false;
                showL2 = true;
            }
            if (showCS) {
                showCS = false;
                showL3 = true;
            }
        }
    }

    if (!showCS) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.95, Game::instance->window_height * 0.5), Vector2(32, 32), "data/button/FLECHADER.png")) {
            if (showL1) {
                showL1 = false;
                showL2 = true;
            }
            if (showL2) {
                showL2 = false;
                showL3 = true;
            }
            if (showL3) {
                showL3 = false;
                showCS = true;
            }
            if (showTutorial) {
                showTutorial = false;
                showL1 = true;
            }
        }
    }

}

void MenuStage::update(double dt) {
    if (L1_completed && L2_completed && L3_completed) {
        // todo el juego completado, podemos implementar un titulo de victoria del juegador
    }
}

void MenuStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false);
}

void MenuStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
}


// TUTORIAL STAGE

void TutorialStage::init() {

}

void TutorialStage::render(Camera* camera) {

}

void TutorialStage::update(double dt) {
    
}

void TutorialStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false);
}

void TutorialStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
}


// LEVEL 1 STAGE

void L1Stage::init() {

}

void L1Stage::render(Camera* camera) {
 
    camera->enable();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

   
    World::get_instance()->render();

    drawGrid();
    drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
    Vector3 color = Player::instance->current_color;
	std::cout << "color: " << color.x << " " << color.y << " " << color.z << std::endl; 
    if (color.x==0 && color.y==0 && color.z==0) {
		std::cout << "has ganado!" << std::endl;

    }
}

void L1Stage::update(double seconds_elapsed) {
    World::get_instance()->update(seconds_elapsed);

    if (L1_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void L1Stage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(true);
}

void L1Stage::onLeave(Stage* stage) {
    
}


// LEVEL 2 STAGE

void L2Stage::init() {
}

void L2Stage::render(Camera* camera) {

}

void L2Stage::update(double seconds_elapsed) {
    World::get_instance()->update(seconds_elapsed);

    if (L2_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void L2Stage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(true);
}

void L2Stage::onLeave(Stage* stage) {

}


// LEVEL 3 STAGE

void L3Stage::init() {
}

void L3Stage::render(Camera* camera) {

}

void L3Stage::update(double seconds_elapsed) {
    World::get_instance()->update(seconds_elapsed);

    if (L3_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void L3Stage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(true);
}

void L3Stage::onLeave(Stage* stage) {

}


// SETTINGS STAGE

void SettingsStage::init() {

}

void SettingsStage::render(Camera* camera) {
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.875), Vector2(192, 64), "data/button/BACK.png")) {
        if (opened_menu) Game::instance->goToStage(STAGE_MENU);
        else Game::instance->goToStage(STAGE_INTRO);
    }
}

void SettingsStage::update(double dt) {

}

void SettingsStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false);
}

void SettingsStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
}